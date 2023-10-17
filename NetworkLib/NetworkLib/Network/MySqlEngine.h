#pragma once
#include "Global.h"
#include <mysql.h>

#pragma comment(lib, "libmysql.lib")

class QueryedItem
{
public:
	QueryedItem(std::string pItem)
	{
		objectMsg_ = pItem;
	}

	int	getInt() const
	{
		return atoi(objectMsg_.c_str()); 
	}

	std::string	 getString() const
	{
		return objectMsg_;
	}

	const char* getChar()
	{
		return objectMsg_.c_str();
	}

private:
	std::string objectMsg_;
};

typedef std::vector<std::vector<QueryedItem>> QUERYRESULT;

class MySql
{
public:
	MySql(const char* dbAddress, const char* dbId, const char* dbPwd, const char* dbName, unsigned short port);

	bool getIsConnected();
	bool setQuery(QUERYRESULT& pResult, const char* query, ...);

private:
	void setIsConnected_(bool flag);

	MYSQL conn_;
	MYSQL* connPtr_;
	bool isConnected_;
};