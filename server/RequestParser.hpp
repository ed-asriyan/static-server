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
	class RequestParser : boost::asio::coroutine {
	public:
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
		boost::tuple<bool, Response::status_type> consume(Request& req, char input);

		std::string source_uri;
	};
}

#endif //STATIC_SERVER_REQUESTPARSER_HPP
