//
// Created by Ed Asriyan on 10/10/2017.
//

#ifndef STATIC_SERVER_FILEHANDLER_HPP
#define STATIC_SERVER_FILEHANDLER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "Response.hpp"
#include "Request.hpp"
#include "mime_types.hpp"

namespace server {
	class FileHandler {
	public:
		explicit FileHandler(const std::string& doc_root);

		void operator()(const Request& req, Response& rep);


	private:
		static constexpr unsigned int BUFFER_SIZE = 8192;

		std::string doc_root;

		std::unordered_map<std::string, std::tuple<const char*, unsigned long, const char*>> cache;

		void read_file(const boost::filesystem::path& path, char* out, unsigned long size);
		void fill_cache();
	};
}

#endif //STATIC_SERVER_FILEHANDLER_HPP
