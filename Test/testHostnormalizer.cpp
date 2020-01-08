#include <gtest/gtest.h>
#include <iostream>
#include "hostnormalizer.h"


static const std::vector<std::vector<std::string>> 
HostPathMap = {
    {"[fefe::]", "[fefe:0:0:0:0:0:0:0]"},
    {"[::ffff]", "[0:0:0:0:0:0:0:ffff]"},
    {"[::255.255.255.255]", "[0:0:0:0:0:0:ffff:ffff]"},
    {"[::]", "[0:0:0:0:0:0:0:0]"},
    {"[::1]", "[0:0:0:0:0:0:0:1]"},
    {"[aAaA::56.7.7.5]", "[aaaa:0:0:0:0:0:3807:705]"},
    {"[BBBB:ab:f78F:f:DDDD:bab:56.7.7.5]", "[bbbb:ab:f78f:f:dddd:bab:3807:705]"},
    {"[Aaaa::1]", "[aaaa:0:0:0:0:0:0:1]"},
    {"[::192.167.2.2]", "[0:0:0:0:0:0:c0a7:202]"},
    {"[0:ffff::077.0x22.222.11]", "[0:ffff:0:0:0:0:3f22:de0b]"},
    {"[0::ffff:077.0x22.222.11]", "63.34.222.11"},
    {"192.168.1.1", "192.168.1.1"},
    {"0x92.168.1.1", "146.168.1.1"},
    {"3279880203", "195.127.0.11"}
};


TEST(HostNormalizer__Test, testIpHostNormalizationAndGetBytes){
    for(auto it : HostPathMap){
        HostNormalizer hostNormalizer(it[0]);
        std::cout << hostNormalizer.getNormalizedHost() << ":" << it[1] << std::endl;
    }
}

int main(int argc,char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}