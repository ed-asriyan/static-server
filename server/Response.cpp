//
// Created by Ed Asriyan on 10/10/2017.
//

#include "Response.hpp"

std::string current_date() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	return std::ctime(&now_time);
}

namespace server {
	namespace status_strings {
		const std::string ok =
			"HTTP/1.0 200 OK\r\n";

		const std::string bad_request =
			"HTTP/1.0 400 Bad Request\r\n";

		const std::string not_found =
			"HTTP/1.0 404 Not Found\r\n";

		const std::string internal_server_error =
			"HTTP/1.0 500 Internal Server Error\r\n";

		boost::asio::const_buffer to_buffer(Response::status_type status) {
			switch (status) {
				case Response::ok:
					return boost::asio::buffer(ok);
				case Response::bad_request:
					return boost::asio::buffer(bad_request);
				case Response::not_found:
					return boost::asio::buffer(not_found);
				default:
					return boost::asio::buffer(internal_server_error);
			}
		}
	}

	namespace stock_replies {
		const char ok[] = "";

		const char bad_request[] = "400 Bad Request";

		const char not_found[] = "404 Not Found";

		const char internal_server_error[] = "500 Internal Server Error";

		std::string to_string(Response::status_type status) {
			switch (status) {
				case Response::ok:
					return ok;
				case Response::bad_request:
					return bad_request;
				case Response::not_found:
					return not_found;
				case Response::internal_server_error:
					return internal_server_error;
				default:
					return internal_server_error;
			}
		}

	}

	namespace misc_strings {
		const char name_value_separator[] = {':', ' '};

		const char crlf[] = {'\r', '\n'};
	}

	std::vector<boost::asio::const_buffer> Response::to_buffers() {
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(status_strings::to_buffer(status));
		for (auto& h : headers) {
			buffers.push_back(boost::asio::buffer(h.name));
			buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
			buffers.push_back(boost::asio::buffer(h.value));
			buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		}
		buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		buffers.push_back(boost::asio::buffer(content));
		return buffers;
	}

	Response Response::stock_reply(Response::status_type status) {
		Response rep;
		rep.status = status;
		rep.content = stock_replies::to_string(status);
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = std::to_string(rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = "text/html";
		return rep;
	}

	void Response::normalize_headers(Response& response) {
		response.headers.push_back({"Server", "static-server-on-boost-coroutines"});
		response.headers.push_back({"Connection", "Close"});
		response.headers.push_back({"Date", current_date()});
	}
}
