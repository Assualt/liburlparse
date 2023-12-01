#include <gtest/gtest.h>
#include "url.h"
#include "url_detector.h"
#include "url_detect_options.h"
#include <iostream>
#include <list>
#include <set>

void runTest(
        const std::string &strText,
        UrlDetectorOptions nt,
        const std::set<std::string> &urlList,
        int nCnt = -1) {
    std::string tmpString = strText;
    UrlDetectorOptions_T t(nt);
    UrlDetector detect(tmpString, t);
    std::list<Url> urls = detect.detect();
    if (nCnt == -1)
        ASSERT_EQ(urls.size(), urlList.size());
    else
        ASSERT_EQ(urls.size(), nCnt);
    for (Url url : urls) {
        std::string urlText = url.getOriginalUrl();
        ASSERT_TRUE(urlList.find(urlText) != urlList.end());
    }
}

void runTest(const std::string &strText, UrlDetectorOptions nt) {
    std::string tmpString = strText;
    UrlDetectorOptions_T t(nt);
    UrlDetector detect(tmpString, t);
    std::list<Url> urls = detect.detect();
    ASSERT_EQ(urls.size(), 0);
}

TEST(UrlDetector__Test, testUrlPart) {
    UrlDetectorOptions_T T(Default);
    std::string str =
            "https://user:name@www.baidu.com:80/part.html?query=c++#part1";
    UrlDetector detect(str, T);
    std::list<Url> urls = detect.detect();

    ASSERT_EQ(urls.size(), 1);
    Url url = urls.front();

    ASSERT_EQ(
            url.getOriginalUrl(),
            "https://user:name@www.baidu.com:80/part.html?query=c++#part1");
    ASSERT_EQ(url.getScheme(), "https");
    ASSERT_EQ(url.getUsername(), "user");
    ASSERT_EQ(url.getPassword(), "name");
    ASSERT_EQ(url.getHost(), "www.baidu.com");
    ASSERT_EQ(url.getPort(), 80);
    ASSERT_EQ(url.getPath(), "/part.html");
    ASSERT_EQ(url.getQuery(), "?query=c++");
    ASSERT_EQ(url.getFragment(), "#part1");
}

TEST(UrlDetector__Test, testBasicString) {
    runTest("hello world", Default);
}

TEST(UrlDetector__Test, testBasicDetect) {
    runTest("this is a link: www.google.com", Default, {"www.google.com"});
}

TEST(UrlDetector__Test, testEmailAndNormalUrl) {
    runTest("my email is vshlosbe@linkedin.com and my site is "
            "http://www.linkedin.com/vshlos",
            Default,
            {"vshlosbe@linkedin.com", "http://www.linkedin.com/vshlos"});
}

TEST(UrlDetector__Test, testTwoBasicUrls) {
    runTest("the url google.com is a lot better then www.google.com.",
            Default,
            {"google.com", "www.google.com."});
}

TEST(UrlDetector__Test, testLongUrl) {
    runTest("google.com.google.com is kind of a valid url",
            Default,
            {"google.com.google.com"});
}

// Failed to path normalized
// TEST(UrlDetector__Test, testInternationalUrls){
//     runTest("this is an international domain: http://пример.испытание so is
//     this: 五礦發展.中國.", Default,
//     {"http://пример.испытание","五礦發展.中國."});
// }

TEST(UrlDetector__Test, testInternationalUrlsInHtml) {
    // runTest("<a rel=\"nofollow\" class=\"external text\"
    // href=\"http://xn--mgbh0fb.xn--kgbechtv/\">http://ᘅᕹᕵᘄ.ᕳᖂᕸᕶᕵᖅ</a>", HTML,
    // {"http://xn--mgbh0fb.xn--kgbechtv/","http://ᘅᕹᕵᘄ.ᕳᖂᕸᕶᕵᖅ"});
    runTest("<a rel=\"nofollow\" class=\"external text\" "
            "href=\"http://xn--mgbh0fb.xn--kgbechtv/\"></a>",
            HTML,
            {"http://xn--mgbh0fb.xn--kgbechtv/"});
}

TEST(UrlDetector__Test, testDomainWithUsernameAndPassword) {
    runTest("domain with username is "
            "http://username:password@www.google.com/site/1/2",
            Default,
            {"http://username:password@www.google.com/site/1/2"});
}

TEST(UrlDetector__Test, testFTPWithUsernameAndPassword) {
    runTest("ftp with username is ftp://username:password@www.google.com",
            Default,
            {"ftp://username:password@www.google.com"});
}

