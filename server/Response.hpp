//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_RESPONSE_HPP
#define STATIC_SERVER_RESPONSE_HPP

#include <string>
#include <vector>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "Header.hpp"
#include "Response.hpp"

namespace server {
	struct Response {
		enum status_type {
			ok = 200,
			bad_request = 400,
			not_found = 404,
			method_not_allowed = 405,
			internal_server_error = 500,
		} status;

		std::vector<Header> headers;
		std::string content;
		std::vector<boost::asio::const_buffer> to_buffers();

		static Response stock_reply(status_type status);

		static void normalize_headers(Response& response);
	};
}

#endif //STATIC_SERVER_RESPONSE_HPP
