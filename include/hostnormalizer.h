#ifndef _HOSTNORMALIZER_H
#define _HOSTNORMALIZER_H

#include <list>
#include <cstring>
#include <vector>

#include "./urlutil.h"
#include "./stringbuilder.h"
#include "./stringutils.h"
#include "./inetaddress.h"
//日志文件
//#include "./glog.h"

class UrlUtil;
#define ubyte unsigned char

class HostNormalizer
{
public:
	explicit HostNormalizer(const string &host);

	vector<ubyte> getBytes();
	string getNormalizedHost();
private:
	static const long MAX_NUMERIC_DOMAIN_VALUE = 4294967295L;
	static const int MAX_IPV4_PART = 255;
	static const int MIN_IP_PART = 0;
	static const int MAX_IPV6_PART = 0xFFFF;
	static const int IPV4_MAPPED_IPV6_START_OFFSET =12 ;
	static const int NUMBER_BYTES_IN_IPV4 =4;
	string _host;

	//�ֽ�����
	vector<ubyte> _bytes;

	void normalizeHost();
	/**
	* Checks if the host is an ip address. Returns the byte representation of it
	*/
	std::vector<ubyte>& tryDecodeHostToIp(std::vector<ubyte> &bytes,string &host);

	/**
	* This covers cases like:
	* Hexadecimal:        0x1283983
	* Decimal:            12839273
	* Octal:              037362273110
	* Dotted Decimal:     192.168.1.1
	* Dotted Hexadecimal: 0xfe.0x83.0x18.0x1
	* Dotted Octal:       0301.00.046.00
	* Dotted Mixed:       0x38.168.077.1
	*
	* if ipv4 was found, _bytes is set to the byte representation of the ipv4 address
	*/
	vector<ubyte>& tryDecodeHostToIPv4(std::vector<ubyte> &sbytes,string &host);
	string _normalizedHost;

	//bool isHexSection(string &section);
//protected:
//	vector<byte> getBytes();
//	string getNormalizedHost();
};


#endif
