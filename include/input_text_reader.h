#ifndef _INPUTTEXTREADER_H
#define _INPUTTEXTREADER_H

#include <string>

class InputTextReader {
public:

    InputTextReader();
    
    /**
     * Creates a new instance of the InputTextReader using the content to read.
     * @param content The content to read.
     */
    InputTextReader(const std::string &content);
    
    /**
     * Reads a single char from the content stream and increments the index.
     * @return The next available character.
     */
    char read();
    
    /**
     * Peeks at the next number of chars and returns as a string without
     * incrementing the current index.
     * @param numberChars The number of chars to peek.
     */
    std::string peek(int numberChars);
    
    /**
     * Gets the character in the array offset by the current index.
     * @param offset The number of characters to offset.
     * @return The character at the location of the index plus the provided
     * offset.
     */
    char peekChar(int offset);
    
    /**
     * Returns true if the reader has more the specified number of chars.
     * @param numberChars The number of chars to see if we can read.
     * @return True if we can read this number of chars, else false.
     */
    bool canReadChars(int numberChars);
    
    /**
     * Checks if the current stream is at the end.
     * @return True if the stream is at the end and no more can be read.
     */
    bool eof();
    
    /**
     * Gets the current position in the stream.
     * @return The index to the current position.
     */
    int getPosition();
    
    void setPosition(int index);
    
    /**
     * Gets the total number of characters that were backtracked when reading.
     */
    int getBacktrackedCount();
    
    void setBacktrackedCount(int value);
    
    /**
     * Moves the index to the specified position.
     * @param position The position to set the index to.
     */
    void seek(int position);
    
    /**
     * Goes back a single character.
     */
    void goBack();

private:
    /**
     * The content to read.
     */
    std::string _content;
    
    /**
     * The current position in the content we are looking at.
     */
    int _index;
    
    /**
     * Contains the amount of characters that were backtracked. This is used for
     * performance analysis.
     */
    int _backtracked;

    void checkBacktrackLoop(int backtrackLength);
};
#endif
