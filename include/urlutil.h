#ifndef _URLUTIL_H
#define _URLUTIL_H

#include "charutils.h"
#include "inputtextreader.h"
#include "stringbuilder.h"

#include <stack>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#define ubyte unsigned char

class UrlUtil
{
private:
	UrlUtil();

public:
	/**
	* Decodes the url by iteratively removing hex characters with backtracking.
	* For example: %2525252525252525 becomes %
	*/
	static std::string decode(string &url);
	/**
	* Removes TAB (0x09), CR (0x0d), and LF (0x0a) from the URL
	* @param urlPart The part of the url we are canonicalizing
	*/
	static std::string removeSpecialSpaces(string &urlPart);
	/**
	* Replaces all special characters in the url with hex strings.
	*/
	static std::string encode(string &url);
	/**
	* Removes all leading and trailing dots; replaces consecutive dots with a single dot
	* Ex: ".lalal.....com." -> "lalal.com"
	*/
	static std::string removeExtraDots(string &host);
};
#endif
