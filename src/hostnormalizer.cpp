#include "hostnormalizer.h"
#include <algorithm>
std::vector<std::string> HostNormalizer::splitbyDot(const std::string &src, const char c, int nlimit){
    size_t nCnt = count(src.begin(), src.end(), c);
    std::vector<std::string> ret;
    if(nCnt == 0 || nlimit == 1){
        ret.push_back(src);
        return ret;
    }
    string temp;
    for(size_t i = 0 ; i < src.size(); ++i){
        if(src[i] == c){        
            ret.push_back(temp);
            temp.clear();
        }else{
            temp.push_back(src[i]);
        }
    }
    if(!temp.empty())
        ret.push_back(temp);
    for(size_t i = ret.size(); i < nCnt + 1; ++i)
        ret.push_back("");
    if(nlimit < 0){
        return ret;
    }
    else if(nlimit == 0){
        std::vector<std::string>::reverse_iterator it(ret.rbegin());
        while(it->empty())
            it++;
        ret.erase(it.base(), ret.end());
        return ret;
    }
    std::vector<std::string> rcpy; 
    int size = ret.size();
    if( size < nlimit)
        return ret;
    for(int i = 0 ; i < nlimit; ++i )
        rcpy.push_back(ret[i]);
    return rcpy;
}

HostNormalizer::HostNormalizer(const string &host) : _host(host),_useIpv6Decoded(false),_useUpv4Decoded(false) {
    normalizeHost();
}
void HostNormalizer::normalizeHost() {
    if (_host.empty())
        return;
    string host;
    try {
        //remove the high unicode characters
        for(size_t i = 0 ; i < _host.size(); ++i){
            host.push_back(_host[i] & 0xFF);
        }
    } catch (...) {
        // occurs when the url is invalid. Just return
        Log().log().setLevel(LOG_DEBUG_LEVEL).format("IllegalArgumentException happend. [at FILE:%s FUNC:%s "
                        "LINE:%d]",__FILE__,__FUNCTION__,__LINE__).toFile();
        return;
    }
    host = StringUtils::toLowerCase(host);

    host = UrlUtil::decode(host);
    std::vector<ubyte> tempVec(16,0);
    _bytes = tryDecodeHostToIp(tempVec, host);
    
    if (!_bytes.empty()) {
        try {
            // address = InetAddress::getByAddress(_bytes);//
            if(_useIpv6Decoded){ //try to decode with ipv6
                InetAddress address(_bytes, INET_FAMILY_IPV6);
                host = "[" + address.getHostAddress() + "]";
            }else if(_useUpv4Decoded){
                InetAddress address(_bytes, INET_FAMILY_IPV4);
                host = address.getHostAddress();
            }else{
                _bytes.clear();
            }
        } catch (...) {
            //日志输出
            Log().log().setLevel(LOG_ERR_LEVEL).format("IPADDRESS Can't convert to host(string handler error). [at FILE:%s FUNC:%s LINE:%d]",
                            __FILE__,__FUNCTION__,__LINE__).toFile();
            return;
        }
    }
    if (host.empty()) {
        return;
    }

    host = UrlUtil::removeExtraDots(host);
    string temp = UrlUtil::encode(host);
    _normalizedHost = StringUtils::replaceAlls(temp, "\\x", "%");
}

vector<ubyte> &HostNormalizer::tryDecodeHostToIp(vector<ubyte> &bytes,const string &host) {
    bool validEmpty = true;
    if (StringUtils::startsWith(host,"[") && StringUtils::endsWith(host,"]"))
        return tryDecodeHostToIPv6(bytes, host);
    return tryDecodeHostToIPv4(bytes, host,validEmpty);
}
vector<ubyte> &HostNormalizer::tryDecodeHostToIPv4(std::vector<ubyte> &bytes,const string &host, bool &validEmpty) {
    std::vector<std::string> parts;
    CharUtils::splitByDot(parts, host); //use . or %2e to split the host parts
    // vector<byte> bytes;
    int numParts = (int)parts.size();
    if (numParts != 4 && numParts != 1) {
        return bytes;
    }
    string parsedNum;
    // string regex1 = "0x";
    // string regex2 = "0";
    int base;
    for (size_t i = 0; i < parts.size(); i++) {
        if (StringUtils::startsWith(parts[i], "0x")) {  // hex
            parsedNum = parts[i].substr(2);
            base = 16;
        } else if (StringUtils::startsWith(parts[i], "0")) {  // octal
            parsedNum = parts[i].substr(1);
            base = 8;
        } else {  // decimal
            parsedNum = parts[i];
            base = 10;
        }
        long section = strtol(parsedNum.c_str(), NULL, base);
        if (((numParts == 4) &&
             (section > MAX_IPV4_PART)) ||  // This would look like 288.1.2.4
            ((numParts == 1) &&
             (section >
              MAX_NUMERIC_DOMAIN_VALUE)) ||  // This would look like 4294967299
            section < MIN_IP_PART) {
            return bytes;
        }
        if (numParts == 4) {
            bytes[IPV4_MAPPED_IPV6_START_OFFSET + i] = (ubyte)section;
        } else {  // numParts == 1
            int index = IPV4_MAPPED_IPV6_START_OFFSET;
            bytes[index++] = (ubyte)((section >> 24) & 0xFF);
            bytes[index++] = (ubyte)((section >> 16) & 0xFF);
            bytes[index++] = (ubyte)((section >> 8) & 0xFF);
            bytes[index] = (ubyte)(section & 0xFF);
            _useUpv4Decoded= true;
            return bytes;
        }
    }
    validEmpty = false;
    _useUpv4Decoded = true;
    return bytes;
}

