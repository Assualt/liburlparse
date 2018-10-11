#ifndef _URLPART_H
#define _URLPART_H
enum UrlPart
{
	URL_FRAGMENT = 1,
	URL_QUERY = 2,
	URL_PATH = 3,
	URL_PORT = 4,
	URL_HOST = 5,
	URL_USERNAME_PASSWORD = 6,
	URL_SCHEME = 7
};
class UrlPart_T
{
private:
	UrlPart _nextPart;
	int _index ;
public:
	int getPart(UrlPart part);
	UrlPart getPartByIndex(int index);
	UrlPart getNextUrlPart();
	explicit UrlPart_T(UrlPart nextPart);
};
#endif // !_URLPART_H
