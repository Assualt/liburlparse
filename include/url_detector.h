#ifndef _URLDETECTOR_H
#define _URLDETECTOR_H

#include <iostream>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <cstring>

#include "input_text_reader.h"
#include "string_builder.h"
#include "char_utils.h"
#include "url_detect_options.h"
#include "url_part.h"
#include "string_utils.h"
#include "url_marker.h"
#include "domainname_reader.h"
#include "url.h"

/**
 * The states to use to continue writing or not.
 */
enum ReadEndState {
    /**
     * The current url is valid.
     */
    ValidUrl,
    /**
     * The current url is invalid.
     */
    InvalidUrl
};

/**
 * The response of character matching.
 */
enum CharacterMatch {
    /**
     * The character was not matched.
     */
    CharacterNotMatched,
    /**
     * A character was matched with requires a stop.
     */
    CharacterMatchStop,
    /**
     * The character was matched which is a start of parentheses.
     */
    CharacterMatchStart
};

class CharacterHandler {
public:
    virtual void addCharacter(char chars) = 0;  // 纯虚函数
    virtual ~CharacterHandler();
};

class UrlDetector : public CharacterHandler {
public:
    /**
     * Creates a new UrlDetector object used to find urls inside of text.
     * @param content The content to search inside of.
     * @param options The UrlDetectorOptions to use when detecting the content.
     */
    UrlDetector(const std::string &content, UrlDetectorOptions_T options);

    UrlDetector();

    ~UrlDetector();

    /*set 方法*/
    void setOptions(UrlDetectorOptions_T options);

    void setContent(const std::string &content);

    // virtual ~UrlDetector();
    /**
     * Gets the number of characters that were backtracked while reading the
     * input. This is useful for performance measurement.
     * @return The count of characters that were backtracked while reading.
     */
    int getBacktracked();

    /**
     * Detects the urls and returns a list of detected url strings.
     * @return A list with detected urls.
     */
    const std::list<Url> &detect();

    // CharacterHandler 纯虚函数 需要加上virtual 关键字
    virtual void addCharacter(char chars);

public:  // static const
    /**
     * Valid protocol schemes.
     */
    static std::set<std::string> VALID_SCHEMES;
    /**
     * Contains the string to check for and remove if the scheme is this.
     */
    static std::string HTML_MAILTO;

private:
    /**
     * Stores options for detection.
     */
    UrlDetectorOptions_T _options;

    /**
     * The input stream to read.
     */
    InputTextReader _reader;

    /**
     * Buffer to store temporary urls inside of.
     */
    StringBuilder _buffer;
    /**
     * Has the scheme been found in this iteration?
     */
    bool _hasScheme;

    /**
     * If the first character in the url is a quote, then look for matching
     * quote at the end.
     */
    bool _quoteStart;

    /**
     * If the first character in the url is a single quote, then look for
     * matching quote at the end.
     */
    bool _singleQuoteStart;

    /**
     * If we see a '[', didn't find an ipv6 address, and the bracket option is
     * on, then look for urls inside the brackets.
     */
    bool _dontMatchIpv6;

    /**
     * Stores the found urls.
     */
    std::list<Url> _urlList;
    /**
     * Stores the urls to avoid the same
     */
    std::set<std::string> _urlSet;

    /**
     * Keeps the count of special characters used to match quotes and different
     * types of brackets.
     */
    std::map<char, int> _characterMatch;

    /**
     * Keeps track of certain indices to create a Url object.
     */
    UrlMarker _currentUrlMarker;

    /**
     * The default input reader which looks for specific flags to start
     * detecting the url.
     */
    void readDefault();

    /**
     * We found a ":" and is now trying to read either scheme, username/password
     * @param length first index of the previous part (could be beginning of the
     * buffer, beginning of the username/password, or beginning
     * @return new index of where the domain starts
     */
    int processColon(int length);

    /**
     * Gets the number of times the current character was seen in the document.
     * Only special characters are tracked.
     * @param curr The character to look for.
     * @return The number of times that character was seen
     */
    int getCharacterCount(char curr);

    /**
     * Increments the counter for the characters seen and return if this
     * character matches a special character that might require stopping reading
     * the url.
     * @param curr The character to check.
     * @return The state that this character requires.
     */
    CharacterMatch checkMatchingCharacter(char curr);

    /**
     * Checks if the url is in the format:
     * //google.com/static/js.js
     * @return True if the url is in this format and was matched correctly.
     */
    bool readHtml5Root();

    /**
     * Reads the scheme and allows returns true if the scheme is http(s?):// or
     * ftp(s?)://
     * @return True if the scheme was found, else false.
     */
    bool readScheme();

    /**
     * Reads the input and looks for a username and password.
     * Handles:
     * http://username:password@...
     * @param beginningOfUsername Index of the buffer of where the username
     * began
     * @return True if a valid username and password was found.
     */
    bool readUserPass(int beginningOfUsername);

    /**
     * Try to read the current string as a domain name
     * @param current The current string used.
     * @return Whether the domain is valid or not.
     */
    bool readDomainName(std::string &current);

    /**
     * Reads the fragments which is the part of the url starting with #
     * @return If a valid fragment was read true, else false.
     */
    bool readFragment();

    /**
     * Try to read the query string.
     * @return True if the query string was valid.
     */
    bool readQueryString();

    /**
     * Try to read the port of the url.
     * @return True if a valid port was read.
     */
    bool readPort();

    /**
     * Tries to read the path
     * @return True if the path is valid.
     */
    bool readPath();

    /**
     * The url has been read to here. Remember the url if its valid, and reset
     * state.
     * @param state The state indicating if this url is valid. If its valid it
     * will be added to the list of urls.
     * @return True if the url was valid.
     */
    bool readEnd(ReadEndState state);
};

#endif
