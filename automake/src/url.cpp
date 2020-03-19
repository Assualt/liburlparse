#include "url.h"


std::string Url::DEFAULT_SCHEME = "http";
std::map<std::string,int> Url::SCHEME_PORT_MAP = {
    {"http",80},
    {"https",443},
    {"ftp",21}
};


Url::Url(UrlMarker &urlMarker) : _port(0) {
    _urlMarker = urlMarker;
    _originalUrl = urlMarker.getOriginalUrl();
}
Url::Url() {}
Url::~Url() {}
Url Url::create(std::string &url) {
    std::string format = StringUtils::replaceAlls(StringUtils::trim(url), " ", "%20");
    std::string formattedString = UrlUtil::removeSpecialSpaces(format);
    UrlDetectorOptions_T test(ALLOW_SINGLE_LEVEL_DOMAIN);
    std::list<Url> urls = UrlDetector(formattedString, test).detect();
    if (urls.size() == 1) {
        return urls.front();
    } else if (urls.size() == 0) {
        // std::cerr<<"We couldn't find any urls in string: " + url<<std::endl;
        Log().log().setLevel(LOG_DEBUG_LEVEL).format("We couldn't find any urls in string. [at FILE:%s FUNC:%s LINE:%d]",__FILE__, __FUNCTION__,__LINE__) .toFile();
        return Url();  //
    } else {
        // std::cerr<<"We found more than one url in string: " + url<<std::endl;
        Log().log().setLevel(LOG_DEBUG_LEVEL).format("We found more than one url in such string. [at FILE:%s FUNC:%s LINE:%d]",__FILE__,__FUNCTION__, __LINE__).toFile();
        return Url();  
    }
}

NormalizedUrl Url::normalize() {
    return NormalizedUrl(_urlMarker);
}

std::string Url::toString() {
    return getFullUrl();
}
std::string Url::getFullUrl() {
    std::string fragement = getFragment();
    return getFullUrlWithoutFragment() + StringUtils::defaultstring(fragement);
}

std::string Url::getFullUrlWithoutFragment() {
    StringBuilder url;
    if (!getScheme().empty()) {
        url.append(getScheme());
        url.append(":");
    }
    url.append("//");
    if (!getUsername().empty()) {
        url.append(getUsername());
        if (!getPassword().empty()) {
            url.append(":");
            url.append(getPassword());
        }
        url.append("@");
    }

    url.append(getHost());
    if (getPort() > 0 && getPort() != SCHEME_PORT_MAP[getScheme()]) {
        url.append(":");
        url.append(getPort());
    }

    url.append(getPath());
    url.append(getQuery());

    return url.toString();
}

std::string Url::getScheme() {
    std::string regex1 = "//";
    if (_scheme.empty()) {
        if (exists(URL_SCHEME)) {
            _scheme = getPart(URL_SCHEME);
            int index = _scheme.find(":");
            if (index != -1) {
                _scheme = _scheme.substr(0, index);
            }
        } else if (!StringUtils::startsWith(_originalUrl, regex1)) {
            _scheme = DEFAULT_SCHEME;
        }
    }
    return StringUtils::defaultstring(_scheme);
}

std::string Url::getUsername() {
    if (_username.empty()) {
        populateUsernamePassword();
    }
    return StringUtils::defaultstring(_username);
}

std::string Url::getPassword() {
    if (_password.empty()) {
        populateUsernamePassword();
    }
    return StringUtils::defaultstring(_password);
}

std::string Url::getHost() {
    if (_host.empty()) {
        _host = getPart(URL_HOST);
        if (exists(URL_PORT)) {
            _host = _host.substr(0, _host.size() - 1);
        }
    }
    return _host;
}

int Url::getPort() {
    if (_port == 0) {
        std::string portString = getPart(URL_PORT);
        if (!portString.empty()) {
            try {
                _port = atoi(portString.c_str());
                if(_port < 0 && _port > 65535) //valid port should be range(0,65535)
                    _port = -1;
            } catch (int) {
                // std::cout<< "NumberFormatException in url.cpp 136 line" <<
                // std::endl;
                _port = -1;
            }
        } else if (SCHEME_PORT_MAP.count(getScheme())) {
            if (SCHEME_PORT_MAP.find(getScheme()) != SCHEME_PORT_MAP.end()) {
                _port = SCHEME_PORT_MAP[getScheme()];
            }
        } else {
            _port = -1;
        }
    }
    return _port;
}

