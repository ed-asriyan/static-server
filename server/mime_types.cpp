//
// Created by Ed Asriyan on 11/10/2017.
//

#include "mime_types.hpp"

#define DEFAULT_TYPE "text/plain";

struct mapping {
	const char* extension;
	const char* mime_type;
} mappings[] = {
	{".html", "text/html"},
	{".css", "text/css"},
	{".js", "application/javascript"},
	{".jpg", "image/jpeg"},
	{".jpeg", "image/jpeg"},
	{".png", "image/png"},
	{".gif", "image/gif"},
	{".swf", "application/x-shockwave-flash"},
	{0, 0}
};

const char* server::extension_to_type(const std::string& extension) {
	for (mapping* m = mappings; m->extension; ++m) {
		if (m->extension == extension) {
			return m->mime_type;
		}
	}

	return DEFAULT_TYPE;
}
