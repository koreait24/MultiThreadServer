#pragma once
#include "Global.h"

#define		LOGDEBUG(explain, ...)		LogSystem::getLogMessage(__FILE__, __FUNCTION__, __LINE__, ERR_DEBUG, explain, __VA_ARGS__)
#define		LOGTRACE(explain, ...)		LogSystem::getLogMessage(__FILE__, __FUNCTION__, __LINE__, ERR_TRACE, explain, __VA_ARGS__)
#define		LOGCRITICAL(explain, ...)	LogSystem::getLogMessage(__FILE__, __FUNCTION__, __LINE__, ERR_CRITICAL, explain, __VA_ARGS__)
#define		CONSOLETRACE(explain, ...)	LogSystem::getLogMessage(__FILE__, __FUNCTION__, __LINE__, LOG_TRACE, explain, __VA_ARGS__)

#define		LOG_TRACE			0
#define		ERR_DEBUG			10
#define		ERR_TRACE			20
#define		ERR_CRITICAL		30

class LogSystem
{
public:
	static void getLogMessage(const char* fileName, const char* funcName, int errLine, int type, const char* explain, ...);

private:
	LogSystem();
	~LogSystem();
	void writeErrorLog(const char* fileName, const char* funcName, int errLine, int type, const char* explain, va_list args);

	FILE* logDebug_;
	FILE* logCritical_;
	FILE* logTrace_;

	std::mutex debugMutex_;
	std::mutex criticalMutex_;
	std::mutex traceMutex_;
};