TEST(UrlDetector__Test, testUncommonFormatUsernameAndPassword) {
    runTest("weird url with username is username:password@www.google.com",
            Default,
            {"username:password@www.google.com"});
}

TEST(UrlDetector__Test, testEmailAndLinkWithUserPass) {
    runTest("email and username is hello@test.google.com or "
            "hello@www.google.com hello:password@www.google.com",
            Default,
            {"hello@test.google.com",
             "hello@www.google.com",
             "hello:password@www.google.com"});
}

TEST(UrlDetector__Test, testWrongSpacingInSentence) {
    runTest("I would not like to work at salesforce.com, it looks like a crap "
            "company.and not cool!",
            Default,
            {"salesforce.com", "company.and"});
}

TEST(UrlDetector__Test, testNumbersAreNotDetected) {
    // make sure pure numbers don't work, but domains with numbers do.
    runTest("Do numbers work? such as 3.1415 or 4.com", Default, {"4.com"});
}

TEST(UrlDetector__Test, testNewLinesAndTabsAreDelimiters) {
    runTest("Do newlines and tabs break? google.com/hello/\nworld "
            "www.yahoo.com\t/stuff/ yahoo.com/\thello "
            "news.ycombinator.com\u0000/hello world",
            Default,
            {"google.com/hello/",
             "www.yahoo.com",
             "yahoo.com/",
             "news.ycombinator.com"});
}

TEST(UrlDetector__Test, testIpAddressFormat) {
    runTest("How about IP addresses? fake: 1.1.1 1.1.1.1.1 0.0.0.256 "
            "255.255.255.256 real: 1.1.1.1 192.168.10.1 1.1.1.1.com "
            "255.255.255.255",
            Default,
            {"1.1.1.1", "192.168.10.1", "1.1.1.1.com", "255.255.255.255"});
}

TEST(UrlDetector__Test, testNumericIpAddress) {
    runTest("http://3232235521/helloworld",
            Default,
            {"http://3232235521/helloworld"});
}

TEST(UrlDetector__Test, testNumericIpAddressWithPort) {
    runTest("http://3232235521:8080/helloworld",
            Default,
            {"http://3232235521:8080/helloworld"});
}

TEST(UrlDetector__Test, testDomainAndLabelSizeConstraints) {
    // Really long addresses testing rules about total length of domain name and
    // number of labels in a domain and size of each label.
    runTest("This will work: "
            "1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3."
            "4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6."
            "7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9."
            "0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.a.b.c.d.e.ly This will "
            "not work:  "
            "1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3."
            "4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6."
            "7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9."
            "0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.a.b.c.d.e.f.ly This "
            "should as well: "
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa."
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb."
            "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc."
            "dddddddddddddddddddddddddddddddddddddddddddddddddddddd.bit.ly But "
            "this wont: "
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa."
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb."
            "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc."
            "dddddddddddddddddddddddddddddddddddddddddddddddddddddd.bit.ly.dbl."
            "spamhaus.org",
            Default,
            {"1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2."
             "3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4."
             "5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6."
             "7.8.9.0.1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.0.a.b.c.d.e.ly",
             "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa."
             "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb."
             "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc."
             "dddddddddddddddddddddddddddddddddddddddddddddddddddddd.bit.ly"});
}

TEST(UrlDetector__Test, testBasicHtml) {
    runTest("<script type=\"text/javascript\">var a = 'http://www.abc.com', "
            "b=\"www.def.com\"</script><a "
            "href=\"http://www.google.com\">google.com</a>",
            HTML,
            {"http://www.google.com",
             "http://www.abc.com",
             "www.def.com",
             "google.com"});
}

TEST(UrlDetector__Test, testLongUrlWithInheritedScheme) {
    runTest("<link rel=\"stylesheet\" "
            "href=\"//bits.wikimedia.org/en.wikipedia.org/"
            "load.php?debug=false&amp;lang=en&amp;modules=ext.gadget.DRN-"
            "wizard%2CReferenceTooltips%2Ccharinsert%2Cteahouse%7Cext."
            "wikihiero%7Cmediawiki.legacy.commonPrint%2Cshared%7Cmw."
            "PopUpMediaTransform%7Cskins.vector&amp;only=styles&amp;skin="
            "vector&amp;*\" />",
            HTML,
            {"//bits.wikimedia.org/en.wikipedia.org/"
             "load.php?debug=false&amp;lang=en&amp;modules=ext.gadget.DRN-"
             "wizard%2CReferenceTooltips%2Ccharinsert%2Cteahouse%7Cext."
             "wikihiero%7Cmediawiki.legacy.commonPrint%2Cshared%7Cmw."
             "PopUpMediaTransform%7Cskins.vector&amp;only=styles&amp;skin="
             "vector&amp;*"});
}

