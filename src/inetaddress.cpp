#include "inetaddress.h"
InetAddress::InetAddress():family(INET_FAMILY_IPV4),address(0)
{}
Inet4Address::Inet4Address():InetAddress(){
	hostName = "";
	address = 0;
	family = INET_FAMILY_IPV4;
}
Inet4Address::Inet4Address(vector<ubyte> &addr) {
	hostName = "";
	family = INET_FAMILY_IPV4;
	if (!addr.empty() && addr.size() == 4)
	{
		address = addr[3] & 0x0F;
		address |= ((addr[2] << 8) & 0xFF00);
		address |= ((addr[1] << 16) & 0xFF0000);
		address |= ((addr[0] << 24) & 0xFF000000); 
	}
}
