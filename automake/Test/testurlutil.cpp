#include <gtest/gtest.h>
#include "urlutil.h"
#include <vector>
#include <iostream>
using namespace std;

static const std::vector<std::vector<std::string>> DecodeMap = {
    {"%%32%35", "%"},
    {"%2%35", "%"},
    {"%%325", "%"},
    {"%%32%3525", "%"},
    {"%%%32%35", "%%"},
    {"%%32%35%", "%%"},
    {"%%32%3532", "2"},
    {"%%%32%3532%%32%3535", "%"},
    {"/%25%32%35", "/%"},
    {"/%2%2%2", "/%2%2%2"},
    {"/%2%%335", "/%"},
    {"/%25%32%35%25%32%35", "/%%"},
    {"/%2525252525252525", "/%"},
    {"/asdf%25%32%35asd", "/asdf%asd"},
    {"/%%%25%32%35asd%%", "/%%%asd%%"},
    {"/%2E%73%65%63%75%72%65/%77%77%77%2E%65%62%61%79%2E%63%6F%6D/",
     "/.secure/www.ebay.com/"},
    {"/uploads/%20%20%20%20/", "/uploads/    /"},
    {"/%257Ea%2521b%2540c%2523d%2524e%25f%255E00%252611%252A22%252833%"
     "252944_55%252B",
    "/~a!b@c#d$e%f^00&11*22(33)44_55+"}};

TEST(UrlUtil__Test, testDecode) {
    for (auto it : DecodeMap) {
        EXPECT_EQ(UrlUtil::decode(it[0]), it[1]);
    }
}

static const std::vector<std::vector<std::string>> EncodeMap = {
    {"/lnjbk%", "/lnjbk%25"},
    {"/%2%2%2", "/%252%252%252"}};

TEST(UrlUtil__Test, testEncode) {
    for (auto it : EncodeMap) {
        EXPECT_EQ(UrlUtil::encode(it[0]), it[1]);
    }
}

static const std::vector<std::vector<std::string>> ExtraDotsStrings = {
    {".s..ales.....com", "s.ales.com"},
    {"33r.nEt...", "33r.nEt"},
    {"[::-34:50]...", "[::-34:50]"},
    {"asdf.[-34::192.168.34.-3]...", "asdf.[-34::192.168.34.-3]"},
    {".", ""}};

TEST(UrlUtil__Test, testExtraDotsHosts) {
    for (auto it : ExtraDotsStrings) {
        EXPECT_EQ(UrlUtil::removeExtraDots(it[0]), it[1]);
    }
}

// int main(int argc,char **argv){
//     testing::InitGoogleTest(&argc,argv);
//     return RUN_ALL_TESTS();
// }
