#include <gtest/gtest.h>
#include "charutils.h"

TEST(CharUtils__Test, testCharUtilsIsHex) {
    char arr[] = {'a', 'A', '0', '9'};
    for (char a : arr) {
        EXPECT_TRUE(CharUtils::isHex(a));
    }
    char arr2[] = {'~', ';', 'Z', 'g'};
    for (char a : arr2) {
        EXPECT_FALSE(CharUtils::isHex(a));
    }
}

TEST(CharUtils__Test, testCharUtilsIsNumeric) {
    char arr[] = {'0', '4', '6', '9'};
    for (char a : arr) {
        EXPECT_TRUE(CharUtils::isNumberic(a));
    }

    char arr2[] = {'a', '~', 'A', 0};
    for (char a : arr2) {
        EXPECT_FALSE(CharUtils::isNumberic(a));
    }
}

TEST(CharUtils__Test, testCharUtilsIsAlpha) {
    char arr[] = {'a', 'Z', 'f', 'X'};
    for (char a : arr) {
        EXPECT_TRUE(CharUtils::isAlpha(a));
    }

    char arr2[] = {'0', '9', '[', '~'};
    for (char a : arr2) {
        EXPECT_FALSE(CharUtils::isAlpha(a));
    }
}

TEST(CharUtils__Test, testCharUtilsIsAlphaNumeric) {
    char arr[] = {'a', 'G', '3', '9'};
    for (char a : arr) {
        EXPECT_TRUE(CharUtils::isAlphaNumberic(a));
    }

    char arr2[] = {'~', '-', '_', '\n'};
    for (char a : arr2) {
        EXPECT_FALSE(CharUtils::isAlphaNumberic(a));
    }
}

TEST(CharUtils__Test, testCharUtilsIsUnreserved) {
    char arr[] = {'-', '.', 'a', '9', 'Z', '_', 'f'};
    for (char a : arr) {
        EXPECT_TRUE(CharUtils::isUnreserved(a));
    }

    char arr2[] = {' ', '!', '(', '\n'};
    for (char a : arr2) {
        EXPECT_FALSE(CharUtils::isUnreserved(a));
    }
}
const std::vector<std::vector<std::string>> splitMap = {
    {"192.168.1.1","[192,168,1,1]"},
    {"..","[,,]"},
    {"asdf","[asdf]"},
    {"192.39%2e1%2E1","[192,39,1,1]"},
    {"192%2e168%2e1%2e1","[192,168,1,1]"},
    // {"as\uFF61awe.a3r23.lkajsf0ijr....","[as,awe,a3r23,lkajsf0ijr,,,,]"},
    {"%2e%2easdf","[,,asdf]"},
    {"sdoijf%2e","[sdoijf,]"},
    {"ksjdfh.asdfkj.we%2","[ksjdfh,asdfkj,we%2]"},
    {"0xc0%2e0x00%2e0x02%2e0xeb","[0xc0,0x00,0x02,0xeb]"},
    {"","[]"}
};

void runTest(const std::vector<std::string> result, const std::string &excepted){
    std::string temp = "[";
    for(size_t i = 0 ; i < result.size(); ++i){
        temp += result[i];
        if(i!= result.size()-1)
            temp += ",";
    }
    temp += "]";
    // std::cout << temp << "\n" << excepted << std::endl;
    EXPECT_EQ(temp,excepted);
}

TEST(CharUtils__Test, getSplitStrings) {
    for (auto s : splitMap) {
        std::vector<std::string> tmpList;
        CharUtils::splitByDot(tmpList, s[0]);
        runTest(tmpList, s[1]);
    }
}

TEST(StringUtils__Test, testJudgeValidBaseString){
    EXPECT_FALSE(CharUtils::JudgeValidRadixString("0123",1));
    EXPECT_FALSE(CharUtils::JudgeValidRadixString("0123",18));
    EXPECT_FALSE(CharUtils::JudgeValidRadixString("0123",3));
    EXPECT_TRUE(CharUtils::JudgeValidRadixString("0123",4));
    EXPECT_FALSE(CharUtils::JudgeValidRadixString("host",8));
    EXPECT_TRUE(CharUtils::JudgeValidRadixString("047",8));
    EXPECT_FALSE(CharUtils::JudgeValidRadixString("host",10));
    EXPECT_TRUE(CharUtils::JudgeValidRadixString("1234",10));
    EXPECT_FALSE(CharUtils::JudgeValidRadixString("host",10));
    EXPECT_FALSE(CharUtils::JudgeValidRadixString("host",16));
    EXPECT_TRUE(CharUtils::JudgeValidRadixString("44df",16));
    EXPECT_TRUE(CharUtils::JudgeValidRadixString("ffee",16));
}

// int main(int argc,char **argv){
//     testing::InitGoogleTest(&argc,argv);
//     return RUN_ALL_TESTS();
// }