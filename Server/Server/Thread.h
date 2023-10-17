#pragma once

#include<Global.h>

class DispatchThread : public Thread
{
public:
	DispatchThread(int pThreadId) : Thread(pThreadId) {}

	virtual void Run();
};