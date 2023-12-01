#ifndef _URL_H
#define _URL_H

#include <list>
#include <iostream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cstddef>

#include "url_util.h"
#include "url_marker.h"
#include "host_normalizer.h"
#include "path_normalizer.h"

#define ubyte unsigned char
// using namespace __gnu_cxx;
/**
 * Creating own Uri class since java.net.Uri would throw parsing exceptions
 * for URL's considered ok by browsers.
 *
 * Also to avoid further conflict, this does stuff that the normal Uri object
 * doesn't do:
 * - Converts http://google.com/a/b/.//./../c to http://google.com/a/c
 * - Decodes repeatedly so that http://host/%2525252525252525 becomes
 * http://host/%25 while normal decoders would make it
 * http://host/%25252525252525 (one less 25)
 * - Removes tabs and new lines: http://www.google.com/foo\tbar\rbaz\n2 becomes
 * "http://www.google.com/foobarbaz2"
 * - Converts IP addresses: http://3279880203/blah becomes
 * http://195.127.0.11/blah
 * - Strips fragments (anything after #)
 *
 */
class NormalizedUrl;
class Url {
public:
    std::string getFullUrlWithoutFragment();
    
    std::string getScheme();
    
    /**
     * Returns a normalized url given a url object
     */
    NormalizedUrl normalize();
    
    /*Override*/
    std::string toString();

    /**
     * Note that this includes the fragment
     * @return Formats the url to:
     * [scheme]://[username]:[password]@[host]:[port]/[path]?[query]#[fragment]
     */
    std::string getFullUrl();
    
    std::string getFragment();
    
    std::string getUsername();
    
    std::string getPassword();
    
    virtual std::string getHost();

    /**
     * port = 0 means it hasn't been set yet. port = -1 means there is no port
     */
    int getPort();

    virtual std::string getPath();

    std::string getQuery();
    
    /**
     * Always returns null for non normalized urls.
     */
    virtual vector<ubyte> getHostBytes() const;
    
    std::string getOriginalUrl() const;

public:
    explicit Url(UrlMarker &urlMarker);
    
    Url();
    
    ~Url();

public:
    static std::string DEFAULT_SCHEME;
    
    static std::map<std::string, int> SCHEME_PORT_MAP;
    
    static Url create(std::string &url);

private:
    UrlMarker _urlMarker;
    std::string _scheme;
    std::string _username;
    std::string _password;
    std::string _host;
    int _port;
    std::string _path;
    std::string _query;
    std::string _fragment;
    std::string _originalUrl;

private:
    void populateUsernamePassword();
    
    /**
     * @param urlPart The url part we are checking for existence
     * @return Returns true if the part exists.
     */
    bool exists(UrlPart urlPart);
    
    /**
     * For example, in http://yahoo.com/lala/, nextExistingPart(UrlPart.HOST)
     * would return UrlPart.PATH
     * @param urlPart The current url part
     * @return Returns the next part; if there is no existing next part, it
     * returns null
     */
    UrlPart nextExistingPart(UrlPart urlPart);

    /**
     * @param part The part that we want. Ex: host, path
     */
    std::string getPart(UrlPart part);

public:
    void setRawPath(const std::string &path);
    
    void setRawHost(const std::string &host);
    
    std::string getRawPath() const;
    
    std::string getRawHost() const;
    
    UrlMarker getUrlMarker() const;
};

class NormalizedUrl : public Url {
public:
    /**
     * Returns a normalized url given a single url.
     */
    static NormalizedUrl create(std::string &url);
    
    explicit NormalizedUrl(UrlMarker &urlMarker);
    
    NormalizedUrl();

public:
    // Override
    std::string getPath();
    
    std::string getHost();
    
    std::vector<ubyte> getHostBytes();
    
    void populateHostAndHostBytes();

private:
    bool _isPopulated;
    std::vector<ubyte> _hostBytes;
};

#endif
