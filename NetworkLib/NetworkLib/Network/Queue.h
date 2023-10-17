#pragma once
#include "Global.h"

template<typename Ty>
class Queue
{
public:
	Queue() {}

	virtual void push(std::shared_ptr<Ty> item) = 0;
	virtual std::shared_ptr<Ty> pop() = 0;
	virtual bool empty() = 0;
};

class ObjectItem
{
public:
	ObjectItem(NetworkUser* pUser, int pPacketId, int pSize, std::string pMsg)
	{
		user = pUser;
		objectId = pPacketId;
		objectSize = pSize;
		objectMsg = pMsg;
	}

	NetworkUser* user;
	int	objectId;
	int	objectSize;
	std::string objectMsg;
};

class ObjectQueue : public Queue<ObjectItem>
{
public:
	ObjectQueue() {}

	virtual void push(std::shared_ptr<ObjectItem> item)
	{
		packetQue_.push(item);
	}

	virtual std::shared_ptr<ObjectItem> pop()
	{
		std::shared_ptr<ObjectItem> packet;

		if (!packetQue_.try_pop(packet))
			return nullptr;

		return packet;
	}

	virtual bool empty()
	{
		return packetQue_.empty();
	}

private:
	tbb::concurrent_queue<std::shared_ptr<ObjectItem>> packetQue_;
};