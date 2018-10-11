#include "urldetector.h"
#include "time.h"
UrlDetector::UrlDetector(string &content, UrlDetectorOptions_T options):HTML_MAILTO("mailto:"),
VALID_SCHEMES{"http://","https://","ftp://","ftps://","http%3a//","https%3a//","ftp%3a//"},
_options(options),_reader(InputTextReader(content)),_hasScheme(false),_quoteStart(false),_singleQuoteStart(false),_dontMatchIpv6(false)
{
	
	// HTML_MAILTO = "mailto:";
	// VALID_SCHEMES = {"http://","https://","ftp://","ftps://","http%3a//","https%3a//","ftp%3a//"};
	//_options = options;
	//_reader = InputTextReader(content);
	
}
UrlDetector::UrlDetector():HTML_MAILTO("mailto:"),VALID_SCHEMES{"http://","https://","ftp://","ftps://","http%3a//","https%3a//","ftp%3a//"},
_hasScheme(false),_quoteStart(false),_singleQuoteStart(false),_dontMatchIpv6(false)
{
}
void UrlDetector::setOptions(UrlDetectorOptions_T options)
{
	_options = options;
}
void UrlDetector::setContent(string &content)
{
	_reader = InputTextReader(content);
}
void UrlDetector::addCharacter(char chars)
{
    checkMatchingCharacter(chars);
}
int UrlDetector::getBacktracked() 
{
	return _reader.getBacktrackedCount();
}

