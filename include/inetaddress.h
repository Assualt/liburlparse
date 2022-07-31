#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <vector>
#include <cstring>

#define INET_FAMILY_IPV4 1
#define INET_FAMILY_IPV6 2

#define ubyte unsigned char
#include <iostream>

using namespace std;
class InetAddress {
public:
    InetAddress(const int nFamily);
    InetAddress(const std::vector<ubyte> ipaddr, const int nFamily = INET_FAMILY_IPV4);

    std::string getHostAddress();

protected:
    std::string hostName;
    int family;
    int address;
    std::vector<ubyte> _bytes;

protected:
    std::vector<ubyte> &getBytes();
};

class InetAddressUtils {
public:
    static std::string getIPV6HostAddress(const std::vector<ubyte> &vec);
    static std::string getIPV4HostAddress(const std::vector<ubyte> &vec);
};

#endif  // !_INETADDRESS_H
