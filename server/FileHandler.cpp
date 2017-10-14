//
// Created by Ed Asriyan on 10/10/2017.
//

#include "FileHandler.hpp"

server::FileHandler::FileHandler(const std::string& doc_root)
	: doc_root(doc_root) {

}

void server::FileHandler::operator()(const server::Request& req, server::Response& rep) {
	switch (req.method) {
		case Request::method::GET:
		case Request::method::HEAD:
			break;
		default:
			rep = Response::stock_reply(Response::method_not_allowed);
			return;
	}

	// copy url to path.
	std::string request_path = req.uri;

	// request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
		|| request_path.find("..") != std::string::npos) {
		rep = Response::stock_reply(Response::bad_request);
		return;
	}

	// if path ends in slash (i.e. is a directory) then add "index.html".
	if (request_path[request_path.size() - 1] == '/') {
		request_path += "index.html";
	}

	// open the file to send back.
	std::string full_path = doc_root + request_path;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is) {
		rep = Response::stock_reply(Response::not_found);
		return;
	}

	// determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
		extension = request_path.substr(last_dot_pos + 1);
	}

	// fill out the reply to be sent to the client.
	rep.status = Response::ok;
	size_t file_size = static_cast<size_t>(is.tellg());

	if (req.method == Request::method::GET) {
		is.seekg(0, std::ios::end);
		file_size = static_cast<size_t>(is.tellg()) - file_size;
		is.seekg(0, std::ios::beg);
		rep.body = std::move(is);
	} else if (req.method == Request::method::HEAD) {
		is.seekg(0, std::ios::end);
		file_size = static_cast<size_t>(is.tellg()) - file_size;
	}
	rep.headers.resize(2);
	rep.headers[0].name = "Content-Length";
	rep.headers[0].value = std::to_string(file_size);
	rep.headers[1].name = "Content-Type";
	rep.headers[1].value = server::extension_to_type(extension);
}