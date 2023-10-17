#pragma once

#include <winsdkver.h>
#include <assert.h>

#include <Global.h>
#include <Network.h>
#include <MySqlEngine.h>

#include "ChatServer.h"
#include "CsvParser.h"

typedef enum
{
	DATABASE_CAHTSERVER = 0,
	DATABASE_MAX
}databaseType;

extern Thread*	gThread[THREAD_MAX];
extern MySql*	gDatabase[DATABASE_MAX];

struct ServerConfig
{
	ServerConfig()
		:portNumber(0), maxUser(0)
	{}

	unsigned short	portNumber;
	unsigned int	maxUser;
};

struct DatabaseConnectInfo
{
	DatabaseConnectInfo()
		:portNumber(0)
	{
		ipAddress.clear();
		dbId.clear();
		dbPwd.clear();

		mainDbId.clear();
	}

	unsigned short portNumber;

	std::string ipAddress;
	std::string dbId;
	std::string dbPwd;

	std::string mainDbId;
};

extern ServerConfig			serverConfig;
extern DatabaseConnectInfo	dbConnectInfo;