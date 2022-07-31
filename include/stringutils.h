#ifndef _STRINGUTILS_H
#define _STRINGUTILS_H

#include <list>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include "charutils.h"

using namespace std;
class StringUtils {
public:
    static void split(std::vector<std::string> &v, const std::string &s, const std::string &c);
    // static bool trans(const char *pFromCode, const char *pToCode, const char
    // *pInBuf, size_t iInLen, char *pOutBuf, size_t iOutLen );

    static std::string &toLowerCase(std::string &src);
    static std::string &toUpperCase(std::string &src);
    static bool startsWith(const std::string &s, const std::string &sub);
    static bool endsWith(const std::string &s, const std::string &sub);

    static std::string &replaceAll(std::string &old, char oldChar, char newChar);
    static std::string &replaceAlls(std::string &old, const std::string &oldStr, const std::string &newStr);

    static std::string &trim(std::string &s);

    static std::string &defaultstring(std::string &str);

    static bool stricmp(const char *dst, const char *src);

    static int min(int num1, int num2);
    static int max(int num1, int num2);
};
#endif
