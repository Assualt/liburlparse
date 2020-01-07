#include "inputtextreader.h"
/**
 * Creates a new instance of the InputTextReader using the content to read.
 * @param content The content to read.
 */
InputTextReader::InputTextReader() :
        _index(0),
        _backtracked(0),
        MINIMUM_BACKTRACK_LENGTH(20),
        MAX_BACKTRACK_MULTIPLIER(10) {}
InputTextReader::InputTextReader(string &content) :
        _content(content),
        _index(0),
        _backtracked(0),
        MINIMUM_BACKTRACK_LENGTH(20),
        MAX_BACKTRACK_MULTIPLIER(10) {}
/**
 * Reads a single char from the content stream and increments the index.
 * @return The next available character.
 */
char InputTextReader::read() {
    char chr = _content[_index++];
    return CharUtils::isWhiteSpace(chr) ? ' ' : chr;
}
/**
 * Peeks at the next number of chars and returns as a string without
 * incrementing the current index.
 * @param numberChars The number of chars to peek.
 */
string InputTextReader::peek(int numberChars) {
    return string(_content, _index, numberChars);
}
/**
 * Gets the character in the array offset by the current index.
 * @param offset The number of characters to offset.
 * @return The character at the location of the index plus the provided offset.
 */
char InputTextReader::peekChar(int offset) {
    if (!canReadChars(offset)) {
        // throw std::overflow_error();
        Log().log()
                .setLevel(LOG_WARN_LEVEL)
                .format("Array is out of index. [at FILE:%s FUNC:%s LINE:%d]",
                        __FILE__,
                        __FUNCTION__,
                        __LINE__)
                .toFile();
    }
    return _content[_index + offset];
}
/**
 * Returns true if the reader has more the specified number of chars.
 * @param numberChars The number of chars to see if we can read.
 * @return True if we can read this number of chars, else false.
 */
bool InputTextReader::canReadChars(int numberChars) {
    return (int)_content.size() >= _index + numberChars;
}
/**
 * Checks if the current stream is at the end.
 * @return True if the stream is at the end and no more can be read.
 */
bool InputTextReader::eof() {
    return (int)_content.size() <= _index;
}
/**
 * Gets the current position in the stream.
 * @return The index to the current position.
 */
int InputTextReader::getPosition() {
    return _index;
}
/**
 * Gets the total number of characters that were backtracked when reading.
 */
int InputTextReader::getBacktrackedCount() {
    return _backtracked;
}
void InputTextReader::setBacktrackedCount(int value) {
    _backtracked = value;
}
/**
 * Moves the index to the specified position.
 * @param position The position to set the index to.
 */
void InputTextReader::seek(int position) {
    int backtrackLength = max(_index - position, 0);
    _backtracked += backtrackLength;
    _index = position;
    checkBacktrackLoop(backtrackLength);
}

/**
 * Goes back a single character.
 */
void InputTextReader::goBack() {
    _backtracked++;
    _index--;
    checkBacktrackLoop(1);
}
void InputTextReader::checkBacktrackLoop(int backtrackLength) {
    if (_backtracked > ((int)_content.size() * MAX_BACKTRACK_MULTIPLIER)) {
        if (backtrackLength < MINIMUM_BACKTRACK_LENGTH) {
            backtrackLength = MINIMUM_BACKTRACK_LENGTH;
        }
        int start = max(_index, 0);
        if (start + backtrackLength > (int)_content.size()) {
            backtrackLength = (int)_content.size() - start;
        }
        string badText = string(_content, start, backtrackLength);
        // throw new NegativeArraySizeException("Backtracked max amount of
        // characters. Endless loop detected. Bad Text: '"+ badText + "'");
        // Log().log().setLevel(LOG_ERR_LEVEL).format("Backtracked max amount of
        // characters. Endless loop detected. Bad Text: %s. [at FILE:%s FUNC:%s
        // LINE:%d]",badText, __FILE__, __FUNCTION__, __LINE__).toFile();
        Log().log()
                .setLevel(LOG_ERR_LEVEL)
                .format("Backtracked max amount of characters. Endless loop "
                        "detected. Bad Text: %s  [at FILE:%s FUNC:%s LINE:%d]",
                        badText.c_str(),
                        __FILE__,
                        __FUNCTION__,
                        __LINE__)
                .toFile();
    }
}
void InputTextReader::setPosition(int index) {
    _index = index;
}
