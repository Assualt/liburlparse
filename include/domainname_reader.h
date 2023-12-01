#ifndef _DOMAINNAMEREADER_H
#define _DOMAINNAMEREADER_H

#include "input_text_reader.h"
#include "char_utils.h"
#include "url_detect_options.h"
#include "string_builder.h"
#include "url_detector.h"

#include <stdlib.h>
#include <string.h>
class CharacterHandler;
enum ReaderNextState {
    /**
     * Trying to read the domain name caused it to be invalid.
     */
    InvalidDomainName,
    /**
     * The domain name is found to be valid.
     */
    ValidDomainName,
    /**
     * Finished reading, next step should be to read the fragment.
     */
    ReadFragment,
    /**
     * Finished reading, next step should be to read the path.
     */
    ReadPath,
    /**
     * Finished reading, next step should be to read the port.
     */
    ReadPort,
    /**
     * Finished reading, next step should be to read the query string.
     */
    ReadQueryString
};

/**
 * The domain name reader reads input from a InputTextReader and validates if
 * the content being read is a valid domain name. After a domain name is read,
 * the returning status is what to do next. If the domain is valid but a
 * specific character is found, the next state will be to read another part for
 * the rest of the url. For example, if a "?" is found at the end and the domain
 * is valid, the return state will be to read a query string.
 */
class DomainNameReader {
public:
    /**
     * Creates a new instance of the DomainNameReader object.
     * @param reader The input stream to read.
     * @param buffer The string buffer to use for storing a domain name.
     * @param current The current string that was thought to be a domain name.
     * @param options The detector options of this reader.
     * @param characterHandler The handler to call on each non-matching
     * character to count matching quotes and stuff.
     */    
    DomainNameReader(
            InputTextReader &reader,
            StringBuilder &buffer,
            const std::string &current,
            UrlDetectorOptions_T &options);
    
    /**
     * Reads the Dns and returns the next state the state machine should take in
     * throwing this out, or continue processing if this is a valid domain name.
     * @return The next state to take.
     */
    ReaderNextState readDomainName();

    void setBuffer(StringBuilder &builder);

    int getReaderPosition();

    int getBacktracked();

private:
    /**
     * The currently written string buffer.
     */
    StringBuilder _buffer;

    /**
     * The domain name started with a partial domain name found. This is the
     * original string of the domain name only.
     */
    std::string _current;

    /**
     * Detection option of this reader.
     */
    UrlDetectorOptions_T _options;

    /**
     * Keeps track the number of dots that were found in the domain name.
     */
    int _dots;

    /**
     * Keeps track of the number of characters since the last "."
     */
    int _currentLabelLength;

    /**
     * Keeps track of the number of characters in the top level domain.
     */
    int _topLevelLength;

    /**
     * Keeps track where the domain name started. This is non zero if the buffer
     * starts with http://username:password@...
     */
    int _startDomainName;

    /**
     * Keeps track if the entire domain name is numeric.
     */
    bool _numeric;

    /**
     * Keeps track if we are seeing an ipv6 type address.
     */
    bool _seenBracket;

    /**
     * Keeps track if we have seen a full bracket set "[....]"; used for ipv6
     * type address.
     */
    bool _seenCompleteBracketSet;

    /**
     * Keeps track if we have a zone index in the ipv6 address.
     */
    bool _zoneIndex;

    /**
     * Contains the input stream to read.
     */
    InputTextReader _reader;

    /**
     * Contains the handler for each character match.
     */
    CharacterHandler *_characterHandler;

    /**
     * Reads and parses the current string to make sure the domain name started
     * where it was supposed to, and the current domain name is correct.
     * @return The next state to use after reading the current.
     */
    ReaderNextState readCurrent();

    /**
     * Checks the current state of this object and returns if the valid state
     * indicates that the object has a valid domain name. If it does, it will
     * return append the last character and return the validState specified.
     * @param validState The state to return if this check indicates that the
     * dns is ok.
     * @param lastChar The last character to add if the domain is ok.
     * @return The validState if the domain is valid, else
     * ReaderNextState.InvalidDomainName
     */
    ReaderNextState checkDomainNameValid(ReaderNextState validState, char lastChar);

    /**
     * Handles Hexadecimal, octal, decimal, dotted decimal, dotted hex, dotted
     * octal.
     * @param testDomain the string we're testing
     * @return Returns true if it's a valid ipv4 address
     */
    bool isValidIpv4(const std::string &testDomain);

    /**
     * Sees that there's an open "[", and is now checking for ":"'s and stopping
     * when there is a ']' or invalid character. Handles ipv4 formatted ipv6
     * addresses, zone indices, truncated notation.
     * @return Returns true if it is a valid ipv6 address
     */
    bool isValidIpv6(const std::string &testDomain);
};

#endif
