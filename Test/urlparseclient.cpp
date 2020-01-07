/*************************************************************************
        > File Name: urlparseclient.cpp
        > Author:
        > Mail:
        > Created Time: 2020年01月06日 星期一 13时58分02秒
 ************************************************************************/

#include <iostream>
#include <fstream>
#include "url.h"
#include <list>
#include "urldetectoroptions.h"
#include "urldetector.h"
using namespace std;
string readFromFile(const std::string &filepath) {
    std::ifstream fin(filepath.c_str());
    if (!fin.is_open())
        return "";

    fin.seekg(0, std::ios_base::end);
    off_t length = fin.tellg();
    fin.seekg(0, std::ios_base::beg);
    char *temp = new char[length + 1];
    fin.read(temp, length);
    fin.close();
    std::string tmp = temp;
    delete temp;
    std::cout << "file length allocated " << length << std::endl;
    return tmp;
}

void Usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "-s" << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        Usage();
        return 0;
    }
    string readString = readFromFile(argv[1]);
    UrlDetectorOptions_T t(HTML);
    UrlDetector detect(readString, t);

    std::list<Url> urls = detect.detect();
    int i = 0;
    for (Url url : urls) {
        std::cout << i++ << "th ->" << url.getOriginalUrl() << std::endl;
    }
    return 0;
}
