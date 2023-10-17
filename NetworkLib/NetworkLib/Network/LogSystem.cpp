#include "LogSystem.h"

LogSystem::LogSystem() :logTrace_(NULL), logDebug_(NULL), logCritical_(NULL)
{
	time_t curTime = time(NULL);

	struct tm* pLocal = NULL;
#if defined(_WIN32) || defined(_WIN64) 
	pLocal = localtime(&curTime);
#else 
	localtime_r(&curTime, pLocal);
#endif 
	if (pLocal == NULL)
	{
		return;
	}

	std::string logDebug = ".\\Bin\\Logs\\" + std::to_string(pLocal->tm_year + 1900) + "_"
		+ std::to_string(pLocal->tm_mon + 1) + "_" + std::to_string(pLocal->tm_mday) + "LogDebug_.log";

	std::string logTrace = ".\\Bin\\Logs\\" + std::to_string(pLocal->tm_year + 1900) + "_"
		+ std::to_string(pLocal->tm_mon + 1) + "_" + std::to_string(pLocal->tm_mday) + "LogTrace_.log";

	std::string logCritical = ".\\Bin\\Logs\\" + std::to_string(pLocal->tm_year + 1900) + "_"
		+ std::to_string(pLocal->tm_mon + 1) + "_" + std::to_string(pLocal->tm_mday) + "LogCritical_.log";

	logDebug_ = fopen(logDebug.c_str(), "at");
	logTrace_ = fopen(logTrace.c_str(), "at");
	logCritical_ = fopen(logCritical.c_str(), "at");
}

LogSystem::~LogSystem()
{
	if (NULL != logDebug_)
		fclose(logDebug_);
	if (NULL != logTrace_)
		fclose(logTrace_);
	if (NULL != logCritical_)
		fclose(logCritical_);
}

void LogSystem::getLogMessage(const char* fileName, const char* funcName, int errLine, int type, const char* explain, ...)
{
	LogSystem logSystem;

	if (NULL == fileName)
		return;
	if (NULL == funcName)
		return;
	if (NULL == explain)
		return;

	va_list args;

	va_start(args, explain);
	logSystem.writeErrorLog(fileName, funcName, errLine, type, explain, args);
	va_end(args);
}

void LogSystem::writeErrorLog(const char* fileName, const char* funcName, int errLine, int type, const char* explain, va_list args)
{
	LogSystem logSystem;;

	time_t curTime = time(NULL);

	struct tm* pLocal = NULL;
#if defined(_WIN32) || defined(_WIN64) 
	pLocal = localtime(&curTime);
#else 
	localtime_r(&curTime, pLocal);
#endif 
	if (pLocal == NULL)
	{
		return;
	}

	switch (type)
	{
	case ERR_DEBUG:
	{
		std::lock_guard<std::mutex> debugLock(debugMutex_);
		fprintf(logSystem.logDebug_, "\n%02d:%02d:%02d %s %s %d ", pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec, fileName, funcName, errLine);
		vfprintf(logSystem.logDebug_, explain, args);
	}
	break;

	{
	case ERR_TRACE:
	{
		std::lock_guard<std::mutex> traceLock(traceMutex_);
		{
			fprintf(logSystem.logTrace_, "\n%02d:%02d:%02d %s %s %d ", pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec, fileName, funcName, errLine);
			vfprintf(logSystem.logTrace_, explain, args);
		}
	}

	case LOG_TRACE:
	{
		printf("\n%s %d ", funcName, errLine);
		vprintf(explain, args);
	}
	}
	break;

	case ERR_CRITICAL:
	{
		std::lock_guard<std::mutex> criticalLock(criticalMutex_);
		fprintf(logSystem.logCritical_, "\n%02d:%02d:%02d %s %s %d ", pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec, fileName, funcName, errLine);
		vfprintf(logSystem.logCritical_, explain, args);
	}
	break;
	}
}