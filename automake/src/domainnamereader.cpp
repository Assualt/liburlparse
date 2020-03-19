#include "domainnamereader.h"

// DomainNameReader::DomainNameReader(InputTextReader reader,
// StringBuilder buffer, string current, UrlDetectorOptions_T
// options,CharacterHandler *characterHandler)
int DomainNameReader::MAX_TOP_LEVEL_DOMAIN = 22;
int DomainNameReader::MIN_TOP_LEVEL_DOMAIN = 2;
long DomainNameReader::MAX_NUMERIC_DOMAIN_VALUE = 4294967295L;
long DomainNameReader::MIN_NUMERIC_DOMAIN_VALUE = 16843008L;
int DomainNameReader::MIN_IP_PART = 0;
int DomainNameReader::MAX_IP_PART = 255;
int DomainNameReader::INTERNATIONAL_CHAR_START = 192;
int DomainNameReader::MAX_LABEL_LENGTH = 64;
int DomainNameReader::MAX_NUMBER_LABELS = 127;
int DomainNameReader::MAX_DOMAIN_LENGTH = 255;
std::string DomainNameReader::HEX_ENCODED_DOT = "2e";

DomainNameReader::DomainNameReader(InputTextReader &reader,StringBuilder &buffer,const std::string &current,UrlDetectorOptions_T &options) :
        _current(current),_options(options) ,
        _dots(0), _currentLabelLength(0), _topLevelLength(0), _startDomainName(0),
        _numeric(false), _seenBracket(false), _seenCompleteBracketSet(false), _zoneIndex(false) {
    _reader = reader; 
    _characterHandler = NULL;
    StringBuilder::copyOf(buffer,_buffer);
}
ReaderNextState DomainNameReader::readCurrent() {
    if (!_current.empty()) {
        // Handles the case where the string is ".hello"
        if (_current.size() == 1 && CharUtils::isDot(_current[0])) {
            // return ReaderNextState::InvalidDomainName;
            return InvalidDomainName;
        }
        // else if (_current.size() == 3 && _current.equalsIgnoreCase("%" +
        // HEX_ENCODED_DOT))
        else if (
                _current.size() == 3 &&
                StringUtils::stricmp(
                        _current.c_str(), ("%" + HEX_ENCODED_DOT).c_str())) {
            // return ReaderNextState::InvalidDomainName;
            return InvalidDomainName;
        }

        // The location where the domain name started.
        _startDomainName = _buffer.size() - _current.size();

        // flag that the domain is currently all numbers and/or dots.
        _numeric = true;

        // If an invalid char is found, we can just restart the domain from
        // there.
        int newStart = 0;

        std::string currArray = _current;

        int length = currArray.size();

        // hex special case
        bool isAllHexSoFar = length > 2 &&
                (currArray[0] == '0' &&
                 (currArray[1] == 'x' || currArray[1] == 'X'));

        int index = isAllHexSoFar ? 2 : 0;

        bool done = false;
        while (index < length && !done) {
            // get the current character and update length counts.
            char curr = currArray[index];
            _currentLabelLength++;
            _topLevelLength = _currentLabelLength;
            // Is the length of the last part > 64 (plus one since we just
            // incremented)
            if (_currentLabelLength > MAX_LABEL_LENGTH) {
                // return ReaderNextState::InvalidDomainName;
                return InvalidDomainName;
            } else if (CharUtils::isDot(curr)) {
                // found a dot. Increment dot count, and reset last length
                _dots++;
                _currentLabelLength = 0;
            } else if (curr == '[') {
                _seenBracket = true;
                _numeric = false;
            } else if (
                    curr == '%' && (index + 2) < length &&
                    CharUtils::isHex(currArray[index + 1]) &&
                    CharUtils::isHex(currArray[index + 2])) {
                // handle url encoded dot
                if (currArray[index + 1] == '2' &&
                    currArray[index + 2] == 'e') {
                    _dots++;
                    _currentLabelLength = 0;
                } else {
                    _numeric = false;
                }
                index += 2;
            } else if (isAllHexSoFar) {
                // if it's a valid character in the domain that is not numeric
                if (!CharUtils::isHex(curr)) {
                    _numeric = false;
                    isAllHexSoFar = false;
                    index--;  // backtrack to rerun last character knowing it
                              // isn't hex.
                }
            } else if (
                    CharUtils::isAlpha(curr) || curr == '-' ||
                    curr >= INTERNATIONAL_CHAR_START) {
                _numeric = false;
            } else if (
                    !CharUtils::isNumberic(curr) &&
                    !_options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN)) {
                // if its not _numeric and not alphabetical, then restart
                // searching for a domain from this point.
                newStart = index + 1;
                _currentLabelLength = 0;
                _topLevelLength = 0;
                _numeric = true;
                _dots = 0;
                done = true;
            }
            index++;
        }

        // An invalid character for the domain was found somewhere in the
        // current buffer. cut the first part of the domain out. For example:
        // http://asdf%asdf.google.com <- asdf.google.com is still valid, so
        // restart from the %
        if (newStart > 0) {
            // make sure the location is not at the end. Otherwise the thing is
            // just invalid.
            if (newStart < (int)_current.size()) {
                _buffer.replace(0, _buffer.size(), _current.substr(newStart));
                // cut out the previous part, so now the domain name has to be
                // from here.
                _startDomainName = 0;
            }

            // now after cutting if the buffer is just "." newStart > current
            // (last character in current is invalid)
            if (newStart >= (int)_current.size() ||
                !strcmp(_buffer.toString().c_str(), (const char *)".")) {
                // return ReaderNextState::InvalidDomainName;
                return InvalidDomainName;
            }
        }
    } else {
        _startDomainName = _buffer.size();
    }
    // all else is good, return OK
    // return ReaderNextState::ValidDomainName;
    return ValidDomainName;
}