TEST(UrlDetector__Test, testQuoteMatching) {
    // test quote matching with no html
    runTest("my website is \"www.google.com\" but my email is "
            "\"vshlos@gmail.com\" \" www.abcd.com\" \" hello.com "
            "\"www.abc.com\"",
            QUOTE_MATCH,
            {"www.google.com",
             "vshlos@gmail.com",
             "www.abcd.com",
             "hello.com",
             "www.abc.com"});
}

TEST(UrlDetector__Test, testIncorrectParsingHtmlWithBadOptions) {
    runTest("<a href=\"http://www.google.com/\">google.com</a>",
            Default,
            // Doesn't have the http since it was read as "http:// and goes to
            // the end.
            {"www.google.com/\">google.com</a>"});
}

TEST(UrlDetector__Test, testBracketMatching) {
    runTest("MY url (www.google.com) is very cool. the domain [www.google.com] "
            "is popular and when written like this {www.google.com} it looks "
            "like code",
            BRACKET_MATCH,
            {"www.google.com", "www.google.com", "www.google.com"});
}

TEST(UrlDetector__Test, testParseJson) {
    runTest("{\"url\": \"www.google.com\", \"hello\": \"world\", "
            "\"anotherUrl\":\"http://www.yahoo.com\"}",
            JSON,
            {"www.google.com", "http://www.yahoo.com"});
}

TEST(UrlDetector__Test, testParseJavascript) {
    runTest("var url = 'www.abc.com';\nvar url = \"www.def.com\";",
            JAVASCRIPT,
            {"www.abc.com", "www.def.com"});
}

TEST(UrlDetector__Test, testParseXml) {
    runTest("<url attr=\"www.def.com\">www.abc.com</url><url "
            "href=\"hello.com\" />",
            XML,
            {"www.abc.com", "www.def.com", "hello.com"});
}

// TEST(UrlDetector__Test, testNonStandardDots)
// {
//     runTest(
//         "www\u3002google\u3002com username:password@www\uFF0Eyahoo\uFF0Ecom
//         http://www\uFF61facebook\uFF61com
//         http://192\u3002168\uFF0E0\uFF611/", Default,
//         {"www\u3002google\u3002com",
//         "username:password@www\uFF0Eyahoo\uFF0Ecom",
//         "http://www\uFF61facebook\uFF61com",
//          "http://192\u3002168\uFF0E0\uFF611/"});
// }

TEST(UrlDetector__Test, testInvalidPartsUrl) {
    // runTest("aksdhf http://asdf#asdf.google.com", Default,
    // {"asdf.google.com"}); //-->run failed
    runTest("00:41.<google.com/>", HTML, {"google.com/"});
}

// TEST(UrlDetector__Test, testNonStandardDotsBacktracking)
// {
//     runTest("\u9053 \u83dc\u3002\u3002\u3002\u3002", Default);
// }

TEST(UrlDetector__Test, testBacktrackingStrangeFormats) {
    runTest("http:http:http://www.google.com www.www:yahoo.com yahoo.com.br "
            "hello.hello..hello.com",
            Default,
            {"www.www",
             "hello.hello.",
             "http://www.google.com",
             "yahoo.com",
             "yahoo.com.br",
             "hello.com"});
}

TEST(UrlDetector__Test, testBacktrackingUsernamePassword) {
    runTest("check out my url:www.google.com", Default, {"www.google.com"});
    runTest("check out my url:www.google.com ", Default, {"www.google.com"});
}

TEST(UrlDetector__Test, testBacktrackingEmptyDomainName) {
    runTest("check out my http:///hello", Default);
    runTest("check out my http://./hello", Default);
}

TEST(UrlDetector__Test, testDoubleScheme) {
    runTest("http://http://", Default);
    runTest("hello http://http://", Default);
}

TEST(UrlDetector__Test, testMultipleSchemes) {
    runTest("http://http://www.google.com", Default, {"http://www.google.com"});
    runTest("make sure it's right here http://http://www.google.com",
            Default,
            {"http://www.google.com"});
    runTest("http://http://http://www.google.com",
            Default,
            {"http://www.google.com"});
    runTest("make sure it's right here http://http://http://www.google.com",
            Default,
            {"http://www.google.com"});
    runTest("http://ftp://https://www.google.com",
            Default,
            {"https://www.google.com"});
    runTest("make sure its right here http://ftp://https://www.google.com",
            Default,
            {"https://www.google.com"});
}

