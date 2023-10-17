#pragma once

#include "stdAfx.h"
#include "ChatUser.h"

#define BEGINMSG(PKID)		switch(PKID)\
							{

#define MSGMAP(MSG, FUNC)	case packetType::##MSG##_Tag:\
							{\
								auto ptr = std::make_unique<MSG>();\
								std::string message(pMsg, pPacketSize);\
								Stream::getBinary(*ptr, pMsg);\
								FUNC(pUser, ptr.get());\
							}\
							break;

#define ENDMSG()			default:\
								return false; }\
							return true;



class ChatServer : public NetworkService
{
public:
	ChatServer(int pPort, int pMax) : NetworkService(pPort, pMax)
	{
		assert(0 != pPort);
		assert(0 != pMax);
	};

	virtual void initializeThreadEx();
	virtual void startThreadEx();
	virtual void joinThreadEx();

	bool initializeDatabase();
};

class ChatServerInstance : public Singleton<ChatServerInstance>
{
public:
	bool dispatchTemplate(ChatUser* pUser, int pPacketId, int pPacketSize, std::string pMsg);

	bool registRequest(ChatUser* pUser, CS_RegistPacket_Req* pPacket);
	bool loginRequest(ChatUser* pUser, CS_LoginPacket_Req* pPacket);
	bool chatRequest(ChatUser* pUser, CS_MessagePacket_Req* pPacket);
};