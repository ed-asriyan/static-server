//
// Created by Ed Asriyan on 10/10/2017.
//

#include "FileHandler.hpp"

server::FileHandler::FileHandler(const std::string& doc_root)
	: doc_root(doc_root) {

}

void server::FileHandler::operator()(const server::Request& req, server::Response& rep) {
	switch (req.method) {
		case Request::GET:
		case Request::HEAD:
			break;
		default:
			rep = Response::stock_reply(Response::method_not_allowed);
			return;
	}

	// Copy url to path.
	std::string request_path = req.uri;

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
		|| request_path.find("/../") != std::string::npos) {
		rep = Response::stock_reply(Response::bad_request);
		return;
	}

	// If path ends in slash (i.e. is a directory) then add "index.html".
	bool is_index;
	if ((is_index = request_path[request_path.size() - 1] == '/')) {
		request_path += "index.html";
	}

	// Open the file to send back.
	std::string full_path = doc_root + request_path;
	FILE* is = std::fopen(full_path.c_str(), "rb");
	if (!is) {
		if (is_index) {
			rep = Response::stock_reply(Response::forbidden);
		} else {
			rep = Response::stock_reply(Response::not_found);
		}
		return;
	}

	// Determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
		extension = request_path.substr(last_dot_pos + 1);
	}

	// Fill out the reply to be sent to the client.
	rep.status = Response::ok;
	size_t file_size = static_cast<size_t>(ftell(is));

	if (req.method == Request::GET) {
		std::fseek(is, 0, SEEK_END);
		file_size = static_cast<size_t>(ftell(is)) - file_size;
		fseek(is, 0, SEEK_SET);
		rep.body = std::move(is);
	} else if (req.method == Request::HEAD) {
		fseek(is, 0, SEEK_END);
		file_size = static_cast<size_t>(ftell(is)) - file_size;
	}

	rep.headers.resize(2);
	rep.headers[0].name = "Content-Length";
	rep.headers[0].value = std::to_string(file_size);
	rep.headers[1].name = "Content-Type";
	rep.headers[1].value = server::extension_to_type(extension);
}
