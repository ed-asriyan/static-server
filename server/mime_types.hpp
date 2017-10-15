//
// Created by Ed Asriyan on 11/10/2017.
//

#ifndef STATIC_SERVER_MIME_TYPES_HPP
#define STATIC_SERVER_MIME_TYPES_HPP

#include <string>

namespace server {
	/**
	 * Convert a file extension into a MIME type.
	 * @param extension File extension without a begginning dot.
	 * @return MIME type.
	 */
	std::string extension_to_type(const std::string& extension);
}

#endif //STATIC_SERVER_MIME_TYPES_HPP
