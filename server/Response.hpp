//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_RESPONSE_HPP
#define STATIC_SERVER_RESPONSE_HPP

#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "Header.hpp"
#include "Response.hpp"

namespace server {
	/**
	 * A response to be sent to a client.
	 */
	struct Response {
		/**
		 * The status of the response.
		 */
		enum status_type {
			ok = 200,
			bad_request = 400,
			forbidden = 403,
			not_found = 404,
			method_not_allowed = 405,
			internal_server_error = 500,
		} status;

		/// The headers to be included in the reply.
		std::vector<Header> headers;

		/// The http body.
		FILE* body;

		/**
		 * Convert the headers into a vector of buffers.
		 */
		std::vector<boost::asio::const_buffer> get_header_buffer();

		/**
		 * Get a stock response.
		 * @param status HTTP response status.
		 * @return Stock response.
		 */
		static Response stock_reply(status_type status);

		/**
		 * Include required headers such as date, server and connection to the responce.
		 * @param response The responce to process.
		 */
		static void normalize_headers(Response& response);
	};
}

#endif //STATIC_SERVER_RESPONSE_HPP
