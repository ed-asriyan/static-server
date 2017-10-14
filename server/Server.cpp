//
// Created by Ed Asriyan on 30/09/2017.
//

#include "Server.hpp"
#include <boost/asio/yield.hpp>

server::Server::Server(
	boost::asio::io_service& io_service,
	const std::string& address,
	const std::string& port,
	boost::function<void(const Request&, Response&)> request_handler
) : request_handler(request_handler) {
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
		request.reset(new Request);
		response.reset(new Response);

		do {
			yield socket->async_read_some(boost::asio::buffer(*buffer), *this);

			boost::tie(request_handled, parser_status_type) = request_parser.parse(
				*request,
				buffer->data(),
				buffer->data() + length
			);
		} while (!request_handled);

		if (parser_status_type == Response::status_type::ok) {
			request_handler(*request, *response);
		} else {
			*response = Response::stock_reply(parser_status_type);
		}

		Response::normalize_headers(*response);

		yield boost::asio::async_write(*socket, response->get_header_buffer(), *this);
		if (response->body) {
			do {
				if ((response_sent = response->body.read(buffer->c_array(), sizeof(*buffer)).gcount()) <= 0) break;
				yield socket->async_write_some(boost::asio::buffer(*buffer, static_cast<size_t>(response_sent)), *this);
			} while (true);
			response->body.close();
		}

		socket->shutdown(tcp::socket::shutdown_both, ec);
	}
}
