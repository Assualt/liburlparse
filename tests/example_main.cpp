#include "url.h"
#include "url_detector.h"
#include "url_detect_options.h"

#include <iostream>
#include <list>
using namespace std;

int main(int argc, char **argv) {
    UrlDetectorOptions_T T(Default);

    // std::string str =
    // "\"https://user:name@www.baidu.com:80/part.html?query=c++#part1";
    std::string str = "hello www%2ewtfismyip%2ecom";
    UrlDetector detect(str, T);
    std::list<Url> urls = detect.detect();

    for (Url url : urls) {
        std::cout << "url:" + url.getOriginalUrl() << std::endl;  //获取Url所有目录 一般得到这个url全值即可
        std::cout << "scheme:" << url.getScheme() << std::endl;  //获取协议头
        std::cout << "username:" << url.getUsername() << std::endl;  //获取用户名
        std::cout << "password:" << url.getPassword() << std::endl;  //获取密码
        std::cout << "host:" << url.getHost() << std::endl;  //获取主机地址
        std::cout << "port:" << url.getPort() << std::endl;  //获取端口号
        std::cout << "path:" << url.getPath() << std::endl;  //获取路径
        std::cout << "query:" << url.getQuery() << std::endl;  //获取查询参数
        std::cout << "fragment:" << url.getFragment() << std::endl;  //获取fragment
    }
    //Normalized Url 
    string u = "http://\x01\x80.com/";
    NormalizedUrl url = NormalizedUrl::create(u);
    std::cout << url.getHost() << std::endl;



    return 0;
}