const list<Url>& UrlDetector::detect()
{
	clock_t start, end;
	Log().log().setLevel(LOG_DEBUG_LEVEL).format("URL Detector begin. [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
	start = clock();
	readDefault();
	end = clock();
	Log().log().setLevel(LOG_DEBUG_LEVEL).format("URL Detector end.Cost time:%f s. [at FILE:%s FUNC:%s LINE:%d]",(double)(end-start)/CLOCKS_PER_SEC , __FILE__, __FUNCTION__, __LINE__).toFile();
	return _urlList;
}
CharacterHandler::~CharacterHandler(){}
UrlDetector::~UrlDetector(){}
void UrlDetector::readDefault()
{
	//Keeps track of the number of characters read to be able to later cut out the domain name.
	int length = 0;
	string temp;
	//until end of string read the contents
	while (!_reader.eof()) 
	{
		//read the next char to process.
		char curr = _reader.read();
		//std::cout << "readDefault:" << _buffer.ToString() << std::endl;
		switch (curr)
		{
		case ' ':
			//space was found, check if it's a valid single level domain.
			if (_options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN) && _buffer.size() > 0 && _hasScheme)
			{
				_reader.goBack();
				temp = _buffer.substr(length);
				readDomainName(temp);
			}
			_buffer.Append(curr);
			readEnd(InvalidUrl);
			length = 0;
			break;
		case ';':
			//; was found, check if it's a valid single level domain.
			if (_options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN) && _buffer.size() > 0 && _hasScheme)
			{
				_reader.goBack();
				temp = _buffer.substr(length);
				readDomainName(temp);
			}
			_buffer.Append(curr);
			readEnd(InvalidUrl);
			length = 0;
			break;
		case ',':
			//, was found, check if it's a valid single level domain.
			if (_options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN) && _buffer.size() > 0 && _hasScheme)
			{
				_reader.goBack();
				temp = _buffer.substr(length);
				readDomainName(temp);
			}
			_buffer.Append(curr);
			readEnd(InvalidUrl);
			length = 0;
			break;
		case '%':
			if (_reader.canReadChars(2)) {
				if(StringUtils::stricmp(_reader.peek(2).c_str(),(const char *)"3a"))
				{
					_buffer.Append(curr);
					_buffer.Append(_reader.read());
					_buffer.Append(_reader.read());
					length = processColon(length);
				}
				else if (CharUtils::isHex(_reader.peekChar(0)) && CharUtils::isHex(_reader.peekChar(1))) 
				{
					_buffer.Append(curr);
					_buffer.Append(_reader.read());
					_buffer.Append(_reader.read());
					temp = _buffer.substr(length);
					readDomainName(temp);
					length = 0;
				}
			}
			break;
			/*
		case 0x3002: //non-standard dots 
				break;
		case 0xFF0E:
				break;
		case 0xFF61:
				break;
				*/
		case '.': //"." was found, read the domain name using the start from length.
			_buffer.Append(curr);
			temp = _buffer.substr(length);
			//std::cout<<"temp: . "<<temp<<std::endl;
			readDomainName(temp);

			length = 0;
			break;
		case '@': //Check the domain name after a username
			if (_buffer.size() > 0) 
			{
				_currentUrlMarker.setIndex(URL_USERNAME_PASSWORD, length);
				_buffer.Append(curr);
				string temp;
				readDomainName(temp);
				length = 0;
			}
			break;
		case '[':
			if (_dontMatchIpv6) 
			{
				//Check if we need to match characters. If we match characters and this is a start or stop of range,
				//either way reset the world and start processing again.
				if (checkMatchingCharacter(curr) != CharacterNotMatched) 
				{
					//readEnd(ReadEndState::InvalidUrl);
					readEnd(InvalidUrl);
					length = 0;
				}
			}
			int beginning;
			beginning = _reader.getPosition();

			//if it doesn't have a scheme, clear the buffer.
			if (!_hasScheme) 
			{
				_buffer.Delete(0, _buffer.size()); 
			}
			_buffer.Append(curr);
			temp = _buffer.substr(length);
			if (!readDomainName(temp)) 
			{
				//if we didn't find an ipv6 address, then check inside the brackets for urls
				_reader.seek(beginning);
				_dontMatchIpv6 = true;
			}
			length = 0;
			break;
		case '/':
			// "/" was found, then we either read a scheme, or if we already read a scheme, then
			// we are reading a url in the format http://123123123/asdf

			if (_hasScheme || (_options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN) && _buffer.size() > 1))
			{
				//we already have the scheme, so then we already read:
				//http://something/ <- if something is all numeric then its a valid url.
				//OR we are searching for single level domains. We have buffer length > 1 condition
				//to weed out infinite backtrack in cases of html5 roots

				//unread this "/" and continue to check the domain name starting from the beginning of the domain
				_reader.goBack();
				temp = _buffer.substr(length);
				readDomainName(temp);
				length = 0;
			}
			else 
			{
				//we don't have a scheme already, then clear state, then check for html5 root such as: "//google.com/"
				// remember the state of the quote when clearing state just in case its "//google.com" so its not cleared.
				//readEnd(ReadEndState::InvalidUrl);
				readEnd(InvalidUrl);
				_buffer.Append(curr);
				_hasScheme = readHtml5Root();
				length = _buffer.size();
			}
			break;
		case ':':
			//add the ":" to the url and check for scheme/username
			_buffer.Append(curr);
			length = processColon(length);
			//std::cout << "len:" << length << std::endl;
			break;
		default:
			//Check if we need to match characters. If we match characters and this is a start or stop of range,
			//either way reset the world and start processing again.
			if (checkMatchingCharacter(curr) != CharacterNotMatched) 
			{
				//readEnd(ReadEndState::InvalidUrl);
				readEnd(InvalidUrl);
				length = 0;
			}
			else 
			{
				_buffer.Append(curr);
			}
			break;
		}
	}
	if (_options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN) && _buffer.size() > 0 && _hasScheme)
	{
		string str = _buffer.substr(length);
		readDomainName(str);
	}
}

int UrlDetector::processColon(int length)
{
	//std::cout << "_hasScheme:"<<_hasScheme << std::endl;
	if (_hasScheme) 
	{
		//read it as username/password if it has scheme
		if (!readUserPass(length) && _buffer.size() > 0) {
            //unread the ":" so that the domain reader can process it
            _reader.goBack();
            _buffer.Delete(_buffer.size() - 1, _buffer.size());// 
            int backtrackOnFail = _reader.getPosition() - _buffer.size() + length;
			string temp = _buffer.substr(length);
            if (!readDomainName(temp)) {
                //go back to length location and restart search
                _reader.seek(backtrackOnFail);
                readEnd(InvalidUrl);
            }
            length = 0;
        }
	}
    else if (readScheme() && _buffer.size() > 0)
    {
        _hasScheme = true;
        length = _buffer.size(); //set length to be right after the scheme
    }
    else if (_buffer.size() > 0 && _options.hasFlag(ALLOW_SINGLE_LEVEL_DOMAIN)
             && _reader.canReadChars(1))
    { //takes care of case like hi:
        _reader.goBack(); //unread the ":" so readDomainName can take care of the port
        _buffer.Delete(_buffer.size() - 1, _buffer.size()); //***1
		string str = _buffer.ToString();
        readDomainName(str);
    }
    else
    {
		//std::cout << __FUNCTION__ << " readEnd" << std::endl;
        readEnd(InvalidUrl);
        length = 0;
    }
    return length;
}