TEST(UrlDetector__Test, testDottedHexIpAddress) {
    runTest("http://0xc0.0x00.0xb2.0xEB",
            Default,
            {"http://0xc0.0x00.0xb2.0xEB"});
    runTest("http://0xc0.0x0.0xb2.0xEB",
            Default,
            {"http://0xc0.0x0.0xb2.0xEB"});
    runTest("http://0x000c0.0x00000.0xb2.0xEB",
            Default,
            {"http://0x000c0.0x00000.0xb2.0xEB"});
    runTest("http://0xc0.0x00.0xb2.0xEB/bobo",
            Default,
            {"http://0xc0.0x00.0xb2.0xEB/bobo"});
    runTest("ooh look i can find it in text http://0xc0.0x00.0xb2.0xEB/bobo "
            "like this",
            Default,
            {"http://0xc0.0x00.0xb2.0xEB/bobo"});
    runTest("noscheme look 0xc0.0x00.0xb2.0xEB/bobo",
            Default,
            {"0xc0.0x00.0xb2.0xEB/bobo"});
    runTest("no scheme 0xc0.0x00.0xb2.0xEB or path",
            Default,
            {"0xc0.0x00.0xb2.0xEB"});
}

TEST(UrlDetector__Test, testDottedOctalIpAddress) {
    runTest("http://0301.0250.0002.0353",
            Default,
            {"http://0301.0250.0002.0353"});
    runTest("http://0301.0250.0002.0353/bobo",
            Default,
            {"http://0301.0250.0002.0353/bobo"});
    runTest("http://192.168.017.015/", Default, {"http://192.168.017.015/"});
    runTest("ooh look i can find it in text http://0301.0250.0002.0353/bobo "
            "like this",
            Default,
            {"http://0301.0250.0002.0353/bobo"});
    runTest("noscheme look 0301.0250.0002.0353/bobo",
            Default,
            {"0301.0250.0002.0353/bobo"});
    runTest("no scheme 0301.0250.0002.0353 or path",
            Default,
            {"0301.0250.0002.0353"});
}

TEST(UrlDetector__Test, testHexIpAddress) {
    runTest("http://0xC00002EB/hello", Default, {"http://0xC00002EB/hello"});
    runTest("http://0xC00002EB.com/hello",
            Default,
            {"http://0xC00002EB.com/hello"});
    runTest("still look it up as a normal url http://0xC00002EXsB.com/hello",
            Default,
            {"http://0xC00002EXsB.com/hello"});
    runTest("ooh look i can find it in text http://0xC00002EB/bobo like this",
            Default,
            {"http://0xC00002EB/bobo"});
    runTest("browsers dont support this without a scheme look 0xC00002EB/bobo",
            Default);
}

TEST(UrlDetector__Test, testOctalIpAddress) {
    runTest("http://030000001353/bobobo",
            Default,
            {"http://030000001353/bobobo"});
    runTest("ooh look i can find it in text http://030000001353/bobo like this",
            Default,
            {"http://030000001353/bobo"});
    runTest("browsers dont support this without a scheme look "
            "030000001353/bobo",
            Default);
}

TEST(UrlDetector__Test, testUrlWithEmptyPort) {
    runTest("http://wtfismyip.com://foo.html",
            Default,
            {"http://wtfismyip.com://foo.html"});
    runTest("make sure its right here http://wtfismyip.com://foo.html",
            Default,
            {"http://wtfismyip.com://foo.html"});
}

TEST(UrlDetector__Test, testUrlEncodedDot) {
    runTest("hello www%2ewtfismyip%2ecom", Default, {"www%2ewtfismyip%2ecom"});
    runTest("hello wtfismyip%2ecom", Default, {"wtfismyip%2ecom"});
    runTest("http://wtfismyip%2ecom", Default, {"http://wtfismyip%2ecom"});
    runTest("make sure its right here http://wtfismyip%2ecom",
            Default,
            {"http://wtfismyip%2ecom"});
}

TEST(UrlDetector__Test, testUrlEncodedBadPath) {
    runTest("%2ewtfismyip", Default);
    runTest("wtfismyip%2e", Default);
    runTest("wtfismyip%2ecom%2e", Default, {"wtfismyip%2ecom%2e"});
    runTest("wtfismyip%2ecom.", Default, {"wtfismyip%2ecom."});
    runTest("%2ewtfismyip%2ecom", Default, {"wtfismyip%2ecom"});
}

