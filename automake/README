# liburlparse

[![Build Status](./urlparse.svg)](https://github.com/Assualt/liburlparse)

## Description

This is a lib which can parse more urls from string text and the lib is overwritted by c++ and  referenced by [linkedin/URL-Detector](https://github.com/linkedin/URL-Detector) 

### It is able to find and detect any urls such as:
* __HTML 5 Scheme__   - //www.linkedin.com
* __Usernames__       - user:pass@linkedin.com
* __Email__           - fred@linkedin.com
* __IPv4 Address__    - 192.168.1.1/hello.html
* __IPv4 Octets__     - 0x00.0x00.0x00.0x00
* __IPv4 Decimal__    - http://123123123123/
* __IPv6 Address__    - ftp://[::]/hello
* __IPv4-mapped IPv6 Address__  - http://[fe30:4:3:0:192.3.2.1]/

It is also able to identify the parts of the identified urls. For example, for the url: `http://user@linkedin.com:39000/hello?boo=ff#frag`

* Scheme   - "http"
* Username - "user"
* Password - null
* Host     - "linkedin.com"
* Port     - 39000
* Path     - "/hello"
* Query    - "?boo=ff"
* Fragment - "#frag"

## Feature

* The lib of Urlparse is only implemented by C++11
* The lib of UrlParse Support the many of Mode
    - Default: like `this is a url test@qq.com` 
    - Html   : like `<html><body>xxx@xxx.com</body></html>` 
    - Json   : like `{abc:123,ccc:"xhou@urlparse.com"}` 
    - Xml    : like `<a><b>Test</b></a>` 
    - JavaScript: like `<script>var location="www.baidu.com";</script>` 

 + The lib of UrlParse is 

## Author

### Contact with [xhou](mailto:13752913198@163.com)

## How to build

- make && make main

## Example

* We Can Parse Url from Text

``` c
UrlDetectorOptions_T T(Default);
std::string str = "https://user:name@www.baidu.com:80/part.html?query=c+#part1";
UrlDetector detect(str, T);
std::list<Url> urls = detect.detect();
for (Url url : urls)
{
    std::cout << "url:" + url.getOriginalUrl() << std::endl;     //获取Url所有录 一般得到这个url全值即可
    std::cout << "scheme:" << url.getScheme() << std::endl;      //获取协议头
    std::cout << "username:" << url.getUsername() << std::endl;  //获取用户名
    std::cout << "password:" << url.getPassword() << std::endl;  //获取密码
    std::cout << "host:" << url.getHost() << std::endl;          //获取主机地址
    std::cout << "port:" << url.getPort() << std::endl;          //获取端口号
    std::cout << "path:" << url.getPath() << std::endl;          //获取路径
    std::cout << "query:" << url.getQuery() << std::endl;        //获取查询参数
    std::cout << "fragment:" << url.getFragment() << std::endl;  //获取fragment
}
```
### 生成单元测试覆盖度
+ 1. make 添加参数 -fprofile-arcs -ftest-coverage 生成 gcno文件
+ 2. 执行 生成 gcda文件
+ 3. lcov -d . -t 'unitmain' -o 'unitmain.info' -b . -c 生成 unitmain.info文件
+ 4. genhtml -o result hello_test.info   
+ 5. python3 -m http.server 8080 


