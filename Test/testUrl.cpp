#include <gtest/gtest.h>
#include "url.h"

#include <iostream>
#include <vector>
using namespace std;
const static std::vector<std::vector<std::string> > urlpart =
{
	{"http://www.google.com/", "www.google.com", "/", "", ""},
    {"nooooo:password@teeee.com", "teeee.com", "/", "nooooo", "password"},
    {"hello:ono@bob.com/lala.html", "bob.com", "/lala.html", "hello", "ono"},
    {"lala:asdfjdj1k@bob.com", "bob.com", "/", "lala", "asdfjdj1k"},
    {"sdf@bob.com", "bob.com", "/", "sdf", ""},
    // {"@www.google.com", "www.google.com", "/", "", ""},
    {"lalal:@www.gogo.com", "www.gogo.com", "/", "lalal", ""},
    {"nono:boo@[::1]", "[::1]", "/", "nono", "boo"},
    {"nono:boo@yahoo.com/@1234", "yahoo.com", "/@1234", "nono", "boo"}
};


TEST(Url__Test, testUsernamePasswordUrls){
	for(auto it: urlpart){
		string strurl = it[0];
		Url url = Url::create(strurl);
		// std::cout << url.getHost() << ":" << url.getPath() << ":" << url.getUsername() << ":" << url.getPassword() << std::endl;
		// std::cout << it[1] << ":" << it[2] << ":" << it[3] << ":" << it[4] << std::endl;
		EXPECT_EQ(url.getHost(),it[1]);
		EXPECT_EQ(url.getPath(),it[2]);
		EXPECT_EQ(url.getUsername(),it[3]);
		EXPECT_EQ(url.getPassword(),it[4]);
	}

}

const static std::vector<std::vector<std::string> > portUrls =
{
	{"http://www.google.com:820", "www.google.com", "/", "820"},
	{"foooo.coo:80", "foooo.coo", "/", "80"},
	{"[::ffff:192.168.1.1]:800", "[::ffff:192.168.1.1]", "/", "800"},
	{"[::1]:900/dodododo", "[::1]", "/dodododo", "900"},
	{"hdh:@[::1]:9/nono", "[::1]", "/nono", "9"},
	{"http://touch.www.linkedin.com:9000", "touch.www.linkedin.com", "/", "9000"}
};

TEST(Url__Test, testPort){
	for(auto it: portUrls){
		string strurl = it[0];
		Url url = Url::create(strurl);
		EXPECT_EQ(url.getHost(),it[1]);
		EXPECT_EQ(url.getPath(),it[2]);
		EXPECT_EQ(to_string(url.getPort()),it[3]);
	}
}

const static std::vector<std::vector<std::string> > querysUrls =
{
	{"http://www.google.com/", "www.google.com", "/", ""},
    {"www.google.com/lala?here=2", "www.google.com", "/lala", "?here=2"},
    {"bewp.bop.com/boop?bip=2&bep=3", "bewp.bop.com", "/boop", "?bip=2&bep=3"},
    {"[fe80::1:192.168.12.3]/nooo?dop=2&wop=4", "[fe80::1:192.168.12.3]", "/nooo", "?dop=2&wop=4"},
    {"[::1:192.1.1.1]:80/nooo?dop=[::1]&wop=4", "[::1:192.1.1.1]", "/nooo", "?dop=[::1]&wop=4"}
};

TEST(Url__Test, testQuery){
	for(auto it: querysUrls){
		string strurl = it[0];
		Url url = Url::create(strurl);
		EXPECT_EQ(url.getHost(),it[1]);
		EXPECT_EQ(url.getPath(),it[2]);
		EXPECT_EQ(url.getQuery(),it[3]);
	}
}
const static std::vector<std::vector<std::string> > schemeUrls =
{
	{"http://www.google.com/", "http", "www.google.com", "/"},
    {"//www.google.com/", "", "www.google.com", "/"},
    {"//123825342/", "", "123825342", "/"},
    {"//hello/", "", "hello", "/"},
    {"//hello:/", "", "hello", "/"}
};
TEST(Url__Test, testScheme){
	for(auto it: schemeUrls){
		string strurl = it[0];
		Url url = Url::create(strurl);
		EXPECT_EQ(url.getScheme(),it[1]);
		EXPECT_EQ(url.getHost(),it[2]);
		EXPECT_EQ(url.getPath(),it[3]);
	}
}

const static std::vector<std::vector<std::string> > UrlsAndHosts =
{
    {"www.booopp.com:20#fa", "www.booopp.com", "http://www.booopp.com:20/#fa"},
    {"www.yahooo.com:20?fff#aa", "www.yahooo.com", "http://www.yahooo.com:20/?fff#aa"},
    {"www.google.com#fa", "www.google.com", "http://www.google.com/#fa"},
    {"www.google.com?3fd#fa", "www.google.com", "http://www.google.com/?3fd#fa"},
    {"//www.google.com/", "www.google.com", "//www.google.com/"},
    {"http://www.google.com/", "www.google.com", "http://www.google.com/"},
    {"ftp://whosdere:me@google.com/", "google.com", "ftp://whosdere:me@google.com/"},
    {"ono:doope@fb.net:9090/dhdh", "fb.net", "http://ono:doope@fb.net:9090/dhdh"},
    {"ono:a@fboo.com:90/dhdh/@1234", "fboo.com", "http://ono:a@fboo.com:90/dhdh/@1234"},
    {"fbeoo.net:990/dhdeh/@1234", "fbeoo.net", "http://fbeoo.net:990/dhdeh/@1234"},
    {"fbeoo:@boop.com/dhdeh/@1234?aj=r", "boop.com", "http://fbeoo@boop.com/dhdeh/@1234?aj=r"},
    {"bloop:@noooo.com/doop/@1234", "noooo.com", "http://bloop@noooo.com/doop/@1234"},
    {"bah.com/lala/@1234/@dfd@df?@dsf#ono", "bah.com", "http://bah.com/lala/@1234/@dfd@df?@dsf#ono"},
    {"https://dewd:dood@www.google.com:20/?why=is&this=test#?@Sdsf", "www.google.com", "https://dewd:dood@www.google.com:20/?why=is&this=test#?@Sdsf"}
};

TEST(Url__Test, testHostAndFullUrl){
	for(auto it: UrlsAndHosts){
		string strurl = it[0];
		Url url = Url::create(strurl);
		EXPECT_EQ(url.getHost(),it[1]);
		EXPECT_EQ(url.getFullUrl(),it[2]);
		size_t index = it[2].find("#");
		EXPECT_EQ(url.getFullUrlWithoutFragment(), index == string::npos ? it[2] : it[2].substr(0, index));
	}
}

const static std::vector<std::vector<std::string> > SingleDomainUrls =
{
	{"localhost:9000/", "localhost", "9000", "http://localhost:9000/"},
    {"go/tj", "go", "80", "http://go/tj"}
};

TEST(Url__Test, testSingleDomainUrls){
	for(auto it: SingleDomainUrls){
		string strurl = it[0];
		Url url = Url::create(strurl);
		EXPECT_EQ(url.getHost(),it[1]);
		// EXPECT_EQ(to_string(url.getPort()),it[2]);
		EXPECT_EQ(url.getFullUrl(),it[3]);
	}
}

// int main(int argc,char **argv){
// 	testing::InitGoogleTest(&argc,argv);
//  	return RUN_ALL_TESTS();
// }