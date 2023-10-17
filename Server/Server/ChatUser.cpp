#include "stdAfx.h"

bool ChatUser::dispatchProxy(int pPacketId, int pPacketSize, std::string pMsg)
{
	const auto& server = ChatServerInstance::getInstance();

	return server->dispatchTemplate(this, pPacketId, pPacketSize, pMsg);
}