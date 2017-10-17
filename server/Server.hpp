//
// Created by Ed Asriyan on 30/09/2017.
//

#ifndef STATIC_SERVER_SERVER_HPP
#define STATIC_SERVER_SERVER_HPP

#include <sys/sendfile.h>

#include <string>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "RequestParser.hpp"
#include "Response.hpp"

namespace server {
	/**
	 * The top-level coroutine of the HTTP server.
	 */
	class Server : boost::asio::coroutine {
	public:
		/**
		 * Construct the server to listen on the specified TCP port, and serve up incoming connections.
		 *
		 * @param io_service io_service instance
		 * @param port Port to listen
		 * @param request_handler Request handler
		 */
		explicit Server(
			boost::asio::io_service& io_service,
			unsigned short port,
			boost::function<void(const Request&, Response&)> request_handler
		);

		/**
		 * Perform work associated with the server.
		 */
		void operator()(
			boost::system::error_code ec = boost::system::error_code(),
			std::size_t length = 0
		);

	private:
		/// receive and send buffer size
		static constexpr unsigned int BUFFER_SIZE = 8192;

		typedef boost::asio::ip::tcp tcp;

		/// Acceptor used to listen for incoming connections.
		boost::shared_ptr<tcp::acceptor> acceptor;

		/// The current connection from a client.
		boost::shared_ptr<tcp::socket> socket;

		/// Buffer for incoming and outcoming data.
		boost::shared_ptr<boost::array<char, Server::BUFFER_SIZE>> buffer;

		/// The parser for the incoming request.
		RequestParser request_parser;

		/// The incoming request.
		boost::shared_ptr<Request> request;

		/// Status type received from parser (detecting 405).
		Response::status_type parser_status_type;

		// Indicator whether the request is processed.
		bool request_handled;

		/// The outcoming response.
		boost::shared_ptr<Response> response;

		/// The nubmer of bytes sent to the client.
		std::streamsize response_sent;

		// Current file descriptor
		int fd;

		/// The user-supplied handler for all incoming requests.
		boost::function<void(const Request&, Response&)> request_handler;
	};
}

#endif //STATIC_SERVER_SERVER_HPP
