#include "charutils.h"
#include <algorithm>
#include <stdlib.h>
using namespace std;

// Check if Character is a valid hex character.
bool CharUtils::isHex(char a) {
    return (a >= '0' && a <= '9') || (a >= 'a' && a <= 'f') ||(a >= 'A' && a <= 'F');
}

// Check if character is a valid alphabetic character.
bool CharUtils::isAlpha(char a) {
    return ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'));
}

// Checks if character is a valid numeric character.
bool CharUtils::isNumberic(char a) {
    return a >= '0' && a <= '9';
}

// Checks if character is a valid alphanumeric character.
bool CharUtils::isAlphaNumberic(char a) {
    return isAlpha(a) || isNumberic(a);
}

// Checks if character is a valid unreserved character. This is defined by the
// RFC 3986 ABNF
bool CharUtils::isUnreserved(char a) {
    return isAlphaNumberic(a) || a == '-' || a == '.' || a == '_' || a == '~';
}

// Checks if character is a dot
bool CharUtils::isDot(char a) { 
    //判断是否为 . 。．｡  (半角和全角的点号和句号
    //此处编译会出现警告
    return (a == '.' || (int)a == 0x3002 || (int)a == 0xFF0E ||(int)a == 0xFF61);
}
bool CharUtils::isWhiteSpace(char a) {
    return (a == '\n' || a == '\t' || a == '\r' || a == ' ');
}
void CharUtils::splitByDot(std::vector<std::string> &splitList,const string &input) {
    StringBuilder<char> section;
    if (input.empty()) {
        return;
    }
    InputTextReader reader(input);
    while (!reader.eof()) {
        char current = reader.read();
        if (isDot(current)) {
            splitList.push_back(section.ToString());
            section.setlength(0);
        } else if (current == '%' && reader.canReadChars(2) && StringUtils::stricmp(reader.peek(2).c_str(), (const char *)"2e")) {
            reader.read();
            reader.read();  // advance past the 2e
            splitList.push_back(section.ToString());
            section.setlength(0);
        } else {
            section.Append(current);  //加到尾部
        }
    }
    splitList.push_back(section.ToString());  //加到尾部
}

//radix should between [2,16)
bool CharUtils::JudgeValidRadixString(const std::string &src, int radix){
    string temp = src;
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    if(radix < 2 || radix> 16)
        return false; //invalid radix
    int SignleraidxMax = radix;
    if(SignleraidxMax < 10){
        for(size_t i = 0; i < src.size(); i++){
            if(!CharUtils::isNumberic(src[i]))
                return false;
            else if(abs((src[i] - '0')) >= SignleraidxMax )
                return false;
        }
        return true;
    }else{
        char c = 'a' + SignleraidxMax - 10;
        for(size_t i = 0; i< src.size(); i++){
            if(CharUtils::isNumberic(src[i]))
                continue;
            else if(src[i]>= 'a' && src[i] < c)
                continue;
            else
                return false;
        }
        return true;
    }
    //never reach here
    return true;
}

