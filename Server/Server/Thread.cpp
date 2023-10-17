#include "stdAfx.h"
#include "Thread.h"

void DispatchThread::Run()
{
	while (true == isStop())
	{
		if (true == gPkRecvQueue.empty())
		{
			this->suspend();
		}

		const auto& packet = gPkRecvQueue.pop();

		if (nullptr == packet)
		{
			LOGTRACE("DROP PACKET");
			continue;
		}

		auto getUser = packet->user;

		if (nullptr == getUser)
			continue;

		else if (false == getUser->isActive())
			continue;

		ChatUser* user = reinterpret_cast<ChatUser*>(getUser);

		const auto& server = ChatServerInstance::getInstance();

		server->dispatchTemplate(
			user,
			packet->objectId,
			packet->objectSize,
			packet->objectMsg
		);
	}
}