TEST(UrlDetector__Test, testUrlEncodedColon) {
    runTest("http%3A//google.com", Default, {"http%3A//google.com"});
    runTest("hello http%3A//google.com", Default, {"http%3A//google.com"});
}

TEST(UrlDetector__Test, testIncompleteBracketSet) {
    runTest("[google.com", BRACKET_MATCH, {"google.com"});
    runTest("lalla [google.com", Default, {"google.com"});
}

TEST(UrlDetector__Test, testDetectUrlEncoded) {
    runTest("%77%77%77%2e%67%75%6d%62%6c%61%72%2e%63%6e",
            Default,
            {"%77%77%77%2e%67%75%6d%62%6c%61%72%2e%63%6e"});
    runTest(" asdf  %77%77%77%2e%67%75%6d%62%6c%61%72%2e%63%6e",
            Default,
            {"%77%77%77%2e%67%75%6d%62%6c%61%72%2e%63%6e"});
    runTest("%77%77%77%2e%67%75%6d%62%6c%61%72%2e%63%6e%2e",
            Default,
            {"%77%77%77%2e%67%75%6d%62%6c%61%72%2e%63%6e%2e"});
}

TEST(UrlDetector__Test, testSingleLevelDomain) {
    runTest("localhost:9000/lalala hehe",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"localhost:9000/lalala"});
    runTest("http://localhost lasdf",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"http://localhost"});
    runTest("localhost:9000/lalala",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"localhost:9000/lalala"});
    runTest("192.168.1.1/lalala",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"192.168.1.1/lalala"});
    runTest("http://localhost",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"http://localhost"});
    runTest("//localhost", ALLOW_SINGLE_LEVEL_DOMAIN, {"//localhost"});
    runTest("asf//localhost", ALLOW_SINGLE_LEVEL_DOMAIN, {"asf//localhost"});
    runTest("hello/", ALLOW_SINGLE_LEVEL_DOMAIN, {"hello/"});
    runTest("go/", ALLOW_SINGLE_LEVEL_DOMAIN, {"go/"});
    runTest("hello:password@go//",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"hello:password@go//"});
    runTest("hello:password@go",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"hello:password@go"});
    runTest("hello:password@go lala",
            ALLOW_SINGLE_LEVEL_DOMAIN,
            {"hello:password@go"});
    runTest("hello.com..", ALLOW_SINGLE_LEVEL_DOMAIN, {"hello.com."});
    runTest("a/", ALLOW_SINGLE_LEVEL_DOMAIN);
    runTest("asdflocalhost aksdjfhads", ALLOW_SINGLE_LEVEL_DOMAIN);
    runTest("/", ALLOW_SINGLE_LEVEL_DOMAIN);
    runTest("////", ALLOW_SINGLE_LEVEL_DOMAIN);
    runTest("hi:", ALLOW_SINGLE_LEVEL_DOMAIN);
    runTest("http://localhost", Default);
    runTest("localhost:9000/lalala", Default);
}

TEST(UrlDetector__Test, testIncompleteIpAddresses) {
    runTest("hello 10...", Default);
    runTest("hello 10...1", Default);
    runTest("hello 10..1.", Default);
    runTest("hello 10..1.1", Default);
    runTest("hello 10.1..1", Default);
    runTest("hello 10.1.1.", Default);
    runTest("hello .192..", Default);
    runTest("hello .192..1", Default);
    runTest("hello .192.1.", Default);
    runTest("hello .192.1.1", Default);
    runTest("hello ..3.", Default);
    runTest("hello ..3.1", Default);
    runTest("hello ...1", Default);
}

TEST(UrlDetector__Test, testIPv4EncodedDot) {
    runTest("hello 192%2e168%2e1%2e1", Default, {"192%2e168%2e1%2e1"});
    runTest("hello 192.168%2e1%2e1/lalala",
            Default,
            {"192.168%2e1%2e1/lalala"});
}

TEST(UrlDetector__Test, testIPv4HexEncodedDot) {
    runTest("hello 0xee%2e0xbb%2e0x1%2e0x1",
            Default,
            {"0xee%2e0xbb%2e0x1%2e0x1"});
    runTest("hello 0xee%2e0xbb.0x1%2e0x1/lalala",
            Default,
            {"0xee%2e0xbb.0x1%2e0x1/lalala"});
}

