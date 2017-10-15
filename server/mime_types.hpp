//
// Created by Ed Asriyan on 11/10/2017.
//

#ifndef STATIC_SERVER_MIME_TYPES_HPP
#define STATIC_SERVER_MIME_TYPES_HPP

#include <string>

namespace server {
	const char* extension_to_type(const std::string& extension);
}

#endif //STATIC_SERVER_MIME_TYPES_HPP