int UrlDetector::getCharacterCount(char curr)
{
	int count = 0;
	if (_characterMatch.find(curr) != _characterMatch.end())
	{
		count = _characterMatch[curr];
		count = (count < 0 ? 0 : count);
	}
	return count;
}
CharacterMatch UrlDetector::checkMatchingCharacter(char curr)
{
	//This is a quote and we are matching quotes.
	if ((curr == '\"' && _options.hasFlag(QUOTE_MATCH)) || (curr == '\'' && _options.hasFlag(SINGLE_QUOTE_MATCH)))
	{
		bool quoteStart;
		if (curr == '\"') 
		{
			quoteStart = _quoteStart;
			//remember that a double quote was found.
			_quoteStart = true;
		}
		else
		{
			quoteStart = _singleQuoteStart;
			//remember that a single quote was found.
			_singleQuoteStart = true;
		}

		//increment the number of quotes found.
		int currVal ;
		currVal = getCharacterCount(curr) + 1;
		_characterMatch.insert(make_pair(curr, currVal));

		//if there was already a quote found, or the number of quotes is even, return that we have to stop, else its a start.
		return quoteStart || currVal % 2 == 0 ? CharacterMatchStop : CharacterMatchStart;
	}
	else if (_options.hasFlag( BRACKET_MATCH) && (curr == '[' || curr == '{' || curr == '('))
	{
		//Look for start of bracket
		_characterMatch.insert(make_pair(curr, getCharacterCount(curr) + 1));
		return CharacterMatchStart;
	}
	else if (_options.hasFlag(XML) && (curr == '<'))
	{
		//If its html, look for "<"
		_characterMatch.insert(make_pair(curr, getCharacterCount(curr) + 1));
		return  CharacterMatchStart;
	}
	else if ((_options.hasFlag(BRACKET_MATCH) && (curr == ']' || curr == '}' || curr == ')'))
		|| (_options.hasFlag(XML) && (curr == '>')))
	{

		//If we catch a end bracket increment its count and get rid of not ipv6 flag
		int currVal = getCharacterCount(curr) + 1;
		_characterMatch.insert(make_pair(curr, getCharacterCount(curr) + 1));
		//now figure out what the start bracket was associated with the closed bracket.
		char match = '\0';
		switch (curr) {
		case ']':
			match = '[';
			break;
		case '}':
			match = '{';
			break;
		case ')':
			match = '(';
			break;
		case '>':
			match = '<';
			break;
		default:
			break;
		}
		//If the number of open is greater then the number of closed, return a stop.
		return getCharacterCount(match) > currVal ?  CharacterMatchStop : CharacterMatchStart;
	}
	//Nothing else was found.
	return  CharacterNotMatched;
}

bool UrlDetector::readHtml5Root()
{
	//end of input then go away.
	if (_reader.eof()) 
	{
		return false;
	}

	//read the next character. If its // then return true.
	char curr = _reader.read();
	if (curr == '/') 
	{
		_buffer.Append(curr);
		return true;
	}
	else 
	{
		//if its not //, then go back and reset by 1 character.
		_reader.goBack();
		//readEnd(ReadEndState::InvalidUrl);
		readEnd(InvalidUrl);
	}
	return false;
}

