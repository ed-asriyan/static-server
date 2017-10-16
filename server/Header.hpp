//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_HEADER_H
#define STATIC_SERVER_HEADER_H

#include <string>

namespace server {
	/**
	 * The HTTP header.
	 */
	struct Header {
		std::string name;
		std::string value;
	};
}

#endif //STATIC_SERVER_HEADER_H
