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
	/**
	 * The common handler for all incoming requests.
	 */
	class FileHandler {
	public:
		/**
		 * Construct with a directory containing files to be served.
		 * @param doc_root The server root directory.
		 */
		explicit FileHandler(const std::string& doc_root);

		/**
		 * Handle a request and generate a response.
		 * @param req The incoming request.
		 * @param rep The outcoming response.
		 */
		void operator()(const Request& req, Response& rep);

	private:
		std::string doc_root;
	};
}

#endif //STATIC_SERVER_FILEHANDLER_HPP