ReaderNextState DomainNameReader::readDomainName() {
    // Read the current, and if its bad, just return.
    if (readCurrent() == InvalidDomainName) {
        // return ReaderNextState::InvalidDomainName;
        return InvalidDomainName;
    }
    // while not done and not end of string keep reading.
    bool done = false;
    while (!done && !_reader.eof()) {
        char curr = _reader.read();
        if (curr == '/') {
            // continue by reading the path
            return checkDomainNameValid(ReadPath, curr);
        } else if (curr == ':' && (!_seenBracket || _seenCompleteBracketSet)) {
            // Don't check for a port if it's in the middle of an ipv6 address
            return checkDomainNameValid(ReadPort, curr);
        } else if (curr == '?') {
            return checkDomainNameValid(ReadQueryString, curr);
        } else if (curr == '#') {
            // continue by reading the fragment
            return checkDomainNameValid(ReadFragment, curr);
        } else if (
                CharUtils::isDot(curr) ||
                (curr == '%' && _reader.canReadChars(2) &&
                 StringUtils::stricmp(
                         _reader.peek(2).c_str(), HEX_ENCODED_DOT.c_str()))) {
            // if the current character is a dot or a urlEncodedDot
            // handles the case: hello..
            if (_currentLabelLength < 1) {
                done = true;
            } else {
                // append the "." to the domain name
                _buffer.append(curr);

                // if it was not a normal dot, then it is url encoded
                // read the next two chars, which are the hex representation
                if (!CharUtils::isDot(curr)) {
                    _buffer.append(_reader.read());
                    _buffer.append(_reader.read());
                }

                // increment the dots only if it's not part of the zone index
                // and reset the last length.
                if (!_zoneIndex) {
                    _dots++;
                    _currentLabelLength = 0;
                }

                // if the length of the last section is longer than or equal to
                // 64, it's too long to be a valid domain
                if (_currentLabelLength >= MAX_LABEL_LENGTH) {
                    // return ReaderNextState::InvalidDomainName;
                    return InvalidDomainName;
                }
            }
        } else if (
                _seenBracket &&
                (CharUtils::isHex(curr) || curr == ':' || curr == '[' ||
                 curr == ']' || curr == '%') &&
                !_seenCompleteBracketSet) {  // if this is an ipv6 address.
            switch (curr) {
            case ':':
                _currentLabelLength = 0;
                break;
            case '[':
                // if we read another '[', we need to restart by re-reading from
                // this bracket instead.
                _reader.goBack();
                return InvalidDomainName;
            case ']':
                _seenCompleteBracketSet = true;  // means that we already have a
                                                 // complete ipv6 address.
                _zoneIndex = false;  // set this back off so that we can keep
                                     // counting dots after ipv6 is over.
                break;
            case '%':  // set flag to subtract subsequent dots because it's part
                       // of the zone index
                _zoneIndex = true;
                break;
            default:
                _currentLabelLength++;
                break;
            }
            _numeric = false;
            _buffer.append(curr);
        } else if (
                CharUtils::isAlphaNumberic(curr) || curr == '-' ||
                curr >= INTERNATIONAL_CHAR_START) {
            // Valid domain name character. Either a-z, A-Z, 0-9, -, or
            // international character
            if (_seenCompleteBracketSet) {
                // covers case of [fe80::]www.google.com
                _reader.goBack();
                done = true;
            } else {
                // if its not numeric, remember that; excluded x/X for hex ip
                // addresses.
                if (curr != 'x' && curr != 'X' &&
                    !CharUtils::isNumberic(curr)) {
                    _numeric = false;
                }
                // append to the states.
                _buffer.append(curr);
                _currentLabelLength++;
                _topLevelLength = _currentLabelLength;
            }
        } else if (curr == '[' && !_seenBracket) {
            _seenBracket = true;
            _numeric = false;
            _buffer.append(curr);
        } else if (curr == '[' && _seenCompleteBracketSet) {  // Case where
                                                              // [::][ ...
            _reader.goBack();
            done = true;
        } else if (
                curr == '%' && _reader.canReadChars(2) &&
                CharUtils::isHex(_reader.peekChar(0)) &&
                CharUtils::isHex(_reader.peekChar(1))) {
            // append to the states.
            _buffer.append(curr);
            _buffer.append(_reader.read());
            _buffer.append(_reader.read());
            _currentLabelLength += 3;
            _topLevelLength = _currentLabelLength;
        } else {
            _characterHandler = new UrlDetector();  //使用到则分配
            // called to increment the count of matching characters
            _characterHandler->addCharacter(curr);
            // _characterHandler.~UrlDetector();
            delete (_characterHandler);  //使用完即释放
            // invalid character, we are done.
            done = true;
        }
    }
    // Check the domain name to make sure its ok.
    return checkDomainNameValid(ValidDomainName, ' ');
}
void DomainNameReader::setBuffer(StringBuilder &builder) {
    std::string temp;
    if(builder.size() <_buffer.size())
        temp = _buffer.substr(builder.size());
    builder.append(temp);
    // for (size_t i = builder.size(); i < _buffer.size(); i++)
    // {
    // 	builder.append(_buffer.charAt(i));
    // }
}
ReaderNextState DomainNameReader::checkDomainNameValid(
        ReaderNextState validState,
        char lastChar) {
    bool valid = false;
    int lastDotLength = _buffer.size() > 3 && StringUtils::stricmp( _buffer.substr(_buffer.size() - 3).c_str(),("%" + HEX_ENCODED_DOT).c_str())? 3: 1;
    int domainLength = _buffer.size() - _startDomainName +
            (_currentLabelLength > 0 ? lastDotLength : 0);
    int dotCount = _dots + (_currentLabelLength > 0 ? 1 : 0);
    if (domainLength >= MAX_DOMAIN_LENGTH || (dotCount > MAX_NUMBER_LABELS)) {
        valid = false;
    } else if (_numeric) {
        std::string temp = _buffer.substr(_startDomainName);
        std::string testDomain = StringUtils::toLowerCase(temp);
        //! ipv4 must be 4 parts
        //! if dotCount > 4 is not valid and has no dotCount
        //! like 1.1.3 or 1.2.3.4.5
        if (dotCount > 4 || dotCount == 2 || dotCount == 3)
            valid = false;
        else
            valid = isValidIpv4(testDomain);
    } else if (_seenBracket) {
        std::string temp = _buffer.substr(_startDomainName);
        std::string testDomain = StringUtils::toLowerCase(temp);
        valid = isValidIpv6(testDomain);
    } else if ((_currentLabelLength > 0 && _dots >= 1) || (_dots >= 2 && _currentLabelLength == 0) || (_options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN) && _dots == 0)) {
        int topStart = _buffer.size() - _topLevelLength;
        if (_currentLabelLength == 0) {
            topStart--;
        }
        topStart = StringUtils::max(topStart, 0);

        // get the first 4 characters of the top level domain
        std::string topLevelStart = _buffer.substr(topStart,topStart + StringUtils::min(4, _buffer.size() - topStart));

        // There is no size restriction if the top level domain is international
        // (starts with "xn--")
        valid = ((StringUtils::stricmp( topLevelStart.c_str(), (const char *)"xn--") || (_topLevelLength >= MIN_TOP_LEVEL_DOMAIN && _topLevelLength <= MAX_TOP_LEVEL_DOMAIN)));
    }
    if (valid) {
        // if it's valid, add the last character (if specified) and return the
        // valid state.
        if (lastChar != ' ') {
            _buffer.append(lastChar);
        }

        return validState;
    }
    // Roll back one char if its invalid to handle: "00:41.<br />"
    // This gets detected as 41.br otherwise.
    _reader.goBack();

    // return invalid state.
    // return ReaderNextState::InvalidDomainName;
    return InvalidDomainName;
}

