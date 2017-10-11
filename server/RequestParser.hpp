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

namespace server {
	class RequestParser : boost::asio::coroutine {
	public:
		template<typename InputIterator>
		boost::tuple<boost::tribool, InputIterator> parse(
			Request& req,
			InputIterator begin,
			InputIterator end
		) {
			while (begin != end) {
				boost::tribool result = consume(req, *begin++);
				if (result || !result) {
					return boost::make_tuple(result, begin);
				}
			}
			boost::tribool result = boost::indeterminate;
			return boost::make_tuple(result, begin);
		}

	private:
		boost::tribool consume(Request& req, char input);
	};
}

#endif //STATIC_SERVER_REQUESTPARSER_HPP
