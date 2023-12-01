#ifndef _CHARUTILS_H_
#define _CHARUTILS_H_

#include <string.h>
#include <iostream>
#include <list>
#include <vector>

//	Define character Toolkit.
class CharUtils {
public:
    // Check if Character is a valid hex character.
    static bool isHex(char a);

    // Check if character is a valid alphabetic character.
    static bool isAlpha(char a);

    // Checks if character is a valid numeric character.
    static bool isNumberic(char a);

    // Checks if character is a valid alphanumeric character.
    static bool isAlphaNumberic(char a);

    // Checks if character is a valid unreserved character. This is defined by
    // the RFC 3986 ABNF
    static bool isUnreserved(char a);

    // Checks if character is a dot
    static bool isDot(char a);

    // Checks if whiteSpace is or not
    static bool isWhiteSpace(char a);

    /**
     * split a Strubf without the use of a regex,which could split by isDot() or
     * %2e
     * @param input the input string which can be split by dot
     * @return an arry of Strings that is a partition of the original string by dot
     */
    static void splitByDot(std::vector<std::string> &splitList, const std::string &input);

    static bool judgeValidRadixString(const std::string &src, int radix = 10);
};

#endif
