//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_REQUESTPARSER_HPP
#define STATIC_SERVER_REQUESTPARSER_HPP

#include <string>

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/asio/coroutine.hpp>
#include <boost/lexical_cast.hpp>

#include "Request.hpp"
#include "Response.hpp"

namespace server {
	/**
	 * Parser for incoming requests.
	 */
	class RequestParser : boost::asio::coroutine {
	public:
		/**
		 * Parse some data. The tuple return value consists of
		 * 1. bool: true when a complete request has been parsed, false when more data is required;
		 * 2. 200 if request has successfuly parsed; otherwise the response status that has already determined on this
		 *    stage.
		 * @param req The incoming request.
		 * @param begin The iterator to the begginning position of the sequence of received bytes.
		 * @param end The iterator to the final position of the sequence of received bytes.
		 * @return See description of this method.
		 */
		template<typename InputIterator>
		boost::tuple<bool, Response::status_type> parse(
			Request& req,
			InputIterator begin,
			InputIterator end
		) {
			while (begin != end) {
				auto result = consume(req, *begin++);
				int  u = 0;
				if (boost::get<0>(result)) {
					return result;
				}
			}
			return boost::make_tuple(false, Response::status_type::ok);
		}

	private:
		/**
		 * Handle the next character of input.
		 * @param req The incoming request.
		 * @param input The currect byte.
		 * @return See RequestParser::parse method description.
		 */
		boost::tuple<bool, Response::status_type> consume(Request& req, char input);

		std::string source_uri;
	};
}

#endif //STATIC_SERVER_REQUESTPARSER_HPP
