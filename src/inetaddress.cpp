#include "inetaddress.h"
InetAddress::InetAddress(const int nfamily) : family(nfamily), address(0) {}
InetAddress::InetAddress(const std::vector<ubyte> ipaddr, const int nfamily) : family(nfamily), _bytes(ipaddr) {}

std::vector<ubyte> &InetAddress::getBytes() {
    return _bytes;
}

std::string InetAddress::getHostAddress() {
    if (family == INET_FAMILY_IPV4)
        return InetAddressUtils::getIPV4HostAddress(_bytes);
    return InetAddressUtils::getIPV6HostAddress(_bytes);
}
std::string InetAddressUtils::getIPV4HostAddress(const std::vector<ubyte> &addr) {
    std::string hostName = "";
    if (!addr.empty() && addr.size() == 4) {
        // judge valid
        // top of zeros shouldn't been zero
        if (addr[0] != 0 && addr[0] <= 255) {
            for (int i = 1; i < 4; i++) {
                if (addr[i] < 0 || addr[i] > 255)
                    return "";  // Invalid ipv4
            }
        }
        for (int i = 0; i < 4; i++) {
            hostName += to_string(addr[i]);
            if (i != 3)
                hostName += ".";
        }
        return hostName;
    } else if (addr.size() > 4) {  // 取最后４个
        std::vector<ubyte> tempvec;
        for (size_t i = addr.size() - 4; i < addr.size(); ++i) tempvec.push_back(addr[i]);
        return InetAddressUtils::getIPV4HostAddress(tempvec);
    }
    return "";
}
std::string InetAddressUtils::getIPV6HostAddress(const std::vector<ubyte> &addr) {
    std::string hostName = "";
    if (!addr.empty() && addr.size() == 16) {
        for (int i = 1; i < 16; ++i) {
            if (addr[i] < 0 || addr[i] > 255)
                return "";  // invalid ipv6
        }
        for (size_t i = 0; i < addr.size() - 1; i += 2) {
            int p = addr[i] & 0xFF, p1 = addr[i + 1] & 0xFF;
            if (p == 0 && p1 == 0) {
                hostName += "0";
            } else if (p == 0) {
                char temp[10];
                sprintf(temp, "%x", p1);
                hostName += temp;
            } else if (p1 == 0) {
                char temp[10];
                sprintf(temp, "%02x", p);
                hostName += temp;
                hostName += "00";
            } else {
                char temp[10];
                sprintf(temp, "%x%02x", p, p1);
                hostName += temp;
            }
            if (i != addr.size() - 2)
                hostName += ":";
        }
        return hostName;
    }
    return "";
}