// //IPv6 Tests
TEST(UrlDetector__Test, testIPv6Colons) {
    std::vector<string> lis = {"[fe80:aaaa:aaaa:aaaa:3dd0:7f8e:57b7:34d5]",
                               "[bcad::aaaa:aaaa:3dd0:7f8e:222.168.1.1]",
                               "[bcad::aaaa:aaaa:3dd0:7f8e:57b7:34d5]",
                               "[dead::85a3:0:0:8a2e:370:7334]",
                               "[::BEEF:0:8a2e:370:7334]",
                               "[::beEE:EeEF:0:8a2e:370:7334]",
                               "[::]",
                               "[0::]",
                               "[::1]",
                               "[0::1]"};
    for (auto testString : lis) {
        runTest(testString, Default, {testString});
        runTest(" " + testString + " ", Default, {testString});
        runTest("bobo" + testString + " ", Default, {testString});
        runTest("bobo" + testString + "bobo", Default, {testString});
        runTest("bobo " + testString, Default, {testString});
        runTest("alkfs:afef:" + testString, Default, {testString});
    }
}
TEST(UrlDetector__Test, testIpv6BadUrls) {
    runTest("[fe80:aaaa:aaaa:aaaa:3dd0:7f8e:57b7:34d5f]", Default);
    runTest("[bcad::kkkk:aaaa:3dd0:7f8e:57b7:34d5]", Default);
    runTest("[:BAD:BEEF:0:8a2e:370:7334", Default);
    runTest("[:::]", Default);
    runTest("[lalala:we]", Default);
    runTest("[:0]", Default);
    runTest("[:0:]", Default);
    runTest("::]", Default);
    runTest("[:", Default);
    runTest("fe80:22:]3123:[adf]", Default);
    runTest("[][123[][ae][fae][de][:a][d]aef:E][f", Default);
    runTest("[]]]:d]", Default);
    runTest("[fe80:aaaa:aaaa:aaaa:3dd0:7f8e:57b7:34d5:addd:addd:adee]",
            Default);
    runTest("[][][]2[d][]][]]]:d][[[:d[e][aee:]af:", Default);
    runTest("[adf]", Default);
    runTest("[adf:]", Default);
    runTest("[adf:0]", Default);
    runTest("[:adf]", Default);
    runTest("[]", Default);
}

TEST(UrlDetector__Test, testIpv6BadWithGoodUrls) {
    runTest("[:::] [::] [bacd::]", Default, {"[::]", "[bacd::]"});
    runTest("[:0][::]", Default, {"[::]"});
    runTest("[:0:][::afaf]", Default, {"[::afaf]"});
    runTest("::] [fe80:aaaa:aaaa:aaaa::]",
            Default,
            {"[fe80:aaaa:aaaa:aaaa::]"});
    runTest("fe80:22:]3123:[adf] [fe80:aaaa:aaaa:aaaa::]",
            Default,
            {"[fe80:aaaa:aaaa:aaaa::]"});
    runTest("[][123[][ae][fae][de][:a][d]aef:E][f", Default);
    runTest("[][][]2[d][]][]]]:d][[[:d[e][aee:]af:", Default);
}

TEST(UrlDetector__Test, testIpv6BadWithGoodUrlsEmbedded) {
    runTest("[fe80:aaaa:aaaa:aaaa:[::]3dd0:7f8e:57b7:34d5f]",
            Default,
            {"[::]"});
    runTest("[b[::7f8e]:55]akjef[::]", Default, {"[::7f8e]:55", "[::]"});
    runTest("[bcad::kkkk:aaaa:3dd0[::7f8e]:57b7:34d5]akjef[::]",
            Default,
            {"[::7f8e]:57", "[::]"});
}

TEST(UrlDetector__Test, testIpv6BadWithGoodUrlsWeirder) {
    runTest("[:[::]", Default, {"[::]"});
    runTest("[:] [feed::]", Default, {"[feed::]"});
    runTest(":[::feee]:]", Default, {"[::feee]"});
    runTest(":[::feee]:]]", Default, {"[::feee]"});
    runTest("[[:[::feee]:]", Default, {"[::feee]"});
}

TEST(UrlDetector__Test, testIpv6ConsecutiveGoodUrls) {
    runTest("[::afaf][eaea::][::]", Default, {"[::afaf]", "[eaea::]", "[::]"});
    runTest("[::afaf]www.google.com", Default, {"[::afaf]", "www.google.com"});
    runTest("[lalala:we][::]", Default, {"[::]"});
    runTest("[::fe][::]", Default, {"[::fe]", "[::]"});
    runTest("[aaaa::][:0:][::afaf]", Default, {"[::afaf]", "[aaaa::]"});
}

