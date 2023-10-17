#include "MySqlEngine.h"

MySql::MySql(const char* dbAddress, const char* dbId, const char* dbPwd, const char* dbName, unsigned short port) :
	conn_(), connPtr_(NULL), isConnected_(false)
{
	mysql_init(&conn_);

	connPtr_ = mysql_real_connect(
		&conn_,
		dbAddress,
		dbId,
		dbPwd,
		dbName,
		port,
		(char*)NULL,
		0);

	setIsConnected_(true);

	if (NULL == connPtr_)
	{
		CONSOLETRACE("Database (%s) Connect Failed / Server Shutdown", dbName);
		LOGCRITICAL("Database (%s) Connect Failed", dbName);
		setIsConnected_(false);
	}
	else
	{
		CONSOLETRACE("Database (%s) Connect Success", dbName);
	}
}

bool MySql::getIsConnected()
{
	return isConnected_;
}

bool MySql::setQuery(QUERYRESULT& pResult, const char* query, ...)
{
	MYSQL_ROW Row;
	MYSQL_RES* Result;

	char queryText[1000] = "\0";

	va_list args;

	va_start(args, query);
	vsprintf(queryText, query, args);
	va_end(args);

	int stat = mysql_query(connPtr_, queryText);

	if (0 != stat)
	{
		LOGCRITICAL("Request (%s) Failed", queryText);

		return false;
	}

	Result = mysql_store_result(connPtr_);

	if (NULL != Result)
	{
		while (NULL != (Row = mysql_fetch_row(Result)))
		{
			std::vector<QueryedItem> rowResult;
			for (unsigned int idx = 0; idx < mysql_field_count(connPtr_); idx++)
			{
				QueryedItem item = QueryedItem(Row[idx]);

				rowResult.push_back(item);
			}
			pResult.push_back(rowResult);
		}
	}

	mysql_free_result(Result);

	return true;
}

void MySql::setIsConnected_(bool flag)
{
	isConnected_ = flag;
}