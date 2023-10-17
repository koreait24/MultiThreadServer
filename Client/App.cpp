#include "iostream"
#include <sdkddkver.h>
#include "stdAfx.h"

#pragma comment(lib, "NetworkLib.lib")

int main()
{
	boost::asio::io_service ioService;

	ClientService* client = new ClientService(ioService, "127.0.0.1", "4416");

	std::thread ioThread(boost::bind(&boost::asio::io_service::run, &ioService));
	std::thread recvThread(boost::bind(&ClientService::packetReceiver, client));
	std::thread sendThread(boost::bind(&ClientService::packetWriter, client));

	ioThread.join();
	recvThread.join();
	sendThread.join();

	return 0;
}