bool DomainNameReader::isValidIpv4(const std::string &testDomain) {
    bool valid = false;
    std::vector<std::string>::iterator parts_end;
    std::vector<std::string>::iterator parts_start;
    std::vector<std::string>::iterator iter;
    if (testDomain.size() > 0) {
        // handling format without dots. Ex: http://2123123123123/path/a,
        // http://0x8242343/aksdjf
        if (_dots == 0) {
            try {
                long value;
                if (testDomain.size() > 2 && testDomain[0] == '0' &&
                    testDomain[1] == 'x') {  // hex
                    value = strtol(testDomain.substr(2).c_str(), NULL, 16);
                } else if (testDomain[0] == '0') {  // octal
                    value = strtol(testDomain.substr(1).c_str(), NULL, 8);
                } else {  // decimal
                    value = strtol(testDomain.c_str(), NULL, 10);
                }
                valid = (value <= MAX_NUMERIC_DOMAIN_VALUE) &&
                        (value >= MIN_NUMERIC_DOMAIN_VALUE);
            } catch (int) {
                //日志输出
                Log().log().setLevel(LOG_DEBUG_LEVEL).format("This testDomain can't covert to Int.[at FILE:%s FUNC:%s LINE:%d]",__FILE__,__FUNCTION__,__LINE__).toFile();
                valid = false;
            }
        } else if (_dots == 3 || _dots == 4) {
            // Dotted decimal/hex/octal format
            std::vector<std::string> parts;
            CharUtils::splitByDot(parts, testDomain);
            valid = true;
            // check each part of the ip and make sure its valid.
            parts_end = parts.end();
            parts_start = parts.begin();
            for (iter = parts_start; iter != parts_end; ++iter) {
                std::string part = *iter;
                if (part.size() > 0) {
                    std::string parserNum;
                    int base;
                    if (part.size() > 2 && part[0] == '0' && part[1] == 'x') {
                        // dotted hex
                        parserNum = part.substr(2);
                        base = 16;
                    } else if (part[0] == '0') {
                        // dotted octal
                        parserNum = part.substr(1);
                        base = 8;
                    } else {
                        // dotted decimal
                        parserNum = part;
                        base = 10;
                    }
                    int section;
                    if (parserNum.size() == 0) {
                        section = 0;
                    } else {
                        try {
                            section = strtol(parserNum.c_str(), NULL, base);
                        } catch (int) {
                            //日志输出
                            Log().log().setLevel(LOG_DEBUG_LEVEL).format("This testDomain can't covert to Int.[at FILE:%s FUNC:%s LINE:%d]",__FILE__,__FUNCTION__, __LINE__).toFile();
                            return false;
                        }
                    }
                    if ((section < MIN_IP_PART) || (section > MAX_IP_PART)) {
                        valid = false;
                    }
                } else {
                    valid = false;
                }
            }
        } else  //_dots = 1 2 4 others
        {
            valid = false;
        }
    }
    return valid;
}

