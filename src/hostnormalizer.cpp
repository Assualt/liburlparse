#include "hostnormalizer.h"

HostNormalizer::HostNormalizer(const string &host):_host(host)
{
	//_host = host;
	normalizeHost();
}
void HostNormalizer::normalizeHost()
{
	if (_host.empty()) //
	{
		return ;
	}
	string host;
	try
	{
		host = _host;//***1
	}catch(string)
	{	
		//occurs when the url is invalid. Just return
		//std::cout << "IllegalArgumentException at hostnormalizer.cpp line "<<__LINE__ << std::endl;
		//日志输出
		Log().log().setLevel(LOG_DEBUG_LEVEL).format("IllegalArgumentException happend. [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
		return;
	}
	host = StringUtils::toLowerCase(host);
	
	host = UrlUtil::decode(host);

	_bytes = tryDecodeHostToIp(_bytes,host);
	if (!_bytes.empty()) 
	{
		try 
		{
			Inet4Address address(_bytes);	
			//address = InetAddress::getByAddress(_bytes);//
			string ipAddress;
			//if (address instanceof Inet6Address) 
			//{  ��ʱ������IPV6
			//	host = "[" + ipAddress + "]";
			//}
			//else 
			//{
				host = ipAddress;
			//}
		}
		catch (string) 
		{
			//日志输出
			Log().log().setLevel(LOG_ERR_LEVEL).format("IPADDRESS Can't convert to host(string handler error). [at FILE:%s FUNC:%s LINE:%d]", __FILE__, __FUNCTION__, __LINE__).toFile();
			return;
		}
	}

	if (host.empty()) 
	{
		return;
	}

	host = UrlUtil::removeExtraDots(host);
	string temp = UrlUtil::encode(host);
	_normalizedHost = StringUtils::replaceAlls(temp,"\\x", "%");
}

vector<ubyte>& HostNormalizer::tryDecodeHostToIp(vector<ubyte>& bytes,string &host)
{
	/*
	if (StringUtils::startsWith(host,"[") && StringUtils::endsWith(host,"]")) 
	{
		return tryDecodeHostToIPv6(host);
	}
	*/
	//_bytes = tryDecodeHostToIPv4(host);
	
	return tryDecodeHostToIPv4(bytes,host);

}
vector<ubyte>& HostNormalizer::tryDecodeHostToIPv4(std::vector<ubyte> &bytes,string &host)
{
	list<string> parts;
	vector<string> ve;
	CharUtils::splitByDot(parts,host);
	StringUtils::list2vector(ve,parts);
	// vector<byte> bytes;
	int numParts = (int)parts.size();
	if (numParts != 4 && numParts != 1)
	{
		return bytes;
	}
	
	string parsedNum;
	// string regex1 = "0x";
	// string regex2 = "0";
	int base;
	for (int i = 0; i < (int)parts.size(); i++)
	{
		
		if (StringUtils::startsWith(ve[i],"0x"))
		{ //hex
			parsedNum = ve[i].substr(2);
			base = 16;
		}
		else if (StringUtils::startsWith(ve[i], "0"))
		{ //octal
			parsedNum = ve[i].substr(1);
			base = 8;
		}
		else
		{ //decimal
			parsedNum = ve[i];
			base = 10;
		}
		long section = strtol(parsedNum.c_str(),NULL,base);
		if (((numParts == 4) && (section > MAX_IPV4_PART)) || //This would look like 288.1.2.4
			((numParts == 1) && (section > MAX_NUMERIC_DOMAIN_VALUE)) || //This would look like 4294967299
			section < MIN_IP_PART)
		{
			return bytes;
		}
		if (numParts == 4)
		{
			bytes[IPV4_MAPPED_IPV6_START_OFFSET + i] = (ubyte)section;
		}
		else
		{ //numParts == 1
			int index = IPV4_MAPPED_IPV6_START_OFFSET;
			bytes[index++] = (ubyte)((section >> 24) & 0xFF);
			bytes[index++] = (ubyte)((section >> 16) & 0xFF);
			bytes[index++] = (ubyte)((section >> 8) & 0xFF);
			bytes[index] = (ubyte)(section & 0xFF);
			return bytes;
		}
	}
	return bytes;
}
// bool HostNormalizer::isHexSection(string &section) {
// 	for (int i = 0; i < (int)section.size(); i++) {
// 		if (!CharUtils::isHex(section[i])) {
// 			return false;
// 		}
// 	}
// 	return true;
// }
/*
byte[] HostNormalizer::sectionToTwoBytes(int section)
{
	byte[] bytes = new byte[2];
	bytes[0] = (byte)((section >> 8) & 0xff);
	bytes[1] = (byte)(section & 0xff);
	return bytes;
}
*/
vector<ubyte> HostNormalizer::getBytes() {
	return _bytes;
}

string HostNormalizer::getNormalizedHost()
{
	return _normalizedHost;
}