vector<ubyte> &HostNormalizer::tryDecodeHostToIPv6(std::vector<ubyte> &bytes,const string &host) {
    std::string ip = host.substr(1, host.length() - 2);
    std::vector<std::string> parts = HostNormalizer::splitbyDot(ip, ':', -1);
    if(parts.size() < 3){
        return bytes;
    }
    //Check for embedded ipv4 address
    string lastPart = parts[parts.size()-1];
    int zoneIndexStart = lastPart.rfind("%");
    string lastPartWithoutZoneIndex = zoneIndexStart == -1 ? lastPart : lastPart.substr(0, zoneIndexStart);
    std::vector<ubyte> ipv4Address(16,0);
    bool validEmpty = true;
    if(!isHexSection(lastPartWithoutZoneIndex)){
        ipv4Address = tryDecodeHostToIPv4(ipv4Address, lastPartWithoutZoneIndex, validEmpty);
    }   
    //How many parts do we need to fill by the end of this for loop?
    int totalSize = validEmpty ? 8 : 6;
    //How many zeroes did we fill in the case of double colons? Ex: [::1] will have numberOfFilledZeroes = 7
    int numberOfFilledZeroes = 0;
    //How many sections do we have to parse through? Ex: [fe80:ff::192.168.1.1] size = 3, another ex: [a:a::] size = 4
    size_t size = validEmpty ? parts.size() : parts.size() - 1;

    for(size_t i = 0;i < size; ++i){
        int lenPart = parts[i].length();
        if (lenPart == 0 && i != 0 && i != parts.size() - 1) {
            numberOfFilledZeroes = totalSize - size;
            for (size_t k = i; k < numberOfFilledZeroes + i; k++) {
                //System.arraycopy(sectionToTwoBytes(0), 0, bytes, k * 2, 2);
                std::vector<ubyte> tempVec = sectionToTwoBytes(0);
                bytes[k*2] = tempVec[0];
                bytes[k*2+1] = tempVec[1];
            }
        }
        int parseNum = lenPart == 0? 0 : std::stoi(parts[i],nullptr, 16);
        if(parseNum > MAX_IPV6_PART || parseNum < MIN_IP_PART){
            return bytes;
        }
        std::vector<ubyte> tempVec = sectionToTwoBytes(parseNum);
        bytes[(numberOfFilledZeroes+i) * 2] = tempVec[0];
        bytes[(numberOfFilledZeroes+i) * 2 + 1] = tempVec[1];
        //System.arraycopy(sectionToTwoBytes(section), 0, bytes, (numberOfFilledZeroes + i) * 2, 2);
    }
    if(!validEmpty){
        // System.arraycopy(ipv4Address, IPV4_MAPPED_IPV6_START_OFFSET, bytes, IPV4_MAPPED_IPV6_START_OFFSET,NUMBER_BYTES_IN_IPV4);
        for(size_t i = 0; i < NUMBER_BYTES_IN_IPV4; i++){
            bytes[IPV4_MAPPED_IPV6_START_OFFSET+i] = ipv4Address[IPV4_MAPPED_IPV6_START_OFFSET+i];
        }
    }
    _useIpv6Decoded = true;
    return bytes;
}

bool HostNormalizer::isHexSection(string &section) {
	for (int i = 0; i < (int)section.size(); i++) {
		if (!CharUtils::isHex(section[i])) {
			return false;
		}
	}
	return true;
}

std::vector<ubyte> HostNormalizer::sectionToTwoBytes(int section){
        std::vector<ubyte> bytes(2);
        bytes[0] = (ubyte)((section >> 8) & 0xff);
        bytes[1] = (ubyte)(section & 0xff);
        return bytes;
}
vector<ubyte> HostNormalizer::getBytes() {
    return _bytes;
}

string HostNormalizer::getNormalizedHost() {
    return _normalizedHost;
}