TEST(UrlDetector__Test, testIpv6BacktrackingUsernamePassword) {
    runTest("check out my url:google.com", Default, {"google.com"});
    runTest("check out my url:[::BAD:DEAD:BEEF:2e80:0:0]",
            Default,
            {"[::BAD:DEAD:BEEF:2e80:0:0]"});
    runTest("check out my url:[::BAD:DEAD:BEEF:2e80:0:0] ",
            Default,
            {"[::BAD:DEAD:BEEF:2e80:0:0]"});
}

TEST(UrlDetector__Test, testIpv6BacktrackingEmptyDomainName) {
    runTest("check out my http:///[::2e80:0:0]", Default, {"[::2e80:0:0]"});
    runTest("check out my http://./[::2e80:0:0]", Default, {"[::2e80:0:0]"});
}

TEST(UrlDetector__Test, testIpv6DoubleSchemeWithDomain) {
    runTest("http://http://[::2e80:0:0]", Default, {"http://[::2e80:0:0]"});
    runTest("make sure its right here http://http://[::2e80:0:0]",
            Default,
            {"http://[::2e80:0:0]"});
}

TEST(UrlDetector__Test, testIpv6MultipleSchemes) {
    runTest("http://http://http://[::2e80:0:0]",
            Default,
            {"http://[::2e80:0:0]"});
    runTest("make sure its right here http://http://[::2e80:0:0]",
            Default,
            {"http://[::2e80:0:0]"});
    runTest("http://ftp://https://[::2e80:0:0]",
            Default,
            {"https://[::2e80:0:0]"});
    runTest("make sure its right here http://ftp://https://[::2e80:0:0]",
            Default,
            {"https://[::2e80:0:0]"});
}

TEST(UrlDetector__Test, testIpv6FtpWithUsernameAndPassword) {
    runTest("ftp with username is ftp://username:password@[::2e80:0:0]",
            Default,
            {"ftp://username:password@[::2e80:0:0]"});
}

TEST(UrlDetector__Test, testIpv6NewLinesAndTabsAreDelimiters) {
    runTest("Do newlines and tabs break? [::2e80:0:0]/hello/\nworld "
            "[::BEEF:ADD:BEEF]\t/stuff/ [AAbb:AAbb:AAbb::]/\thello "
            "[::2e80:0:0\u0000]/hello world",
            Default,
            {"[::2e80:0:0]/hello/",
             "[::BEEF:ADD:BEEF]",
             "[AAbb:AAbb:AAbb::]/"});
}

TEST(UrlDetector__Test, testIpv6WithPort) {
    runTest("http://[AAbb:AAbb:AAbb::]:8080/helloworld",
            Default,
            {"http://[AAbb:AAbb:AAbb::]:8080/helloworld"});
}

TEST(UrlDetector__Test, testIpv6BasicHtml) {
    runTest("<script type=\"text/javascript\">var a = '[AAbb:AAbb:AAbb::]', "
            "b=\"[::bbbb:]\"</script><a href=\"[::cccc:]\">[::ffff:]</a>",
            HTML,
            {"[AAbb:AAbb:AAbb::]", "[::bbbb:]", "[::cccc:]", "[::ffff:]"});
}

TEST(UrlDetector__Test, testIpv6LongUrlWithInheritedScheme) {
    runTest("<link rel=\"stylesheet\" "
            "href=\"//[AAbb:AAbb:AAbb::]/en.wikipedia.org/"
            "load.php?debug=false&amp;lang=en&amp;modules=ext.gadget.DRN-"
            "wizard%2CReferenceTooltips%2Ccharinsert%2Cteahouse%7Cext."
            "wikihiero%7Cmediawiki.legacy.commonPrint%2Cshared%7Cmw."
            "PopUpMediaTransform%7Cskins.vector&amp;only=styles&amp;skin="
            "vector&amp;*\" />",
            HTML,
            {"//[AAbb:AAbb:AAbb::]/en.wikipedia.org/"
             "load.php?debug=false&amp;lang=en&amp;modules=ext.gadget.DRN-"
             "wizard%2CReferenceTooltips%2Ccharinsert%2Cteahouse%7Cext."
             "wikihiero%7Cmediawiki.legacy.commonPrint%2Cshared%7Cmw."
             "PopUpMediaTransform%7Cskins.vector&amp;only=styles&amp;skin="
             "vector&amp;*"});
}

