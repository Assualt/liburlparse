#ifndef _URL_H
#define _URL_H

#include <list>
#include <iostream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cstddef>

#include "./urldetector.h"
#include "./urldetectoroptions.h"
#include "./stringutils.h"
#include "./urlutil.h"
#include "./urlmarker.h"
#include "./hostnormalizer.h"
#include "./pathnormalizer.h"
//日志文件
//#include "./glog.h

#define ubyte unsigned char
using namespace std;
//using namespace __gnu_cxx;
/**
* Creating own Uri class since java.net.Uri would throw parsing exceptions
* for URL's considered ok by browsers.
*
* Also to avoid further conflict, this does stuff that the normal Uri object doesn't do:
* - Converts http://google.com/a/b/.//./../c to http://google.com/a/c
* - Decodes repeatedly so that http://host/%2525252525252525 becomes http://host/%25 while normal decoders
*     would make it http://host/%25252525252525 (one less 25)
* - Removes tabs and new lines: http://www.google.com/foo\tbar\rbaz\n2 becomes "http://www.google.com/foobarbaz2"
* - Converts IP addresses: http://3279880203/blah becomes http://195.127.0.11/blah
* - Strips fragments (anything after #)
*
*/
class NormalizedUrl;
class Url
{
public:
	string getFullUrlWithoutFragment();
	string getScheme();
	/**
	* Returns a url given a single url.
	*/
	Url create(string &url);
	/**
	* Returns a normalized url given a url object
	*/
	NormalizedUrl normalize();
	/*Override*/
	string toString();

	/**
	* Note that this includes the fragment
	* @return Formats the url to: [scheme]://[username]:[password]@[host]:[port]/[path]?[query]#[fragment]
	*/
	string getFullUrl();
	string getFragment();
	string getUsername();
	string getPassword();
	virtual string getHost();

	/**
	* port = 0 means it hasn't been set yet. port = -1 means there is no port
	*/
	int getPort();

	virtual string getPath();

	string getQuery();
	/**
	* Always returns null for non normalized urls.
	*/
	virtual vector<ubyte> getHostBytes();
	string getOriginalUrl();
	explicit Url(UrlMarker& urlMarker);	
	Url();
	~Url();
private:
	string DEFAULT_SCHEME ;
	map<string, int> SCHEME_PORT_MAP;

	UrlMarker _urlMarker;
	string _scheme;
	string _username;
	string _password;
	string _host;
	int _port;
	string _path;
	string _query;
	string _fragment;
	string _originalUrl;

	void populateUsernamePassword();
	/**
	* @param urlPart The url part we are checking for existence
	* @return Returns true if the part exists.
	*/
	bool exists(UrlPart urlPart);
	/**
	* For example, in http://yahoo.com/lala/, nextExistingPart(UrlPart.HOST) would return UrlPart.PATH
	* @param urlPart The current url part
	* @return Returns the next part; if there is no existing next part, it returns null
	*/
	UrlPart nextExistingPart(UrlPart urlPart);

	/**
	* @param part The part that we want. Ex: host, path
	*/
	string getPart(UrlPart part);
protected:
	void setRawPath(string &path);
	void setRawHost(string &host);
	string getRawPath();
	string getRawHost();
	UrlMarker getUrlMarker();
};

class NormalizedUrl : public Url
{
public:
	/**
	* Returns a normalized url given a single url.
	*/
	NormalizedUrl create(string &url);
	explicit NormalizedUrl(UrlMarker &urlMarker);
	NormalizedUrl();
	//Override
	string getPath();
	string getHost();
	vector<ubyte> getHostBytes();

	void populateHostAndHostBytes();
private:
	bool _isPopulated;
	vector<ubyte> _hostBytes;


};

#endif
