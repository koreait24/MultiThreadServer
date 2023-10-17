#include <sdkddkver.h>
#include "stdAfx.h"

#pragma comment(lib, "NetworkLib.lib")

#include <Network.h>

ServerConfig		serverConfig;
DatabaseConnectInfo	dbConnectInfo;

bool initializeServerConfig(std::string pFilePath)
{
	auto parser	= CSVParser(pFilePath);
	CSVDATAVEC dataVec = parser.getCSVDataVec();

	if (dataVec.empty())
		return false;

	SWITCH(dataVec)
	{
		CASE([PORT])
			SET_INT(serverConfig.portNumber);

		CASE([MAXUSER])
			SET_INT(serverConfig.maxUser);

		CASE([DBPORT])
			SET_INT(dbConnectInfo.portNumber);

		CASE([DBIP])
			SET_CHAR(dbConnectInfo.ipAddress);

		CASE([DBID])
			SET_CHAR(dbConnectInfo.dbId);

		CASE([DBPW])
			SET_CHAR(dbConnectInfo.dbPwd);

		CASE([MAINDB])
			SET_CHAR(dbConnectInfo.mainDbId);
	}

	return true;
}

int main()
{
	if (false == initializeServerConfig(".\\Config\\ServerConfig.csv"))
	{
		LOGTRACE("Intialize Server Config Failed!");
		return 0;
	}

	ChatServer* network = new ChatServer(serverConfig.portNumber, serverConfig.maxUser);

	if (false == network->initializeDatabase())
	{
		LOGTRACE("Intialize Database Service Failed!");
		return 0;
	}

	network->initializeThreadEx();
	network->startThreadEx();
	network->startServer();
	network->joinThreadEx();

	return 0;
}