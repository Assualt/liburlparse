#include "urlutil.h"
std::string UrlUtil::decode(const string &url) {
    StringBuilder<char> stringBuilder(url);
    stack<int> nonDecodedPercentIndices;
    size_t i = 0;
    size_t size = stringBuilder.size();
    while (i < (size - 2)) {
        char curr = ' ';
        if(i < stringBuilder.size())
            curr = stringBuilder.charAt(i);
        else
            break;
        if (curr == '%') {
            if (CharUtils::isHex(stringBuilder.charAt(i + 1)) &&
                CharUtils::isHex(stringBuilder.charAt(i + 2))) {
                char buf[2];
                sprintf(buf,
                        "%c",
                        (char)strtol(
                                stringBuilder.substr(i + 1, i + 3).c_str(),
                                NULL,
                                16));
                char decodedChar = buf[0];
                stringBuilder.Delete(
                        i, i + 3);  // delete the % and two hex digits ***1
                stringBuilder.insert(
                        i, decodedChar);  // add decoded character ***1
                if (decodedChar == '%') {
                    i--;  // backtrack one character to check for another
                          // decoding with this %.
                } else if (
                        !nonDecodedPercentIndices.empty() &&
                        CharUtils::isHex(decodedChar) &&
                        CharUtils::isHex(stringBuilder.charAt(i - 1)) &&
                        i - nonDecodedPercentIndices.top() == 2) {
                    // Go back to the last non-decoded percent sign if it's
                    // decodable. We only need to go back if it's of form
                    // %[HEX][HEX]
                    i = nonDecodedPercentIndices.top() -
                            1;  // backtrack to the % sign.
                    nonDecodedPercentIndices.pop();
                } else if (
                        (!nonDecodedPercentIndices.empty()) &&
                        (i == stringBuilder.size() - 2)) {
                    // special case to handle %[HEX][Unknown][end of string]
                    i = nonDecodedPercentIndices.top() -
                            1;  // backtrack to the % sign.
                    nonDecodedPercentIndices.pop();
                }
            } else {
                nonDecodedPercentIndices.push(i);
            }
        }
        i++;
    }
    return stringBuilder.ToString();
}

std::string UrlUtil::removeSpecialSpaces(const string &urlPart) {
    StringBuilder<char> stringBuilder(urlPart);
    int size = (int)stringBuilder.size();
    for (int i = 0; i < size; i++) {
        char curr = stringBuilder.charAt(i);
        if (CharUtils::isWhiteSpace(curr)) {
            stringBuilder.DeleteCharAt(i);  
        }
    }
    return stringBuilder.ToString();
}

std::string UrlUtil::encode(const string &url) {
    StringBuilder<char> encoder;
    for (size_t i = 0; i < url.size(); i++) {
        char chr = url[i];
        char buf[10];
        ubyte chrByte = (ubyte)chr;
        string s;
        memset(buf, 0, sizeof(buf));
        if ((chrByte <= 32 || chrByte >= 127 || chr == '#' || chr == '%')) {
            sprintf(buf, "%%%02X", chr);
            s = buf;
            encoder.Append(s);
        } else {
            sprintf(buf, "%c", chr);
            s = buf;
            encoder.Append(chr);
        }
    }
    return encoder.ToString();
}

std::string UrlUtil::removeExtraDots(const string &host) {
    StringBuilder<char> stringBuilder;
    InputTextReader reader(host);
    while (!reader.eof()) {
        char curr = reader.read();
        stringBuilder.Append(curr);
        if (curr == '.') {
            char possibleDot = curr;
            while (possibleDot == '.' && !reader.eof()) {
                possibleDot = reader.read();
            }
            if (possibleDot != '.') {
                stringBuilder.Append(possibleDot);
            }
        }
    }
    if (stringBuilder.size() > 0 &&
        stringBuilder.charAt(stringBuilder.size() - 1) == '.') {
        stringBuilder.DeleteCharAt(stringBuilder.size() - 1);
    }
    if (stringBuilder.size() > 0 && stringBuilder.charAt(0) == '.') {
        stringBuilder.DeleteCharAt(0);
    }
    return stringBuilder.ToString();
}

