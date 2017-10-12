//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_FILEHANDLER_HPP
#define STATIC_SERVER_FILEHANDLER_HPP

#include <string>
#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "Response.hpp"
#include "Request.hpp"
#include "mime_types.hpp"

namespace server {
	class FileHandler {
	public:
		explicit FileHandler(const std::string& doc_root);

		void operator()(const Request& req, Response& rep);

	private:
		std::string doc_root;

		static bool url_decode(const std::string& in, std::string& out);
	};
}

#endif //STATIC_SERVER_FILEHANDLER_HPP
