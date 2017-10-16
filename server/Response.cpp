//
// Created by Ed Asriyan on 10/10/2017.
//

#include "Response.hpp"

std::string current_date() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	auto result = std::string(std::ctime(&now_time));
	*--result.end() = '\0';
	return result;
}

namespace server {
	namespace status_strings {
		const std::string ok =
			"HTTP/1.0 200 OK\r\n";

		const std::string bad_request =
			"HTTP/1.0 400 Bad Request\r\n";

		const std::string not_found =
			"HTTP/1.0 404 Not Found\r\n";

		const std::string method_not_allowed =
			"HTTP/1.0 405 Method Not Allowed\r\n";

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
				case Response::method_not_allowed:
					return boost::asio::buffer(method_not_allowed);
				default:
					return boost::asio::buffer(internal_server_error);
			}
		}
	}

	namespace misc_strings {
		const char name_value_separator[] = {':', ' '};

		const char crlf[] = {'\r', '\n'};
	}

	std::vector<boost::asio::const_buffer> Response::get_header_buffer() {
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(status_strings::to_buffer(status));
		for (auto& h : headers) {
			buffers.push_back(boost::asio::buffer(h.name));
			buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
			buffers.push_back(boost::asio::buffer(h.value));
			buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		}
		buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		return buffers;
	}

	Response Response::stock_reply(Response::status_type status) {
		Response rep;
		rep.status = status;
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = "0";
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = "text/html";
		return rep;
	}

	void Response::normalize_headers(Response& response) {
		response.headers.push_back({"Server", "static-server-on-boost-coroutines"});
		response.headers.push_back({"Connection", "close"});
		response.headers.push_back({"Date", current_date()});
	}
}
