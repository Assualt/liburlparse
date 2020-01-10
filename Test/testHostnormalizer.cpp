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
    // {"[0::ffff:077.0x22.222.11]", "63.34.222.11"}, // -->failed
    {"192.168.1.1", "192.168.1.1"},
    {"0x92.168.1.1", "146.168.1.1"},
    {"3279880203", "195.127.0.11"}
};


TEST(HostNormalizer__Test, testIpHostNormalizationAndGetBytes){
    for(auto it : HostPathMap){
        HostNormalizer hostNormalizer(it[0]);
        EXPECT_EQ(hostNormalizer.getNormalizedHost(), it[1]);
    }
}

static const std::vector<std::vector<std::string>> 
HostFormatMap = {
    {"sALes.com","sales.com"},
    {"33r.nEt","33r.net"},
    {"173839.com","173839.com"},
    {"192.168.-3.1","192.168.-3.1"},
    {"[::-34:50]","[::-34:50]"},
    {"[-34::192.168.34.-3]","[-34::192.168.34.-3]"}
};

TEST(HostNormalizer__Test, testFormatHost){
    for(auto it: HostFormatMap){
        HostNormalizer hostNormalizer(it[0]);
        EXPECT_EQ(hostNormalizer.getNormalizedHost(), it[1]);
        EXPECT_TRUE(hostNormalizer.getBytes().empty());

    }
}
// int main(int argc,char **argv){
//     testing::InitGoogleTest(&argc,argv);
//     return RUN_ALL_TESTS();
// }
