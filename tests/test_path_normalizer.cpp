#include <gtest/gtest.h>
#include <iostream>
#include "path_normalizer.h"
#include <vector>

const static std::vector<std::vector<std::string>> pathData = {
	{"/%25%32%35", "/%25"},
	{"/%2%2%2", "/%252%252%252"},
	{"/%2%%335", "/%25"},
	{"/%25%32%35%25%32%35", "/%25%25"},
	{"/%2525252525252525", "/%25"},
	{"/asdf%25%32%35asd", "/asdf%25asd"},
	{"/%%%25%32%35asd%%", "/%25%25%25asd%25%25"},
	{"/%2E%73%65%63%75%72%65/%77%77%77%2E%65%62%61%79%2E%63%6F%6D/", "/.secure/www.ebay.com/"},
	{"/uploads/%20%20%20%20/.verify/.eBaysecure=updateuserdataxplimnbqmn-xplmvalidateinfoswqpcmlx=hgplmcx/",
	    "/uploads/%20%20%20%20/.verify/.eBaysecure=updateuserdataxplimnbqmn-xplmvalidateinfoswqpcmlx=hgplmcx/"},
	{"/%257Ea%2521b%2540c%2523d%2524e%25f%255E00%252611%252A22%252833%252944_55%252B",
	    "/~a!b@c%23d$e%25f^00&11*22(33)44_55+"},
	{"/lala/.././../..../", "/..../"}, 
	{"//asdfasdf/awef/sadf/sdf//", "/asdfasdf/awef/sadf/sdf/"},
	{"/", "/"},
	{"/a/../b/c", "/b/c"},
	{"/blah/..", "/"},
	{"../", "../"},
	{"/asdf/.", "/asdf/"},
	{"/a/b/./././././../c/d", "/a/c/d"},
	{"/a/b//////.///././././../c/d", "/a/c/d"},
	{"//../a/c/..///sdf", "/a/sdf"},
	{"/../asdf", "/asdf"},
	{"/../asdf/", "/asdf/"},
	{"/a/b/..c", "/a/b/..c"},
	{"/a/b/.././", "/a/"},
	{"/a/b/./", "/a/b/"},
	{"/a/b/../..", "/"},
	{"/a/b/../../../../../../", "/"},
	{"/a/b/../../../../../..", "/"},
	{"/a/b/../../../../../../c/d", "/c/d"},
	{"/a/b/../../../../../../c/d/", "/c/d/"},
	{"/a/b/../.", "/a/"},
	{"/a/b/..", "/a/"},
	{"/1.html", "/1.html"},
	{"/1/2.html?param=1", "/1/2.html?param=1"},
	{"/a./b.", "/a./b."},
	{"/a./b./", "/a./b./"}
};

TEST(PathNormalizer__Test, testPaths){
	for(auto path : pathData){
		PathNormalizer pathNormalizer;
		EXPECT_EQ(pathNormalizer.normalizePath(path[0]), path[1]);
	}
}
#ifdef TEST_MAIN_ONLY
int main(int argc,char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
