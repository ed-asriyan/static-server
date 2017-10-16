//
// Created by Ed Asriyan on 10/10/2017.
//

#include "RequestParser.hpp"
#include <boost/asio/yield.hpp>

#define RETURN_TUPLE return boost::make_tuple
#define RETURN_UNHANDLED RETURN_TUPLE(false, Response::status_type::ok)
#define RETURN_BAD_REQUEST RETURN_TUPLE(true, Response::status_type::bad_request)
#define RETURN_METHOD_NOT_ALLOWED RETURN_TUPLE(true, Response::status_type::method_not_allowed)
#define RETURN_OK RETURN_TUPLE(true, Response::status_type::ok)

inline bool is_char(char c) {
	return c >= 0 && c <= 127;
}

inline bool is_ctl(char c) {
	return (c >= 0 && c <= 31) || (c == 127);
}

inline bool is_special(char c) {
	switch (c) {
		case '(':
		case ')':
		case '<':
		case '>':
		case '@':
		case ',':
		case ';':
		case ':':
		case '\\':
		case '"':
		case '/':
		case '[':
		case ']':
		case '?':
		case '=':
		case '{':
		case '}':
		case ' ':
		case '\t':
			return true;
		default:
			return false;
	}
}

inline bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

inline bool to_lower_compare(char a, char b) {
	return std::tolower(a) == std::tolower(b);
}

bool uri_decode(const std::string& in, std::string& out) {
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i) {
		if (in[i] == '%') {
			if (i + 3 <= in.size()) {
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value) {
					out += static_cast<char>(value);
					i += 2;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else if (in[i] == '+') {
			out += ' ';
		} else {
			out += in[i];
		}
	}
	return true;
}

boost::tuple<bool, server::Response::status_type> server::RequestParser::consume(server::Request& req, char c) {
	reenter (this) {
		req.uri.clear();
		req.http_version_major = 0;
		req.http_version_minor = 0;

		// Parsing the method. We support onlu GET & HEAD.
		if (c == 'G') {
			yield RETURN_UNHANDLED;
			if (c != 'E') RETURN_METHOD_NOT_ALLOWED;
			yield RETURN_UNHANDLED;
			if (c != 'T') RETURN_METHOD_NOT_ALLOWED;
			yield RETURN_UNHANDLED;

			req.method = Request::GET;
		} else if (c == 'H') {
			yield RETURN_UNHANDLED;
			if (c != 'E') RETURN_METHOD_NOT_ALLOWED;
			yield RETURN_UNHANDLED;
			if (c != 'A') RETURN_METHOD_NOT_ALLOWED;
			yield RETURN_UNHANDLED;
			if (c != 'D') RETURN_METHOD_NOT_ALLOWED;
			yield RETURN_UNHANDLED;

			req.method = Request::HEAD;
		} else RETURN_METHOD_NOT_ALLOWED;

		// Parce space after method name.
		if (c != ' ') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// Parce URI.
		source_uri.clear();
		while (!is_ctl(c) && c != ' ') {
			if (c == '?') break;
			source_uri.push_back(c);
			yield RETURN_UNHANDLED;
		}
		if (c == '?') {
			while (!is_ctl(c) && c != ' ') {
				yield RETURN_UNHANDLED;
			}
		}
		if (source_uri.empty()) RETURN_BAD_REQUEST;
		if (!uri_decode(source_uri, req.uri)) RETURN_BAD_REQUEST;
		if (req.uri.empty()) RETURN_BAD_REQUEST;
		while (!is_ctl(c) && c != ' ') {
			yield RETURN_UNHANDLED;
		}

		// Parse space after URI.
		if (c != ' ') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// Parse HTTP protocol identifier.
		if (c != 'H') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;
		if (c != 'T') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;
		if (c != 'T') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;
		if (c != 'P') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// Parce slash-separator between HTTP and its version.
		if (c != '/') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// Parse major HTTP version.
		if (!is_digit(c)) RETURN_BAD_REQUEST;
		while (is_digit(c)) {
			req.http_version_major = req.http_version_major * 10 + c - '0';
			yield RETURN_UNHANDLED;
		}

		// Parse dot between major and minor.
		if (c != '.') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// Parse minor version.
		if (!is_digit(c)) RETURN_BAD_REQUEST;
		while (is_digit(c)) {
			req.http_version_minor = req.http_version_minor * 10 + c - '0';
			yield RETURN_UNHANDLED;
		}

		// Parse CRLF.
		if (c != '\r') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;
		if (c != '\n') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// request headers are not supported
		// request body is not supported

		yield RETURN_OK;
	}

	return true;
}
