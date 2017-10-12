//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_REQUEST_HPP
#define STATIC_SERVER_REQUEST_HPP

#include <string>
#include <vector>

namespace server {
	struct Request {
		std::string method;
		std::string uri;
		int http_version_major;
		int http_version_minor;
	};
}

#endif //STATIC_SERVER_REQUEST_HPP
