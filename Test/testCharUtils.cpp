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

TEST(CharUtils__Test, getSplitStrings) {
    std::vector<std::string> Ret = {{"192.168.1.1"},
                                    {".."},
                                    {"192%2e168%2e1%2e1"},
                                    {"asdf"},
                                    {"192.39%2e1%2E1"},
                                    {"as\uFF61awe.a3r23.lkajsf0ijr...."},
                                    {"%2e%2easdf"},
                                    {"sdoijf%2e"},
                                    {"ksjdfh.asdfkj.we%2"},
                                    {"0xc0%2e0x00%2e0x02%2e0xeb"},
                                    {""}};
    for (auto s : Ret) {
        std::list<std::string> tmpList;
        CharUtils::splitByDot(tmpList, s);
        for (auto t : tmpList) {
            std::cout << t << "\t";
        }
        std::cout << std::endl;
    }
}
/*
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/
