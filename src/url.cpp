#include "url.h"

Url::Url(UrlMarker &urlMarker):_port(0)
{
	SCHEME_PORT_MAP.insert(make_pair("http",80));
	SCHEME_PORT_MAP.insert(make_pair("https",443));
	SCHEME_PORT_MAP.insert(make_pair("ftp",21));
	DEFAULT_SCHEME = "http";
	_urlMarker = urlMarker;
	_originalUrl = urlMarker.getOriginalUrl();
}
Url::Url(){}
Url::~Url(){}
Url Url::create(string &url)
{
	string format = StringUtils::replaceAlls(StringUtils::trim(url), " ", "%20");
	string formattedString = UrlUtil::removeSpecialSpaces(format);
	UrlDetectorOptions_T test(ALLOW_SINGLE_LEVEL_DOMAIN);
	list<Url> urls = UrlDetector(formattedString,test).detect();
	if (urls.size() == 1) {
		return urls.front();
	}
	else if (urls.size() == 0) {
		//std::cerr<<"We couldn't find any urls in string: " + url<<std::endl;
		Log().log().setLevel(LOG_DEBUG_LEVEL).format("We couldn't find any urls in string. [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
		return Url();// 
	}
	else {
		//std::cerr<<"We found more than one url in string: " + url<<std::endl;
		//ÈÕÖ¾Êä³ö
		Log().log().setLevel(LOG_DEBUG_LEVEL).format("We found more than one url in such string. [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();

		return Url();//
	}
}

NormalizedUrl Url::normalize() {
	return NormalizedUrl(_urlMarker);
}

string Url::toString() 
{
	return getFullUrl();
}
string Url::getFullUrl() {
	string fragement = getFragment();
	return getFullUrlWithoutFragment() + StringUtils::defaultstring(fragement);
}

string Url::getFullUrlWithoutFragment() 
{
	StringBuilder<char> url;
	if (!getScheme().empty()) 
	{
		url.Append(getScheme());
		url.Append(":");
	}
	url.Append("//");
	if (!getUsername().empty())
	{
		url.Append(getUsername());
		if (!getPassword().empty())
		{
			url.Append(":");
			url.Append(getPassword());
		}
		url.Append("@");
	}

	url.Append(getHost());
	if (getPort() > 0 && getPort() != SCHEME_PORT_MAP[getScheme()])
	{
		url.Append(":");
		url.Append(getPort());
	}

	url.Append(getPath());
	url.Append(getQuery());

	return url.ToString();
}

string Url::getScheme() 
{
	string regex1 = "//";
	if (_scheme.empty())
	{
		if (exists(URL_SCHEME))
		{
			_scheme = getPart(URL_SCHEME);
			int index = _scheme.find(":");
			if (index != -1) 
			{
				_scheme = _scheme.substr(0, index);
			}
		}
		else if (!StringUtils::startsWith(_originalUrl,regex1))
		{
			_scheme = DEFAULT_SCHEME;
		}
	}
	return StringUtils::defaultstring(_scheme);
}

string Url::getUsername()
{
	if (_username.empty())
	{
		populateUsernamePassword();
	}
	return StringUtils::defaultstring(_username);
}

string Url::getPassword()
{
	if (_password.empty())
	{
		populateUsernamePassword();
	}
	return StringUtils::defaultstring(_password);
}

string Url::getHost()
{
	if (_host.empty())
	{
		_host = getPart(URL_HOST);
		if (exists(URL_PORT))
		{
			_host = _host.substr(0, _host.size() - 1);
		}
	}
	return _host;
}

int Url::getPort()
{
	if (_port == 0)
	{
		string portString = getPart(URL_PORT);
		if (!portString.empty())
		{
			try
			{
				_port = atoi(portString.c_str());
			}
			catch(int)
			{
				// std::cout<< "NumberFormatException in url.cpp 136 line" << std::endl;
				_port = -1;
			}
		}
		else if (SCHEME_PORT_MAP.count(getScheme()))
		{
			if (SCHEME_PORT_MAP.find(getScheme()) != SCHEME_PORT_MAP.end())
			{
				_port = SCHEME_PORT_MAP[getScheme()];
			}
		}
		else
		{
			_port = -1;
		}
	}
	return _port;
}	

string Url::getPath()
{
	if (_path.empty())
	{
		_path = exists(URL_PATH) ? getPart(URL_PATH) : "/";
	}
	return _path;
}

string Url::getQuery()
{
	if (_query.empty()) 
	{
		_query = getPart(URL_QUERY);
	}
	return StringUtils::defaultstring(_query);
}

string Url::getFragment() {
	if (_fragment.empty()) 
	{
		_fragment = getPart(URL_FRAGMENT);
	}
	return StringUtils::defaultstring(_fragment);
}

vector<ubyte> Url::getHostBytes()
{
	return vector<ubyte>();
}
string Url::getOriginalUrl()
{
	return _originalUrl;
}
void Url::populateUsernamePassword()
{
	if (exists(URL_USERNAME_PASSWORD))
	{
		string usernamepassword = getPart(URL_USERNAME_PASSWORD);
		// string temp = usernamepassword.substr(0,usernamepassword.size()-1);
		list<string> usernamePasswordParts;
		// string spliter = ":";
		StringUtils::split(usernamePasswordParts,usernamepassword.substr(0,usernamepassword.size()-1),":");//
		vector<string> ve;
		StringUtils::list2vector(ve,usernamePasswordParts);
		if (usernamePasswordParts.size() == 1) 
		{
			_username = ve[0];
		}
		else if (usernamePasswordParts.size() == 2) 
		{
			_username = ve[0];
			_password = ve[1];
		}
	}
}

bool Url::exists(UrlPart urlPart) 
{
	return (urlPart != 0) && _urlMarker.indexOf(urlPart) >= 0;
}

UrlPart Url::nextExistingPart(UrlPart urlPart)
{
	UrlPart_T part(urlPart);
	UrlPart nextPart = part.getNextUrlPart();
	if (exists(nextPart))
	{
		return nextPart;
	}
	else if (nextPart == 0) 
	{
		return URL_FRAGMENT;
	}
	return nextExistingPart(nextPart);
	
}

/**
* @param part The part that we want. Ex: host, path
*/
string Url::getPart(UrlPart part) {
	if (!exists(part))
	{
		return string();
	}
	UrlPart nextPart = nextExistingPart(part);
	if ( nextPart == 0) 
	{
		return _originalUrl.substr(_urlMarker.indexOf(part));
	}
	return _originalUrl.substr(_urlMarker.indexOf(part), _urlMarker.indexOf(nextPart)-_urlMarker.indexOf(part));
}


void Url::setRawPath(string &path) {
	_path = path;
}

void Url::setRawHost(string &host) {
	_host = host;
}

string Url::getRawPath() {
	return _path;
}

string Url::getRawHost() {
	return _host;
}

UrlMarker Url::getUrlMarker() {
	return _urlMarker;
}


//NormalizedUrl

NormalizedUrl::NormalizedUrl(UrlMarker& urlMarker):Url(urlMarker),_isPopulated(false)
{}
NormalizedUrl::NormalizedUrl():Url(),_isPopulated(false)
{}
NormalizedUrl NormalizedUrl::create(string &url)
{
	//NormalizedUrl url(Url::getUrlMarker());
	return (*this).Url::create(url).normalize();
	//return url;
}

/**
 *@override
 **/
string NormalizedUrl::getHost()
{
	if (getRawHost().empty())
	{
		populateHostAndHostBytes();
	}
	return getRawHost();
}
/**
 *@override
 **/
string NormalizedUrl::getPath()
{
	if (Url::getRawPath().empty())
	{
		PathNormalizer path;
		string urlgetPath = Url::getPath();
		Url::setRawPath(path.normalizePath(urlgetPath));
	}
	return Url::getRawPath();
}

/**
 * @override
 * Returns the byte representation of the ip address. If the host is not an ip address, it returns null.
 */
vector<ubyte> NormalizedUrl::getHostBytes()
{
	if (_hostBytes.empty())
	{
		populateHostAndHostBytes();
	}
	return _hostBytes;
}

void NormalizedUrl::populateHostAndHostBytes()
{
	if (!_isPopulated)
	{
		HostNormalizer hostNormalizer(Url::getHost());
		string rawhost = hostNormalizer.getNormalizedHost();
		setRawHost(rawhost);
		_hostBytes = hostNormalizer.getBytes();
		_isPopulated = true;
	}
}

