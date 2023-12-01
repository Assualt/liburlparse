#include <Python.h>
#include "url.h"
#include "url_detector.h"
#include <list>
#include <vector>
using namespace std;

static vector<std::string> ParseUrlFromText(const std::string &urlText, const int Operation = 0)
{
    UrlDetectorOptions_T tempOperation(static_cast<UrlDetectorOptions>(Operation));
    UrlDetector parse(urlText, tempOperation);
    std::list<Url> urlList = parse.detect();
    std::vector<std::string> urlVector;
    for (auto url : urlList)        
        urlVector.push_back(url.getOriginalUrl());
    return urlVector;
}

PyObject *parse_url(PyObject *self, PyObject *args)
{
    char *urlText;
    int urlArg;
    if (!PyArg_ParseTuple(args, "si", &urlText, &urlArg)){
        return NULL;
    }
    
    std::vector<std::string> result = ParseUrlFromText(urlText, urlArg);
    if(result.size() == 0)
        return Py_BuildValue(""); // None
    PyObject *pTuple =  PyTuple_New(result.size());
    for(size_t i = 0; i < result.size(); ++i){
        PyTuple_SetItem(pTuple, i, Py_BuildValue("s", result[i].c_str()));
    }
    return pTuple;
}

static PyMethodDef test_methods[] = {
    {"parse_url_from_text", parse_url, METH_VARARGS, "something"},
    {NULL,NULL, 0, NULL}
};

extern "C"
void initurlparse()
{
    Py_InitModule("urlparse", test_methods);
}

//g++ -std=c++11 -Wall -Wno-unused-function -Wno-deprecated-declarations
// -Wno-uninitialized -fPIC -shared urlparse.cc url_detect/*.cpp 
// -I./url_detect/include -I/usr/include/python2.7 -o urlparse.so