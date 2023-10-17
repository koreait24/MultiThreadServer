#include "Global.h"

void acceptCallback(boost::system::error_code ec, tcp::socket& socket, tcp::acceptor& myAcceptor)
{
	tcp::no_delay						noOption(true);
	socket.set_option(noOption);

	const auto& userManager = UserManager::getInstance();

	for (int idx = 1; idx <= userManager->getMax(); idx++)
	{
		const auto& iter = userManager->getUserByUid(idx);

		if (nullptr == iter)
		{
			userManager->insertUser(idx, new NetworkUser(std::move(socket), idx));

			LOGTRACE("Connecting User (Invite Index : %d)", idx);

			break;
		}

		else if (nullptr != iter && false == userManager->isActive(idx))
		{
			userManager->insertUser(idx, new NetworkUser(std::move(socket), idx));

			LOGTRACE("Connecting User (Invite Index : %d)", idx);

			break;
		}
	}

	myAcceptor.async_accept
	(
		[&myAcceptor](boost::system::error_code ec, tcp::socket socket)
		{
			acceptCallback(ec, socket, myAcceptor);
		}
	);
}

void WorkerThread::Run()
{
	while (true == isStop())
	{
		try
		{
			tcp::acceptor myAcceptor{ *ioService_, tcp::endpoint(tcp::v4(), gPort) };

			myAcceptor.async_accept
			(
				[&myAcceptor, this](boost::system::error_code ec, tcp::socket socket)
				{
					acceptCallback(ec, socket, myAcceptor);
				}
			);

			ioService_->run();
		}
		catch (std::exception& ex)
		{
			LOGCRITICAL("RUN Failed %s", ex.what());
			ioService_->reset();
		}
	}
}