TEST(UrlDetector__Test, testIpv6QuoteMatching) {
    runTest("my website is \"[AAbb:AAbb:AAbb::]\" but my email is "
            "\"vshlos@[AAbb:AAbb:AAbb::]\" \" [::AAbb:]\" \" [::] "
            "\"www.abc.com\"",
            QUOTE_MATCH,
            {"[AAbb:AAbb:AAbb::]",
             "vshlos@[AAbb:AAbb:AAbb::]",
             "[::AAbb:]",
             "[::]",
             "www.abc.com"});
}

TEST(UrlDetector__Test, testIpv6IncorrectParsingHtmlWithBadOptions) {
    runTest("<a href=\"http://[::AAbb:]/\">google.com</a>",
            Default,
            // Doesn't have the http since it was read as "http:// and goes to
            // the end.
            {"[::AAbb:]/\">google.com</a>"});
}

TEST(UrlDetector__Test, testIpv6BracketMatching) {
    runTest("MY url ([::AAbb:] ) is very cool. the domain [[::ffff:]] is "
            "popular and when written like this {[::BBBe:]} it looks like code",
            BRACKET_MATCH,
            {"[::AAbb:]", "[::ffff:]", "[::BBBe:]"});
}

TEST(UrlDetector__Test, testIpv6EmptyPort) {
    runTest("http://[::AAbb:]://foo.html",
            Default,
            {"http://[::AAbb:]://foo.html"});
    runTest("make sure its right here http://[::AAbb:]://foo.html",
            Default,
            {"http://[::AAbb:]://foo.html"});
}

TEST(UrlDetector__Test, testIpv6UrlEncodedColon) {
    runTest("http%3A//[::AAbb:]", Default, {"http%3A//[::AAbb:]"});
    runTest("hello http%3A//[::AAbb:]", Default, {"http%3A//[::AAbb:]"});
}

std::vector<std::vector<std::string>> kMap = {
        {"[fe80:aaaa:aaaa:aaaa:3dd0:7f8e:192.168.1.1]",
         "[fe80:aaaa:aaaa:aaaa:3dd0:7f8e:192.168.1.1]"},
        {"[bcad::aaaa:aaaa:3dd0:7f8e:222.168.1.1]",
         "[bcad::aaaa:aaaa:3dd0:7f8e:222.168.1.1]"},
        {"[dead::85a3:0:0:8a2e:192.168.1.1]",
         "[dead::85a3:0:0:8a2e:192.168.1.1]"},
        {"[::BEEF:0:8a2e:192.168.1.1]", "[::BEEF:0:8a2e:192.168.1.1]"},
        {"[:BAD:BEEF:0:8a2e:192.168.1.1]", "192.168.1.1"},
        {"[::beEE:EeEF:0:8a2e:192.168.1.1]",
         "[::beEE:EeEF:0:8a2e:192.168.1.1]"},
        {"[::192.168.1.1]", "[::192.168.1.1]"},
        {"[0::192.168.1.1]", "[0::192.168.1.1]"},
        {"[::ffff:192.168.1.1]", "[::ffff:192.168.1.1]"},
        {"[0::ffff:192.168.1.1]", "[0::ffff:192.168.1.1]"},
        {"[0:ffff:192.168.1.1::]", "192.168.1.1"}};

TEST(UrlDetector__Test, testIPv6Ipv4Addresses) {
    for (auto testList : kMap) {
        runTest(testList[0], Default, {testList[1]});
        runTest(testList[0] + " ", Default, {testList[1]});
        runTest(" " + testList[0], Default, {testList[1]});
        runTest(" " + testList[0] + " ", Default, {testList[1]});
    }
}

std::vector<std::vector<std::string>> HexOctalIpAddresses = {
        {"http://[::ffff:0xC0.0x00.0x02.0xEB]", "%251"},
        {"http://[::0301.0250.0002.0353]", "%251"},
        {"http://[0::ffff:0xC0.0x00.0x02.0xEB]", "%223"},
        {"http://[0::0301.0250.0002.0353]", "%2lalal-a."},
        {"http://[::bad:ffff:0xC0.0x00.0x02.0xEB]", "%---"},
        {"http://[::bad:ffff:0301.0250.0002.0353]", "%-.-.-.-....-....--"}};

TEST(UrlDetector__Test, testIpv6HexOctalIpAddress) {
    for (auto testList : HexOctalIpAddresses) {
        runTest(testList[0], Default, {testList[0]});
        runTest(testList[1], Default);
    }
}

TEST(UrlDetector__Test, testBacktrackInvalidUsernamePassword) {
    runTest("http://hello:asdf.com", Default, {"asdf.com"});
}

