#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "server/Server.hpp"
#include "server/FileHandler.hpp"

int main(int argc, char* argv[]) {
	try {
		if (argc != 3) {
			std::cerr << "Usage: static_server <port> <doc_root>" << std::endl;
			std::cerr << "\tstatic_server 80 ." << std::endl;
			return 1;
		}

		boost::asio::io_service io_service;

		server::Server(io_service, std::atoi(argv[1]), server::FileHandler(argv[2]))();

		boost::asio::signal_set signals(io_service);
		signals.add(SIGINT);
		signals.add(SIGTERM);
#if defined(SIGQUIT)
		signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
		signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));

		std::vector<std::thread> pool;
		for (auto threadCount = std::thread::hardware_concurrency(); threadCount; --threadCount) {
			pool.emplace_back([](auto io_s) { return io_s->run(); }, &io_service);
		}

		for (auto& thread : pool)
			thread.join();

	} catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << std::endl;
	}

	return 0;
}
