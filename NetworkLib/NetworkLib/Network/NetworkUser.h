#pragma once

#include "Global.h"
#include "Packet.h"

using namespace boost::asio::ip;

class NetworkUser
{
public:
	NetworkUser();
	NetworkUser(tcp::socket socket, unsigned int sessionId);

	void sendMsg(Packet* pPacket, std::string pSendItem);
	void sendMsgAll(Packet* pPacket, std::string pSendItem);

	void setUid(unsigned int pUid) 
	{
		uid_ = pUid; 
	}

	void setActive(bool pIsActive) 
	{
		isActive_ = pIsActive; 
	}

	void setUserSerial(unsigned int pUserSerial) 
	{
		userSerial_ = pUserSerial; 
	}

	bool setUserNick(const char* pUserNick)
	{
		if (30 > strlen(pUserNick))
		{
			strcpy(userNick_, pUserNick);
			return true;
		}

		return false;
	}

	unsigned int getUid() const 
	{ 
		return uid_; 
	}

	bool isActive() const 
	{
		return isActive_;
	}

	int	getUserSerial() const
	{ 
		return userSerial_;
	}

	const char* getUserNick() const
	{
		return userNick_; 
	}

	friend class UserManager;
private:
	tcp::socket	 socket_;
	unsigned int uid_;
	bool isActive_;

	int	userSerial_;
	char userNick_[30];

	void doRead_();
	void doWrite_();

	char sendPacketId_[10];
	char sendPacketSize_[10];
	std::string sendPacketMsg_;

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
};

class UserManager : public Singleton<UserManager>
{
public:
	UserManager() {}

	void initialize(int pMax)
	{
		maxUser = pMax;
	}

	NetworkUser* getUserByUid(unsigned int pUid)
	{
		auto iter = userHashMap_.find(pUid);

		if (userHashMap_.end() == iter)
			return nullptr;

		return iter->second;
	}

	int getMax() const { return maxUser; }

	bool isActive(unsigned int pUid)
	{
		auto iter = userHashMap_.find(pUid);

		if (userHashMap_.end() == iter)
			return false;

		return iter->second->isActive();
	}

	void setActive(unsigned int pUid, bool pActive)
	{
		auto iter = userHashMap_.find(pUid);

		if (userHashMap_.end() == iter)
			return;

		auto user = iter->second;

		user->setActive(pActive);
	}

	void insertUser(unsigned int pUid, NetworkUser* pUser)
	{
		auto iter = userHashMap_.find(pUid);

		if (userHashMap_.end() == iter)
			userHashMap_.insert(std::make_pair(pUid, std::move(pUser)));
		else
		{
			{
				std::unique_lock<std::mutex> eraseLock(userMutex_);
				userHashMap_.unsafe_erase(iter);
			}
			userHashMap_.insert(std::make_pair(pUid, std::move(pUser)));
		}

		setActive(pUid, true);
	}

private:
	tbb::concurrent_unordered_map<unsigned int, NetworkUser*>	userHashMap_;
	std::mutex													userMutex_;
	std::atomic<int>											maxUser;
};