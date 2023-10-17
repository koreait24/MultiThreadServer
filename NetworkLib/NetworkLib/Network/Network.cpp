#include "Network.h"

using namespace boost::asio::ip;

Thread*				gThread[THREAD_MAX];
std::atomic<int>	gPort;

void NetworkService::startServer()
{
	initializeThread_();
	startThread_();
	joinThread_();
}

void NetworkService::initializeThread_()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	const auto& iter = UserManager::getInstance();

	LOGTRACE("Listen On Port %d, MaxUser %d", gPort.load(), iter->getMax());

	for (unsigned int idx = 0; idx < sysinfo.dwNumberOfProcessors * 2; idx++)
	{
		threadVec_.emplace_back(new WorkerThread(&ioService_, threadId_));
		threadVec_[idx]->setThreadId(threadId_);

		threadId_ += 1;
	}
}

void NetworkService::startThread_()
{
	for (const auto& iter : threadVec_)
	{
		iter->start();
	}
}

void NetworkService::joinThread_()
{
	for (const auto& iter : threadVec_)
	{
		iter->join();
	}
}

void ClientService::closeCallback()
{
	socket_.close();
}

void ClientService::packetReceiver()
{
	socket_.async_read_some(recvBuf,
		[this](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				LOGTRACE("Client Disconnected %d", getUid());
				closeCallback();
			}
			else
			{
				//LOGTRACE("Client (Session Id : %d)", getUid());

				switch (atoi(recvPacketId_))
				{
				case packetType::SC_RegistPacket_Ans_Tag:
				{
					auto ptr = std::make_unique<SC_RegistPacket_Ans>();

					std::string recvMsg(recvPacketMsg_, atoi(recvPacketSize_));

					Stream::getBinary(*ptr, recvMsg);

					if (true == ptr->isSuccess)
						LOGTRACE("PACKET  %d : REGISTERED SUCCESS", atoi(recvPacketId_));
					else
						LOGTRACE("PACKET  %d : REGISTERED FAILED", atoi(recvPacketId_));
				}
				break;

				case packetType::SC_LoginPacket_Ans_Tag:
				{
					auto ptr = std::make_unique<SC_LoginPacket_Ans>();

					std::string recvMsg(recvPacketMsg_, atoi(recvPacketSize_));

					Stream::getBinary(*ptr, recvMsg);

					if (true == ptr->isSuccess)
						LOGTRACE("PACKET  %d : LOGIN SUCCESS", atoi(recvPacketId_));
					else
						LOGTRACE("PACKET  %d : LOGIN FAILED", atoi(recvPacketId_));
				}
				break;

				case packetType::SC_MessagePacket_Ans_Tag:
				{
					auto ptr = std::make_unique<SC_MessagePacket_Ans>();

					std::string recvMsg(recvPacketMsg_, atoi(recvPacketSize_));

					Stream::getBinary(*ptr, recvMsg);

					LOGTRACE("%s : %s", ptr->requestNick, ptr->requestMsg);
				}
				break;
				}

				recvBuf.empty();

				packetReceiver();
			}
		}
	);
}

void ClientService::packetWriter()
{
	while (true)
	{
		CS_RegistPacket_Req packet = CS_RegistPacket_Req("koreait28", "altmrha12", "JGS24");

		//CS_LoginPacket_Req packet = CS_LoginPacket_Req("koreait24", "altmrha12");

		//CS_MessagePacket_Req packet = CS_MessagePacket_Req("koreait", "Hello, World!");

		char pkIdBuf[10] = "\0";
		char pkSizeBuf[10] = "\0";

		sendPacketMsg_ = Stream::putBinary(packet);

		strcpy_s(sendPacketId_, 10, itoa(packet.packetId, pkIdBuf, 10));
		strcpy_s(sendPacketSize_, 10, itoa(static_cast<int>(sendPacketMsg_.size()), pkSizeBuf, 10));

		sendBuf =
		{
			boost::asio::buffer(sendPacketId_),
			boost::asio::buffer(sendPacketSize_),
			boost::asio::buffer(sendPacketMsg_)
		};

		packetSender();
		Sleep(100);
	}
}

void ClientService::packetSender()
{
	boost::asio::async_write(socket_, sendBuf,
		[=](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				LOGCRITICAL("Client Write Fail (Client Id : %d)", getUid());
				closeCallback();
			}
			else
			{
				LOGTRACE("Send Client (Client Id : %d)", getUid());
			}
		}
	);
}