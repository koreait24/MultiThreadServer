#include "stdAfx.h"
#include "Thread.h"

MySql* gDatabase[DATABASE_MAX];

void ChatServer::initializeThreadEx()
{
	gThread[threadType::DISPATCH_THREAD] = new DispatchThread(threadId_ += threadType::DISPATCH_THREAD);
}

void ChatServer::startThreadEx()
{
	for (int idx = 0; idx < threadType::THREAD_MAX; idx++)
	{
		gThread[idx]->start();
	}
}

void ChatServer::joinThreadEx()
{
	for (int idx = 0; idx < threadType::THREAD_MAX; idx++)
	{
		gThread[idx]->join();
	}
}

bool ChatServer::initializeDatabase()
{
	MySql* registerationDatabase = new MySql(
		dbConnectInfo.ipAddress.c_str(),
		dbConnectInfo.dbId.c_str(),
		dbConnectInfo.dbPwd.c_str(),
		dbConnectInfo.mainDbId.c_str(),
		dbConnectInfo.portNumber);

	gDatabase[databaseType::DATABASE_CAHTSERVER] = registerationDatabase;

	if (false == gDatabase[databaseType::DATABASE_CAHTSERVER]->getIsConnected())
	{
		LOGCRITICAL("Database (%s) Connecting Failed", dbConnectInfo.mainDbId.c_str());
		return false;
	}

	return true;
}

bool ChatServerInstance::dispatchTemplate(ChatUser* pUser, int pPacketId, int pPacketSize, std::string pMsg)
{
	BEGINMSG(pPacketId)
		MSGMAP(CS_RegistPacket_Req, registRequest)
		MSGMAP(CS_LoginPacket_Req, loginRequest)
		MSGMAP(CS_MessagePacket_Req, chatRequest)
	ENDMSG()
}

bool ChatServerInstance::registRequest(ChatUser* pUser, CS_RegistPacket_Req* pPacket)
{
	if (false == pUser->isActive())
		return false;

	std::string queryText = "SELECT * FROM chatserverusers WHERE UserId = '%s'";

	QUERYRESULT result;
	bool		bResult = false;

	if (true == gDatabase[databaseType::DATABASE_CAHTSERVER]->setQuery(result, queryText.c_str(), pPacket->requestId))
	{
		if (0 < result.size()) {}
		//LOGTRACE("Already Exist User ID %s", pPacket->requestId);

		else
		{
			queryText.clear();
			queryText = "INSERT INTO chatserverusers (UserId, UserPw, userNick) VALUES ('%s', '%s', '%s');";
			if (true == gDatabase[databaseType::DATABASE_CAHTSERVER]->setQuery(result,
				queryText.c_str(),
				pPacket->requestId,
				pPacket->requestPw,
				pPacket->requestNick))
			{
				bResult = true;
			}
			else
			{
				LOGTRACE("Already Exist User ID %s", pPacket->requestId);
			}
		}
	}

	SC_RegistPacket_Ans packet = SC_RegistPacket_Ans(bResult);

	std::string sendMsg = Stream::putBinary(packet);

	pUser->sendMsg(&packet, sendMsg);

	return true;
}

bool ChatServerInstance::loginRequest(ChatUser* pUser, CS_LoginPacket_Req* pPacket)
{
	if (false == pUser->isActive())
		return false;

	std::string queryText = "SELECT * FROM chatserverusers WHERE UserId = '%s' AND UserPw = '%s'";

	QUERYRESULT result;
	bool		bResult = false;

	bool loginResult = gDatabase[databaseType::DATABASE_CAHTSERVER]->setQuery(result, queryText.c_str(), pPacket->requestId, pPacket->requestPw);

	if (true == loginResult)
	{
		auto& frontInfo = result.front();
		bool	isAlready = true;

		const auto& userManager = UserManager::getInstance();

		for (int idx = 1; idx < userManager->getMax(); idx++)
		{
			if (pUser->getUid() == idx) continue;

			const auto& userIter = userManager->getUserByUid(idx);

			if (nullptr == userIter) break;

			else if (false == userIter->isActive()) continue;

			if (userIter->getUserSerial() == frontInfo[0].getInt())
			{
				isAlready = false;
				break;
			}
		}

		if (false != isAlready)
		{
			pUser->setUserSerial(frontInfo[0].getInt());
			pUser->setUserNick(frontInfo[3].getChar());

			bResult = true;
		}
	}

	else
	{
		LOGTRACE("LOGIN FAILED (SessionId : %d)", pUser->getUid());
	}

	SC_LoginPacket_Ans packet = SC_LoginPacket_Ans(bResult);

	std::string sendMsg = Stream::putBinary(packet);

	pUser->sendMsg(&packet, sendMsg);

	return true;
}

bool ChatServerInstance::chatRequest(ChatUser* pUser, CS_MessagePacket_Req* pPacket)
{
	if (false == pUser->isActive())
		return false;

	SC_MessagePacket_Ans packet = SC_MessagePacket_Ans(pPacket->requestNick, pPacket->requestMsg);

	std::string sendMsg = Stream::putBinary(packet);

	pUser->sendMsgAll(&packet, sendMsg);

	return true;
}