bool UrlDetector::readScheme()
{
	//Check if we are checking html and the length is longer than mailto:
	if(_options.hasFlag(HTML) && _buffer.size() >= HTML_MAILTO.size())
	{
		//Check if the string is actually mailto: then just return nothing.
		if(StringUtils::stricmp(HTML_MAILTO.c_str(), _buffer.substr(_buffer.size() - HTML_MAILTO.size()).c_str()))
		{
			//return readEnd(ReadEndState::InvalidUrl);
			return readEnd(InvalidUrl);
		}
	}
	int originalLength = _buffer.size();
	int numSlashes = 0;

	while (!_reader.eof()) 
	{
		char curr = _reader.read();
		//std::cout << __FUNCTION__ << " curr:" << curr << std::endl;
		//if we match a slash, look for a second one.
		if (curr == '/') 
		{
			_buffer.Append(curr);
			if (numSlashes == 1)
			{
				//return only if its an approved protocol. This can be expanded to allow others
				string toString = _buffer.ToString();
				string temp1 =  StringUtils::toLowerCase(toString);
				if (VALID_SCHEMES.count(temp1)) 
				{
					//_currentUrlMarker.setIndex(UrlPart::SCHEME, 0);
					_currentUrlMarker.setIndex(URL_SCHEME, 0);
					return true;
				}
				return false;
			}
			numSlashes++;
		}
		else if (curr == ' ' || checkMatchingCharacter(curr) != CharacterNotMatched )
		{
			//if we find a space or end of input, then nothing found.
			_buffer.Append(curr);
			return false;
		}
		else if (curr == '[') { //if we're starting to see an ipv6 address
			_reader.goBack(); //unread the '[', so that we can start looking for ipv6
			return false;
		}
		else if (originalLength > 0 || numSlashes > 0 || !CharUtils::isAlpha(curr))
		{
			// if it's not a character a-z or A-Z then assume we aren't matching scheme, but instead
			// matching username and password.
			_reader.goBack();
			return readUserPass(0);
		}
	}

	return false;
}
bool UrlDetector::readUserPass(int beginningOfUsername)
{
	//The start of where we are.
	int start = _buffer.size();

	//keep looping until "done"
	bool done = false;

	//if we had a dot in the input, then it might be a domain name and not a username and password.
	bool rollback = false;
	while (!done && !_reader.eof()) 
	{
		char curr = _reader.read();
		// if we hit this, then everything is ok and we are matching a domain name.
		if (curr == '@') 
		{
			//std::cout << "@@@@@@@" << std::endl;
			_buffer.Append(curr);
			_currentUrlMarker.setIndex(URL_USERNAME_PASSWORD, beginningOfUsername);
			string temp = "";
			return readDomainName(temp);
		}
		else if (CharUtils::isDot(curr) || curr == '[') 
		{
			//everything is still ok, just remember that we found a dot or '[' in case we might need to backtrack
			_buffer.Append(curr);
			rollback = true;
		}
		else if (curr == '#' || curr == ' ' || curr == '/'
			|| checkMatchingCharacter(curr) !=  CharacterNotMatched || curr == ',' || curr == ';') 
		{
			//one of these characters indicates we are invalid state and should just return.
			rollback = true;
			done = true;
		}
		/*
		else if (curr == ',' || curr == ';' )
		{
			rollback = true;
			done = true;
		}*/
		else 
		{
			//all else, just append character assuming its ok so far.
			_buffer.Append(curr);
		}
	}

	if (rollback) 
	{
		//got to here, so there is no username and password. (We didn't find a @)
		int distance = _buffer.size() - start;
		_buffer.Delete(start, _buffer.size());//

		int currIndex = max(_reader.getPosition() - distance - (done ? 1 : 0), 0);
		_reader.seek(currIndex);

		return false;
	}
	else 
	{
		//return readEnd(ReadEndState::InvalidUrl);
		return readEnd(InvalidUrl);
	}
}
bool UrlDetector::readDomainName(string &current)
{
	//std::cout << __FUNCTION__ << " " << current << std::endl;

	int hostIndex = current.empty() ? _buffer.size() : _buffer.size() - current.size();
	_currentUrlMarker.setIndex(URL_HOST, hostIndex);
	//create the domain name reader and specify the handler that will be called when a quote character
	//or something is found.

	//CharacterHandler *handler = new UrlDetector();
	DomainNameReader reader(_reader,_buffer,current,_options);
	//delete(handler);
	//Try to read the dns and act on the response.
	ReaderNextState state = reader.readDomainName();//
	
	_reader.setPosition(reader.getReaderPosition());
	_reader.setBacktrackedCount(reader.getBacktracked());
    reader.setBuffer(_buffer);
	switch (state) 
	{
	case ValidDomainName:
		return readEnd(ValidUrl);
	case ReadFragment:
		return readFragment();
	case ReadPath:
		return readPath();
	case ReadPort:
		return readPort();
	case ReadQueryString:
		return readQueryString();
	default:
		return readEnd(InvalidUrl);
	}
}

bool UrlDetector::readFragment() {
	_currentUrlMarker.setIndex(URL_FRAGMENT, _buffer.size() - 1);

	while (!_reader.eof()) 
	{
		char curr = _reader.read();

		//if it's the end , ; or space, then a valid url was read.
		if (curr == ' ' || checkMatchingCharacter(curr) !=  CharacterNotMatched || curr == ';')
		{
			//return readEnd(ReadEndState::ValidUrl);
			return readEnd(ValidUrl);
		}
		else 
		{
			//otherwise keep appending.
			_buffer.Append(curr);
		}
	}
	//if we are here, anything read is valid.
	return readEnd(ValidUrl);
}

