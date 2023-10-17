#pragma once
#include "Global.h"

class Thread
{
public:
	Thread(int pThreadId) : stop_(true)
	{
		setThreadId(pThreadId);
	}

	virtual ~Thread()
	{
		stop();
	}

	void start()
	{
		thread_ = std::thread(&Thread::Run, this);
	}

	void join()
	{
		thread_.join();
	}

	void stop()
	{
		stop_ = false;
		thread_.join();
	}
	void suspend()
	{
		std::unique_lock<std::mutex> threadLock_(threadMutex_);
		threadCond_.wait(threadLock_);
	}

	void setThreadId(int pThreadId) 
	{
		threadId_ = pThreadId; 
	}

	int	getThreadId() 
	{
		return threadId_; 
	}

	void resume() 
	{
		threadCond_.notify_one(); 
	}
	
	bool isStop() 
	{
		return stop_; 
	}

protected:
	virtual void Run() = 0;

private:
	std::condition_variable threadCond_;
	std::mutex threadMutex_;
	std::thread	thread_;

	bool stop_;
	int	threadId_;
};

class WorkerThread : public Thread
{
public:
	WorkerThread(boost::asio::io_service* pIoService, int pThreadId) : Thread(pThreadId)
	{
		ioService_ = pIoService;
	}

	virtual void Run();

private:
	boost::asio::io_service* ioService_;
};