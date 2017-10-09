//
// Created by Ed Asriyan on 30/09/2017.
//

#include "Server.hpp"
#include <boost/asio/yield.hpp>

server::Server::Server(boost::asio::io_service& io_service, const std::string& address, const std::string& port) {
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(address, port);
	acceptor.reset(new tcp::acceptor(io_service, *resolver.resolve(query)));
}

void server::Server::operator()(boost::system::error_code ec, size_t length) {
	if (ec) {
		return;
	}

	reenter (this) {
		do {
			socket.reset(new tcp::socket(acceptor->get_io_service()));
			yield acceptor->async_accept(*socket, *this);
			fork Server(*this)();
		} while (is_parent());

		buffer.reset(new boost::array<char, Server::BUFFER_SIZE>);

		do {
			yield socket->async_read_some(boost::asio::buffer(*buffer), *this);
			yield socket->async_write_some(boost::asio::buffer(*buffer), *this);
		} while (buffer.operator*()[0] != '.');

		yield boost::asio::async_write(*socket, boost::asio::buffer("end\n", 5), *this);
		socket->shutdown(tcp::socket::shutdown_both, ec);
	}
}

