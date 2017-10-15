//
// Created by Ed Asriyan on 30/09/2017.
//

#include "Server.hpp"
#include <boost/asio/yield.hpp>

server::Server::Server(
	boost::asio::io_service& io_service,
	unsigned short port,
	boost::function<void(const Request&, Response&)> request_handler
) : request_handler(request_handler) {
	acceptor.reset(new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), port)));
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
		if (response->body.first) {
			response_sent = 0;
			do {
				if (response_sent >= response->body.second) break;
				yield socket->async_write_some(
					boost::asio::buffer(
						response->body.first + response_sent,
						static_cast<size_t>(BUFFER_SIZE > response->body.second - response_sent ? response->body.second - response_sent : BUFFER_SIZE)
					),
					*this
				);
				response_sent += Server::BUFFER_SIZE;
			} while (true);
		}

		socket->shutdown(tcp::socket::shutdown_both, ec);
	}
}
