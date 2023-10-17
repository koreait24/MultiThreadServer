#pragma once
#include "Global.h"

using namespace boost::asio::ip;

class NetworkService
{
public:
	NetworkService(int pPort, int pMax) : threadId_(0)
	{
		const auto& iter = UserManager::getInstance();

		iter->initialize(pMax);

		gPort = pPort;
	}

	void startServer();

	virtual void initializeThreadEx() = 0;
	virtual void startThreadEx() = 0;
	virtual void joinThreadEx() = 0;

protected:
	void initializeThread_();
	void startThread_();
	void joinThread_();

	boost::asio::io_service	ioService_{};

	std::vector<Thread*> threadVec_;

	std::atomic<int> threadId_;
};

class ClientService
{
public:
	ClientService(boost::asio::io_service& pIoService_
		, const std::string& pHost, const std::string& pPort) :socket_(pIoService_), uId_(0)
	{
		boost::asio::ip::tcp::resolver resolver(pIoService_);
		boost::asio::ip::tcp::resolver::query query(pHost, pPort);
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		boost::asio::async_connect(socket_, endpoint_iterator,
			boost::bind(&ClientService::handle_connect, this, boost::asio::placeholders::error));
	}

	void setUid(int pUid)
	{
		uId_ = pUid;
	}

	int	getUid()
	{
		return uId_;
	}

	void handle_connect(const boost::system::error_code& e)
	{
		if (!e)
		{
			LOGTRACE("Connected!");
		}
	}

	void closeCallback();
	void packetReceiver();
	void packetWriter();
	void packetSender();

private:
	int	uId_;

	char sendPacketId_[10];
	char sendPacketSize_[10];
	std::string	sendPacketMsg_;

	boost::array<boost::asio::mutable_buffer, 3> sendBuf =
	{
		boost::asio::buffer(sendPacketId_),
		boost::asio::buffer(sendPacketSize_),
		boost::asio::buffer(sendPacketMsg_)
	};

	char recvPacketId_[10];
	char recvPacketSize_[10];
	char recvPacketMsg_[4096];

	boost::array<boost::asio::mutable_buffer, 3> recvBuf =
	{
		boost::asio::buffer(recvPacketId_),
		boost::asio::buffer(recvPacketSize_),
		boost::asio::buffer(recvPacketMsg_)
	};

	tcp::socket socket_;
};