//
// Created by Ed Asriyan on 30/09/2017.
//

#ifndef STATIC_SERVER_SERVER_HPP
#define STATIC_SERVER_SERVER_HPP

#include <string>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "RequestParser.hpp"
#include "Response.hpp"

namespace server {
	class Server : boost::asio::coroutine {
	public:
		explicit Server(
			boost::asio::io_service& io_service,
			unsigned short port,
			boost::function<void(const Request&, Response&)> request_handler
		);

		void operator()(
			boost::system::error_code ec = boost::system::error_code(),
			std::size_t length = 0
		);

	private:
		static constexpr unsigned int BUFFER_SIZE = 8192;

		typedef boost::asio::ip::tcp tcp;

		boost::shared_ptr<tcp::acceptor> acceptor;
		boost::shared_ptr<tcp::socket> socket;
		boost::shared_ptr<boost::array<char, Server::BUFFER_SIZE>> buffer;

		RequestParser request_parser;
		boost::shared_ptr<Request> request;
		Response::status_type parser_status_type;
		bool request_handled;

		boost::shared_ptr<Response> response;

		boost::function<void(const Request&, Response&)> request_handler;

		// temp
		std::streamsize response_sent;
	};
}

#endif //STATIC_SERVER_SERVER_HPP
