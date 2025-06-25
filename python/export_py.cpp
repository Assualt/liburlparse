#include <pybind11/stl.h>
#include "url.h"
#include "url_detector.h"

namespace py = pybind11;

std::vector<std::string> parse_url(const std::string& urlText) {
    UrlDetectorOptions_T tempOperation(UrlDetectorOptions::Default);
    UrlDetector parse(urlText, tempOperation);
    std::list<Url> urlList = parse.detect();
    std::vector<std::string> urlVector;
    for (auto url : urlList) urlVector.push_back(url.getOriginalUrl());
    return urlVector;
}

PYBIND11_MODULE(pyurlparse, m) {
    m.def(
            "add", [](int i, int j) { return i + j; }, "A function which adds two numbers");
    m.def(
            "sub", [](int i, int j) { return i - j; }, "A function which subtracts two numbers");
    m.def("parse_url", &parse_url, "A function which parses a URL");
}