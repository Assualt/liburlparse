#ifndef _PATHNORMALIZER_H
#define _PATHNORMALIZER_H

#include <string>

using namespace std;
class PathNormalizer {
public:
    /**
     * Normalizes the path by doing the following:
     * remove special spaces, decoding hex encoded characters,
     * gets rid of extra dots and slashes, and re-encodes it once
     */
    std::string &normalizePath(std::string &path);

private:
    /**
     * 1. Replaces "/./" with "/" recursively.
     * 2. "/blah/asdf/.." -> "/blah"
     * 3. "/blah/blah2/blah3/../../blah4" -> "/blah/blah4"
     * 4. "//" -> "/"
     * 5. Adds a slash at the end if there isn't one
     */
    static std::string sanitizeDotsAndSlashes(const std::string &path);
};

#endif  // !_PATHNORMALIZER_H
