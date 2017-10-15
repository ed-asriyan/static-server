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
			// Creating socket and accepting new connection.
			socket.reset(new tcp::socket(acceptor->get_io_service()));
			yield acceptor->async_accept(*socket, *this);

			// Fork the coroutine.
			fork Server(*this)();
		} while (is_parent()); // If coroutine is parent continue accepting new connections.

		// Create objects needed to receive a request on the connection.
		buffer.reset(new boost::array<char, Server::BUFFER_SIZE>);
		request.reset(new Request);
		response.reset(new Response);

		// Receive incoming data. Parse it by parts with size of the buffer.
		do {
			// Receive part of request.
			yield socket->async_read_some(boost::asio::buffer(*buffer), *this);

			// Parse the part of incoming request.
			boost::tie(request_handled, parser_status_type) = request_parser.parse(
				*request,
				buffer->data(),
				buffer->data() + length
			);
		} while (!request_handled);

		// If parse completed send request to request handler
		if (parser_status_type == Response::status_type::ok) {
			request_handler(*request, *response);
		} else { // Otherwise send response generated by request parser
			*response = Response::stock_reply(parser_status_type);
		}

		// Add headers, wich is neccessary for all responses.
		Response::normalize_headers(*response);

		// Send the headers.
		yield boost::asio::async_write(*socket, response->get_header_buffer(), *this);

		// Send the body if it exists. Send it by parts with size of the buffer.
		if (response->body) {
			do {
				if ((response_sent = response->body.read(buffer->c_array(), sizeof(*buffer)).gcount()) <= 0) break;
				yield socket->async_write_some(boost::asio::buffer(*buffer, static_cast<size_t>(response_sent)), *this);
			} while (true);
			// Close the file stream.
			response->body.close();
		}

		// Close the connection.
		socket->shutdown(tcp::socket::shutdown_both, ec);
	}
}
