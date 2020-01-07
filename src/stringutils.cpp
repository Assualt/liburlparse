#include "stringutils.h"

void StringUtils::split(
        std::list<std::string> &v,
        const std::string &s,
        const std::string &c) {
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}
std::string &StringUtils::toLowerCase(string &src) {
    transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}
std::string &StringUtils::toUpperCase(string &src) {
    transform(src.begin(), src.end(), src.begin(), ::toupper);
    return src;
}
bool StringUtils::startsWith(const string &s, const string &sub) {
    return s.find(sub) == 0 ? true : false;
}

bool StringUtils::endsWith(const string &s, const string &sub) {
    return s.rfind(sub) == (s.size() - sub.size()) ? true : false;
}

std::string &StringUtils::replaceAll(string &old, char oldChar, char newChar) {
    int oldSize = (int)old.size();
    for (int i = 0; i < oldSize; i++) {
        if (old[i] == oldChar) {
            old[i] = newChar;
        }
    }
    return old;
}
std::string &StringUtils::replaceAlls(
        string &old,
        const string &oldStr,
        const string &newStr) {
    std::string::size_type pos = 0;
    std::string::size_type srclen = oldStr.size();
    std::string::size_type dstlen = newStr.size();

    while ((pos = old.find(oldStr, pos)) != std::string::npos) {
        old.replace(pos, srclen, newStr);
        pos += dstlen;
    }
    return old;
}

std::string &StringUtils::trim(std::string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}
std::string &StringUtils::defaultstring(std::string &str) {
    if (str.empty()) {
        str = "";
    }
    return str;
}
bool StringUtils::stricmp(const char *dst, const char *src) {
    int ch1, ch2;
    do {
        if (((ch1 = (unsigned char)(*(dst++))) >= 'A') && (ch1 <= 'Z'))
            ch1 += 0x20;
        if (((ch2 = (unsigned char)(*(src++))) >= 'A') && (ch2 <= 'Z'))
            ch2 += 0x20;
    } while (ch1 && (ch1 == ch2));
    return (ch1 - ch2) == 0 ? true : false;
}
int StringUtils::min(int num1, int num2) {
    return num1 > num2 ? num2 : num1;
}
int StringUtils::max(int num1, int num2) {
    return num1 > num2 ? num1 : num2;
}

void StringUtils::list2vector(vector<string> &result, list<string> &li) {
    list<string>::iterator begin = li.begin();
    list<string>::iterator end = li.end();
    for (list<string>::iterator iter = begin; iter != end; ++iter) {
        result.push_back(*iter);
    }
}
