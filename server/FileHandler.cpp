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

	// if path ends in slash (i.e. is a directory) then add "index.html".
	if (request_path[request_path.size() - 1] == '/') {
		request_path += "index.html";
	}


	auto map_iterator = cache.find(request_path);
	if (map_iterator != cache.end()) {
		rep.status = Response::ok;
		rep.body.first = static_cast<const char*>(std::get<0>(map_iterator->second));
		rep.body.second = std::get<1>(map_iterator->second);
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = std::to_string(rep.body.second);
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = std::string(std::get<2>(map_iterator->second));
	} else {
		rep = Response::stock_reply(Response::not_found);
	}
}
