//
// Created by Ed Asriyan on 10/10/2017.
//

#include "RequestParser.hpp"
#include <boost/asio/yield.hpp>

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

boost::tribool server::RequestParser::consume(server::Request& req, char c) {
	reenter (this) {
		req.method.clear();
		req.uri.clear();
		req.http_version_major = 0;
		req.http_version_minor = 0;
		req.headers.clear();

		// request method
		while (is_char(c) && !is_ctl(c) && !is_special(c) && c != ' ') {
			req.method.push_back(c);
			yield return boost::indeterminate;
		}
		if (req.method.empty()) return false;

		// space
		if (c != ' ') return false;
		yield return boost::indeterminate;

		// URI
		while (!is_ctl(c) && c != ' ') {
			req.uri.push_back(c);
			yield return boost::indeterminate;
		}
		if (req.uri.empty()) return false;

		// space
		if (c != ' ') return false;
		yield return boost::indeterminate;

		// HTTP protocol identifier.
		if (c != 'H') return false;
		yield return boost::indeterminate;
		if (c != 'T') return false;
		yield return boost::indeterminate;
		if (c != 'T') return false;
		yield return boost::indeterminate;
		if (c != 'P') return false;
		yield return boost::indeterminate;

		// slash
		if (c != '/') return false;
		yield return boost::indeterminate;

		// major version number
		if (!is_digit(c)) return false;
		while (is_digit(c)) {
			req.http_version_major = req.http_version_major * 10 + c - '0';
			yield return boost::indeterminate;
		}

		// dot
		if (c != '.') return false;
		yield return boost::indeterminate;

		// minor version number
		if (!is_digit(c)) return false;
		while (is_digit(c)) {
			req.http_version_minor = req.http_version_minor * 10 + c - '0';
			yield return boost::indeterminate;
		}

		// CRLF
		if (c != '\r') return false;
		yield return boost::indeterminate;
		if (c != '\n') return false;
		yield return boost::indeterminate;

		// headers
		while ((is_char(c) && !is_ctl(c) && !is_special(c) && c != '\r') || (c == ' ' || c == '\t')) {
			if (c == ' ' || c == '\t') {
				// leading whitespace, must be continuation of previous header's value
				if (req.headers.empty()) return false;
				while (c == ' ' || c == '\t')
					yield return boost::indeterminate;
			} else {
				// start the next header
				req.headers.push_back(Header());

				// header name
				while (is_char(c) && !is_ctl(c) && !is_special(c) && c != ':') {
					req.headers.back().name.push_back(c);
					yield return boost::indeterminate;
				}

				// colon and space separates the header name from the header value
				if (c != ':') return false;
				yield return boost::indeterminate;
				if (c != ' ') return false;
				yield return boost::indeterminate;
			}

			// header value
			while (is_char(c) && !is_ctl(c) && c != '\r') {
				req.headers.back().value.push_back(c);
				yield return boost::indeterminate;
			}

			// CRLF
			if (c != '\r') return false;
			yield return boost::indeterminate;
			if (c != '\n') return false;
			yield return boost::indeterminate;
		}

		// CRLF
		if (c != '\r') return false;
		yield return boost::indeterminate;
		if (c != '\n') return false;

		// request body is not supported
	}

	return true;
}