bool UrlDetector::readQueryString() {
	_currentUrlMarker.setIndex(URL_QUERY, _buffer.size() - 1);
	while (!_reader.eof()) 
	{
		char curr = _reader.read();

		if (curr == '#') 
		{ //fragment
			_buffer.Append(curr);
			return readFragment();
		}
		else if (curr == ' ' || checkMatchingCharacter(curr) != CharacterNotMatched ) 
		{
			//end of query string
			return readEnd(ValidUrl);
		}
		else 
		{ //all else add to buffer.
			_buffer.Append(curr);
		}
	}
	//a valid url was read.
	return readEnd(ValidUrl);
}

bool UrlDetector::readPort() 
{
	_currentUrlMarker.setIndex(URL_PORT, _buffer.size());
	//The length of the port read.
	int portLen = 0;

	while (!_reader.eof()) 
	{
		//read the next one and remember the length
		char curr = _reader.read();
		portLen++;
		if (curr == '/') 
		{
			//continue to read path
			_buffer.Append(curr);
			return readPath();
		}
		else if (curr == '?') 
		{
			//continue to read query string
			_buffer.Append(curr);
			return readQueryString();
		}
		else if (curr == '#')
		{
			//continue to read fragment.
			_buffer.Append(curr);
			return readFragment();
		}
		//else if (checkMatchingCharacter(curr) == CharacterMatch.CharacterMatchStop || !CharUtils::isNumeric(curr)) 
		else if (checkMatchingCharacter(curr) == CharacterMatchStop || !CharUtils::isNumberic(curr))
		{
			//if we got here, then what we got so far is a valid url. don't append the current character.
			_reader.goBack();

			//no port found; it was something like google.com:hello.world
			if (portLen == 1) 
			{
				//remove the ":" from the end.
				_buffer.Delete(_buffer.size() - 1, _buffer.size());
			}
			_currentUrlMarker.unsetIndex(URL_PORT);
			return readEnd(ValidUrl);
		}
		else 
		{
			//this is a valid character in the port string.
			_buffer.Append(curr);
		}
	}
	//found a correct url
	//return readEnd(ReadEndState::ValidUrl);
	return readEnd(ValidUrl);
}

bool UrlDetector::readPath() 
{
	_currentUrlMarker.setIndex(URL_PATH, (int)_buffer.size() - 1);
	while (!_reader.eof()) 
	{
		//read the next char
		char curr = _reader.read();
		if (curr == ' ' || checkMatchingCharacter(curr) != CharacterNotMatched || curr == ';')
		{
			//if end of state and we got here, then the url is valid.
			return readEnd(ValidUrl);
		}
		//append the char
		_buffer.Append(curr);

		//now see if we move to another state.
		if (curr == '?') 
		{
			//if ? read query string
			return readQueryString();
		}
		else if (curr == '#') 
		{
			//if # read the fragment
			return readFragment();
		}
	}

	//end of input then this url is good.
	//return readEnd(ReadEndState::ValidUrl);
	return readEnd(ValidUrl);
}
bool UrlDetector::readEnd(ReadEndState state) {
	//if the url is valid and greater then 0
	if(state == ValidUrl && (_buffer.size() > 0))
	{

		//get the last character. if its a quote, cut it off.
		int len = _buffer.size();
		if (_quoteStart && (_buffer.charAt(len - 1) == '\"'))
		{
			_buffer.Delete(len - 1, len);//
		}
		//Add the url to the list of good urls.
		if (_buffer.size() > 0)
		{
			//std::cout << "push_ok " << _buffer.ToString() << std::endl;
			string originalUrl = _buffer.ToString();
			_currentUrlMarker.setOriginalUrl(originalUrl);
            Url temp(_currentUrlMarker);
            _urlList.push_back(temp);
		}
	}
	//clear out the buffer.
	_buffer.Delete(0, _buffer.size());
	//reset the state of internal objects.
	_quoteStart = false;
	_hasScheme = false;
	_dontMatchIpv6 = false;
	_currentUrlMarker = UrlMarker();
	return state == ValidUrl;
}