bool DomainNameReader::isValidIpv6(const std::string &testDomain) {
    // char *domainArrays = new char[testDomain.size()];
    // strcpy(domainArrays,testDomain.c_str());
    std::string domainArray = testDomain;
    // delete(domainArrays);
    if ((domainArray.size() < 3) ||
        (domainArray[domainArray.size() - 1] != ']') ||
        (domainArray[0] != '[') ||
        ((domainArray[1] == ':') && (domainArray[2] != ':'))) {
        return false;
    }
    int numSections = 1;
    int hexDigits = 0;
    char prevChar = 0;

    // used to check ipv4 addresses at the end of ipv6 addresses.
    StringBuilder lastSection;
    bool hexSection = true;

    // If we see a '%'. Example: http://[::ffff:0xC0.0x00.0x02.0xEB%251]
    bool zoneIndiceMode = false;

    // If doubleColonFlag is true, that means we've already seen one "::"; we're
    // not allowed to have more than one.
    bool doubleColonFlag = false;

    for (size_t index = 0; index < domainArray.size(); index++) {
        switch (domainArray[index]) {
        case '[':  // found beginning of ipv6 address
            break;
        case '%':
        case ']':  // found end of ipv6 address
            if (domainArray[index] == '%') {
                // see if there's a urlencoded dot
                if (domainArray.size() - index >= 2 &&
                    domainArray[index + 1] == '2' &&
                    domainArray[index + 2] == 'e') {
                    lastSection.append("%2e");
                    index += 2;
                    hexSection = false;
                    break;
                }
                zoneIndiceMode = true;
            }
            if (!hexSection && (!zoneIndiceMode || domainArray[index] == '%')) {
                std::string temp = lastSection.toString();
                if (isValidIpv4(temp)) {
                    numSections++;  // ipv4 takes up 2 sections.
                } else {
                    return false;
                }
            }
            break;
        case ':':
            if (prevChar == ':') {
                if (doubleColonFlag) {  // only allowed to have one "::" in an
                                        // ipv6 address.
                    return false;
                }
                doubleColonFlag = true;
            }
            // This means that we reached invalid characters in the previous
            // section
            if (!hexSection) {
                return false;
            }

            hexSection = true;  // reset hex to true
            hexDigits = 0;      // reset count for hex digits
            numSections++;
            lastSection.setLength(0); // clear last section
            break;
        default:
            if (zoneIndiceMode) {
                if (!CharUtils::isUnreserved(domainArray[index])) {
                    return false;
                }
            } else {
                lastSection.append(domainArray[index]);  // collect our possible
                                                         // ipv4 address
                if (hexSection && CharUtils::isHex(domainArray[index])) {
                    hexDigits++;
                } else {
                    hexSection = false;  // non hex digit.
                }
            }
            break;
        }
        if (hexDigits > 4 || numSections > 8) {
            return false;
        }
        prevChar = domainArray[index];
    }
    // numSections != 1 checks for things like: [adf]
    // If there are more than 8 sections for the address or there isn't a double
    // colon, then it's invalid.
    return numSections != 1 && (numSections >= 8 || doubleColonFlag);
}
int DomainNameReader::getReaderPosition() {
    return _reader.getPosition();
}

int DomainNameReader::getBacktracked() {
    return _reader.getBacktrackedCount();
}
