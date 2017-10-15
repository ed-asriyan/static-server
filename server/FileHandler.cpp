//
// Created by Ed Asriyan on 10/10/2017.
//

#include "FileHandler.hpp"

server::FileHandler::FileHandler(const std::string& doc_root)
	: doc_root(doc_root) {
	fill_cache();
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

	auto file = cache.find(request_path);
	if (file != cache.end()) {
		rep.status = Response::ok;
		if (req.method == Request::method::GET) {
			rep.body.first = static_cast<const char*>(std::get<0>(file->second));
			rep.body.second = std::get<1>(file->second);
			rep.headers.resize(2);
		}
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = std::to_string(rep.body.second);
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = std::string(std::get<2>(file->second));
	} else {
		rep = Response::stock_reply(Response::not_found);
	}
}

bool isHidden(const boost::filesystem::path& path) {
	std::string name = path.filename().c_str();
	return name != ".." && name != "."  && name[0] == '.';
}

void server::FileHandler::read_file(const boost::filesystem::path& path, char* out, unsigned long size) {
	std::ifstream stream(path.string());
	if (!stream) return;

	unsigned long read = 0;

	while (size > read){
		stream.read(out + read, FileHandler::BUFFER_SIZE);
		read += FileHandler::BUFFER_SIZE;
	}

	stream.close();
}

void server::FileHandler::fill_cache() {
	const boost::filesystem::recursive_directory_iterator end;
	for (auto it = boost::filesystem::recursive_directory_iterator(doc_root); it != end; ++it) {
		const auto path = it->path();
		if(boost::filesystem::is_directory(path) && isHidden(path)) {
			it.no_push();
			continue;
		}

		if (boost::filesystem::is_regular_file(path) && !isHidden(path)) {
			const auto relative_path = boost::filesystem::relative(path, doc_root);
			const unsigned long size = boost::filesystem::file_size(path);

			char* data = new char[size];
			read_file(path, data, size);

			cache.insert({
				relative_path.c_str(),
				{
					data,
					static_cast<unsigned long>(size),
					server::extension_to_type(relative_path.extension().string())
				}
			});
		}
	}
}
