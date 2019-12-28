/*
	Copyright 2018 tsun. All rights reserved.
	Date: 2018/7/27.
*/

#ifndef _CHARUTILS_H_
#define _CHARUTILS_H_

#include <cstring>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>

#include "inputtextreader.h"
#include "stringbuilder.h"
#include "stringutils.h"

using namespace std;
//	Define character Toolkit.
class CharUtils
{
public:
	CharUtils();
	//Check if Character is a valid hex character.
	static bool isHex(char a);
	//Check if character is a valid alphabetic character.
	static bool isAlpha(char a);
	//Checks if character is a valid numeric character.
	static bool isNumberic(char a);
	//Checks if character is a valid alphanumeric character.
	static bool isAlphaNumberic(char a);
	//Checks if character is a valid unreserved character. This is defined by the RFC 3986 ABNF
	static bool isUnreserved(char a);
	//Checks if character is a dot
	static bool isDot(char a);
	static bool isWhiteSpace(char a);
	/*
   	Splite a Strubf without the use of a regex,which could split by isDot() or %2e
   	@param input the input string which can be split by dot
   	@return an arry of Strings that is a patition of the original string by dot
   */
	static void splitByDot(list<string> &splitList, string &input);
};
#endif