std::string Url::getPath() {
    if (_path.empty()) {
        _path = exists(URL_PATH) ? getPart(URL_PATH) : "/";
    }
    return _path;
}

std::string Url::getQuery() {
    if (_query.empty()) {
        _query = getPart(URL_QUERY);
    }
    return StringUtils::defaultstring(_query);
}

std::string Url::getFragment() {
    if (_fragment.empty()) {
        _fragment = getPart(URL_FRAGMENT);
    }
    return StringUtils::defaultstring(_fragment);
}

vector<ubyte> Url::getHostBytes() const{ 
    return vector<ubyte>();
}
std::string Url::getOriginalUrl() const{
    return _originalUrl;
}
void Url::populateUsernamePassword() {
    if (exists(URL_USERNAME_PASSWORD)) {
        std::string usernamepassword = getPart(URL_USERNAME_PASSWORD);
        std::vector<std::string> usernamePasswordParts;
        StringUtils::split(usernamePasswordParts,usernamepassword.substr(0, usernamepassword.size() - 1),":");  //
        if (usernamePasswordParts.size() == 1) {
            _username = usernamePasswordParts[0];
        } else if (usernamePasswordParts.size() == 2) {
            _username = usernamePasswordParts[0];
            _password = usernamePasswordParts[1];
        }
    }
}

bool Url::exists(UrlPart urlPart) {
    return (urlPart != 0) && _urlMarker.indexOf(urlPart) >= 0;
}

UrlPart Url::nextExistingPart(UrlPart urlPart) {
    UrlPart_T part(urlPart);
    UrlPart nextPart = part.getNextUrlPart();
    // std::cout <<"nexpart:" <<nextPart << std::endl;
    if (exists(nextPart)) {
        return nextPart;
    } else if (nextPart == 0) {// end of part
        return URL_END;
    }
    return nextExistingPart(nextPart);
}

/**
 * @param part The part that we want. Ex: host, path
 */
std::string Url::getPart(UrlPart part) {
    if (!exists(part)) {
        return string();
    }
    UrlPart nextPart = nextExistingPart(part);
    if (nextPart == 0) {
        return _originalUrl.substr(_urlMarker.indexOf(part));
    }
    return _originalUrl.substr(_urlMarker.indexOf(part),_urlMarker.indexOf(nextPart) - _urlMarker.indexOf(part));
}

void Url::setRawPath(const std::string &path) {
    _path = path;
}

void Url::setRawHost(const std::string &host) {
    _host = host;
}

std::string Url::getRawPath() const {
    return _path;
}

std::string Url::getRawHost() const {
    return _host;
}

UrlMarker Url::getUrlMarker() const {
    return _urlMarker;
}

// NormalizedUrl

NormalizedUrl::NormalizedUrl(UrlMarker &urlMarker) :
        Url(urlMarker),
        _isPopulated(false) {}
NormalizedUrl::NormalizedUrl() : Url(), _isPopulated(false) {}
NormalizedUrl NormalizedUrl::create(std::string &url) {
    return Url::create(url).normalize();
}

/**
 *@override
 **/
std::string NormalizedUrl::getHost() {
    if (getRawHost().empty()) {
        populateHostAndHostBytes();
    }
    return getRawHost();
}
/**
 *@override
 **/
std::string NormalizedUrl::getPath() {
    if (Url::getRawPath().empty()) {
        PathNormalizer path;
        std::string urlgetPath = Url::getPath();
        Url::setRawPath(path.normalizePath(urlgetPath));
    }
    return Url::getRawPath();
}

/**
 * @override
 * Returns the byte representation of the ip address. If the host is not an ip
 * address, it returns null.
 */
vector<ubyte> NormalizedUrl::getHostBytes() {
    if (_hostBytes.empty()) {
        populateHostAndHostBytes();
    }
    return _hostBytes;
}

void NormalizedUrl::populateHostAndHostBytes() {
    if (!_isPopulated) {
        std::string host = Url::getHost();
        HostNormalizer hostNormalizer(host);
        std::string rawhost = hostNormalizer.getNormalizedHost();
        setRawHost(rawhost);
        _hostBytes = hostNormalizer.getBytes();
        _isPopulated = true;
    }
}
