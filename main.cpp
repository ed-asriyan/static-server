#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "server/Server.hpp"
#include "server/FileHandler.hpp"

int main(int argc, char* argv[]) {
	try {
		if (argc != 4) {
			std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    receiver 0.0.0.0 80 .\n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    receiver 0::0 80 .\n";
			return 1;
		}

		boost::asio::io_service io_service;

		server::Server(io_service, argv[1], argv[2], server::FileHandler(argv[3]))();

		boost::asio::signal_set signals(io_service);
		signals.add(SIGINT);
		signals.add(SIGTERM);
#if defined(SIGQUIT)
		signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
		signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));

		io_service.run();
	} catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << std::endl;
	}

	return 0;
}
