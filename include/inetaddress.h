#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <vector>
#include <cstring>

#define INET_FAMILY_IPV4 1
#define INET_FAMILY_IPV6 2

#define ubyte unsigned char
#include <iostream>

using namespace std;
class InetAddress
{
protected:
	static const int IPv4 = INET_FAMILY_IPV4;
	static const int IPv6 = INET_FAMILY_IPV6;
	InetAddress();

public:
	string hostName;
	int family;
	int address;
};

class Inet4Address : public InetAddress
{
public:
	Inet4Address();
	explicit Inet4Address(vector<ubyte> &ipaddr);
};

#endif // !_INETADDRESS_H
