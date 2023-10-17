#include "Global.h"

ObjectQueue	gPkRecvQueue;

void closeCallback(int pUid)
{
	LOGTRACE("Client Deleted On Map (Session Id : %d)", pUid);

	const auto& userManager = UserManager::getInstance();

	const auto& iter = userManager->getUserByUid(pUid);

	if (nullptr != iter)
		iter->setActive(false);
}

NetworkUser::NetworkUser()
	:socket_(nullptr), uid_(0), isActive_(false), userSerial_(0)
{
	LOGCRITICAL("User Create Error");

	ZeroMemory(userNick_, sizeof(userNick_));

	closeCallback(uid_);
}

NetworkUser::NetworkUser(tcp::socket socket, unsigned int sessionId)
	: socket_(std::move(socket)), uid_(sessionId), isActive_(true), userSerial_(0)
{
	LOGTRACE("User Session Id %d", sessionId);

	ZeroMemory(userNick_, sizeof(userNick_));

	doRead_();
}

void NetworkUser::doRead_()
{
	socket_.async_read_some(recvBuf,
		[this](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				LOGTRACE("Client Disconnected %d, ERROR : %s", getUid(), ec.message().c_str());
				closeCallback(getUid());
				socket_.close();
			}

			else
			{
#ifdef _DEBUG
				std::thread::id thisThreadID = std::this_thread::get_id();
				std::stringstream ss;
				ss << thisThreadID;
				std::string thisThreadidStr = ss.str();

				LOGTRACE("READ THREAD ID : %s", thisThreadidStr.c_str());
#endif // _DEBUG

				std::shared_ptr<ObjectItem> packet = std::make_shared<ObjectItem>(
					this,
					atoi(recvPacketId_),
					atoi(recvPacketSize_),
					std::string(recvPacketMsg_, atoi(recvPacketSize_)));

				gPkRecvQueue.push(packet);

				if (true == gThread[threadType::DISPATCH_THREAD]->isStop())
					gThread[threadType::DISPATCH_THREAD]->resume();

				recvBuf.empty();

				doRead_();
			}
		}
	);
}

void NetworkUser::doWrite_()
{
	boost::asio::async_write(socket_, sendBuf,
		[=](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				LOGTRACE("Server Write Fail (Client Id : %d, ERROR : %s)", getUid(), ec.message().c_str());
				closeCallback(getUid());
				socket_.close();
			}
			else
			{
#ifdef _DEBUG
				std::thread::id thisThreadID = std::this_thread::get_id();
				std::stringstream ss;
				ss << thisThreadID;
				std::string thisThreadidStr = ss.str();

				LOGTRACE("WRITE THREAD ID : %s", thisThreadidStr.c_str());
#endif // _DEBUG
			}

			sendBuf.empty();
		}
	);
}

void NetworkUser::sendMsg(Packet* pPacket, std::string pSendItem)
{
	char pkIdBuf[10] = "\0";
	char pkSizeBuf[10] = "\0";

	strcpy_s(sendPacketId_, 10, itoa(pPacket->packetId, pkIdBuf, 10));
	strcpy_s(sendPacketSize_, 10, itoa(static_cast<int>(pSendItem.size()), pkSizeBuf, 10));
	sendPacketMsg_ = pSendItem;

	sendBuf =
	{
		boost::asio::buffer(sendPacketId_),
		boost::asio::buffer(sendPacketSize_),
		boost::asio::buffer(sendPacketMsg_)
	};

	doWrite_();
}

void NetworkUser::sendMsgAll(Packet* pPacket, std::string pSendItem)
{
	const auto& userManager = UserManager::getInstance();

	boost::signals2::signal<void(Packet*, std::string)> signal;

	for (int idx = 1; idx < userManager->getMax(); idx++)
	{
		auto iter = userManager->getUserByUid(idx);

		if (nullptr == iter) break;

		else if (false == iter->isActive() || getUid() == iter->getUid()) continue;

		signal.connect(0, boost::bind(&NetworkUser::sendMsg, iter, pPacket, pSendItem));
	}

	signal(pPacket, pSendItem);
}