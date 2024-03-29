#include "url_marker.h"
#include "log.h"

UrlMarker::UrlMarker() :
        _schemeIndex(-1),
        _usernamePasswordIndex(-1),
        _hostIndex(-1),
        _portIndex(-1),
        _pathIndex(-1),
        _queryIndex(-1),
        _fragmentIndex(-1) {}

void UrlMarker::setOriginalUrl(const std::string &originalUrl) {
    _originalUrl = originalUrl;
}

std::string UrlMarker::getOriginalUrl() const {
    return _originalUrl;
}

void UrlMarker::setIndex(UrlPart urlPart, int index) {
    switch (urlPart) {
    case URL_SCHEME:
        _schemeIndex = index;
        break;
    case URL_USERNAME_PASSWORD:
        _usernamePasswordIndex = index;
        break;
    case URL_HOST:
        _hostIndex = index;
        break;
    case URL_PORT:
        _portIndex = index;
        break;
    case URL_PATH:
        _pathIndex = index;
        break;
    case URL_QUERY:
        _queryIndex = index;
        break;
    case URL_FRAGMENT:
        _fragmentIndex = index;
        break;
    default:
        break;
    }
}

int UrlMarker::indexOf(UrlPart urlPart) {
    switch (urlPart) {
    case URL_SCHEME:
        return _schemeIndex;
    case URL_USERNAME_PASSWORD:
        return _usernamePasswordIndex;
    case URL_HOST:
        return _hostIndex;
    case URL_PORT:
        return _portIndex;
    case URL_PATH:
        return _pathIndex;
    case URL_QUERY:
        return _queryIndex;
    case URL_FRAGMENT:
        return _fragmentIndex;
    default:
        return -1;
    }
}

void UrlMarker::unsetIndex(UrlPart urlPart) {
    setIndex(urlPart, -1);
}

UrlMarker UrlMarker::setIndices(const std::vector<int> indices) {
    if (indices.empty() || indices.size() != 7) {
        Log().log()
                .setLevel(LOG_ERR_LEVEL)
                .format("IllegalArgumentException:Malformed index array. [at "
                        "FILE:%s FUNC:%s LINE:%d]",
                        __FILE__,
                        __FUNCTION__,
                        __LINE__)
                .toFile();
        return UrlMarker();
    }
    setIndex(URL_SCHEME, indices[0]);
    setIndex(URL_USERNAME_PASSWORD, indices[1]);
    setIndex(URL_HOST, indices[2]);
    setIndex(URL_PORT, indices[3]);
    setIndex(URL_PATH, indices[4]);
    setIndex(URL_QUERY, indices[5]);
    setIndex(URL_FRAGMENT, indices[6]);
    return *this;
}
