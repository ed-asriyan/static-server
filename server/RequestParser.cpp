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

boost::tuple<bool, server::Response::status_type> server::RequestParser::consume(server::Request& req, char c) {
	reenter (this) {
		req.method.clear();
		req.uri.clear();
		req.http_version_major = 0;
		req.http_version_minor = 0;

		// request method
		while (is_char(c) && !is_ctl(c) && !is_special(c) && c != ' ') {
			req.method.push_back(c);
			yield RETURN_UNHANDLED;;
		}
		if (req.method.empty()) RETURN_METHOD_NOT_ALLOWED;

		// space
		if (c != ' ') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// URI
		while (!is_ctl(c) && c != ' ') {
			req.uri.push_back(c);
			yield RETURN_UNHANDLED;
		}
		if (req.uri.empty()) RETURN_BAD_REQUEST;

		// space
		if (c != ' ') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// HTTP protocol identifier.
		if (c != 'H') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;
		if (c != 'T') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;
		if (c != 'T') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;
		if (c != 'P') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// slash
		if (c != '/') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// major version number
		if (!is_digit(c)) RETURN_BAD_REQUEST;
		while (is_digit(c)) {
			req.http_version_major = req.http_version_major * 10 + c - '0';
			yield RETURN_UNHANDLED;
		}

		// dot
		if (c != '.') RETURN_BAD_REQUEST;
		yield RETURN_UNHANDLED;

		// minor version number
		if (!is_digit(c)) RETURN_BAD_REQUEST;
		while (is_digit(c)) {
			req.http_version_minor = req.http_version_minor * 10 + c - '0';
			yield RETURN_UNHANDLED;
		}

		// CRLF
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
