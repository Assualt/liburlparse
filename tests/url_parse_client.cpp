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
#include <filesystem>
#include "url_detect_options.h"
#include "url_detector.h"

namespace fs = std::filesystem;
std::string readFromFile(const std::string &filepath) {
    off_t size = fs::file_size(filepath);
    std::ifstream fin(filepath);
    if (!fin.is_open()) {
        return "";
    }

    auto data = std::make_unique<char[]>(size);
    fin.read(data.get(), size);

    return data.get();
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
    if (readString.size() > 100000) {
        std::cout << "over flow " << std::endl;
        return 0;
    }

    UrlDetectorOptions_T t(HTML);

    UrlDetector detect(readString, t);

    std::list<Url> urls = detect.detect();
    int i = 0;
    for (Url url : urls) { std::cout << i++ << "th ->" << url.getOriginalUrl() << std::endl; }
    return 0;
}
