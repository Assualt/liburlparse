#ifndef _URLMARKER_H
#define _URLMARKER_H

#include <iostream>
#include "urlpart.h"

#include <string.h>
#include <vector>
using namespace std;
class UrlMarker {
public:
    UrlMarker();
    // Url createUrl();
    void setOriginalUrl(const std::string &originalUrl);
    std::string getOriginalUrl() const;
    void setIndex(UrlPart urlPart, int index);
    int indexOf(UrlPart urlPart);
    void unsetIndex(UrlPart urlPart);
private:
    std::string _originalUrl;
    int _schemeIndex;
    int _usernamePasswordIndex;
    int _hostIndex;
    int _portIndex;
    int _pathIndex;
    int _queryIndex;
    int _fragmentIndex;

protected:
    UrlMarker setIndices(const std::vector<int> indices);
};

#endif  // !_URLMARKER_H
