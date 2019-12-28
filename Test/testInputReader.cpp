#include <gtest/gtest.h>
#include "inputtextreader.h"
const static std::string Content = "HELLO WORLD";

TEST(InputTextReader___Test, testSimpleRead)
{
    std::string tmpText = Content;
    InputTextReader reader(tmpText);
    for (size_t i = 0; i < tmpText.size(); i++)
    {
        EXPECT_EQ(reader.read(), tmpText.at(i));
    }
}

TEST(InputTextReader___Test, testEOF)
{
    std::string tmpText = Content;
    InputTextReader reader(tmpText);
    for (size_t i = 0; i < tmpText.size() - 1; i++)
        reader.read();
    ASSERT_FALSE(reader.eof());
    reader.read();
    ASSERT_TRUE(reader.eof());
}
TEST(InputTextReader___Test, testGoBack)
{
    std::string tmpText = Content;
    InputTextReader reader(tmpText);
    EXPECT_EQ(reader.read(), tmpText.at(0));
    reader.goBack();
    EXPECT_EQ(reader.read(), tmpText.at(0));
    EXPECT_EQ(reader.read(), tmpText.at(1));
    EXPECT_EQ(reader.read(), tmpText.at(2));
    reader.goBack();
    reader.goBack();
    EXPECT_EQ(reader.read(), tmpText.at(1));
    EXPECT_EQ(reader.read(), tmpText.at(2));
}
TEST(InputTextReader___Test, testSeek)
{
    std::string tmpText = Content;
    InputTextReader reader(tmpText);
    reader.seek(4);
    EXPECT_EQ(reader.read(), tmpText.at(4));

    reader.seek(1);
    EXPECT_EQ(reader.read(), tmpText.at(1));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}