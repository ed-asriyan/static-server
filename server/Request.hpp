//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_REQUEST_HPP
#define STATIC_SERVER_REQUEST_HPP

#include <string>
#include <vector>

#include "Header.hpp"

namespace server {
	struct Request {
		std::string method;
		std::string uri;
		int http_version_major;
		int http_version_minor;
		std::vector<Header> headers;
	};
}

#endif //STATIC_SERVER_REQUEST_HPP
