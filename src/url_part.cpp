#include "url_part.h"

int UrlPart_T::getPart(UrlPart part) {
    int index;
    switch (part) {
    case URL_FRAGMENT:
        index = 1;
        break;
    case URL_QUERY:
        index = 2;
        break;
    case URL_PATH:
        index = 3;
        break;
    case URL_PORT:
        index = 4;
        break;
    case URL_HOST:
        index = 5;
        break;
    case URL_USERNAME_PASSWORD:
        index = 6;
        break;
    case URL_SCHEME:
        index = 7;
        break;
    default:
        index = 7;
        break;
    }
    return index;
}

UrlPart UrlPart_T::getPartByIndex(int index) {
    switch (index) {
    case 0:
        return URL_END;
    case 1:
        return URL_FRAGMENT;
    case 2:
        return URL_QUERY;
    case 3:
        return URL_PATH;
    case 4:
        return URL_PORT;
    case 5:
        return URL_HOST;
    case 6:
        return URL_USERNAME_PASSWORD;
    case 7:
        return URL_SCHEME;
    default:
        return URL_SCHEME;
    }
}

UrlPart UrlPart_T::getNextUrlPart() {
    _index--;
    _nextPart = getPartByIndex(_index);
    return _nextPart;
}

UrlPart_T::UrlPart_T(UrlPart nextPart) {
    _index = getPart(nextPart);
    _nextPart = nextPart;
}