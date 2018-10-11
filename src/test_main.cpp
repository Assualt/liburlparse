/*************************************************************************
	> File Name: test_main.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 15 Aug 2018 10:05:11 PM CST
 ************************************************************************/

#include  <iostream>
#include "urldetector.h"

#include <fstream>
#include "time.h"
#include <list>
#include <cstring>
#include <assert.h>
#include <unistd.h>
using namespace std;

std::string toString(string &log)
{
    if (log.empty()) {
        return "";
    }
    string test = log.substr(log.find('['));
    string temp = StringUtils::replaceAlls(test, "\\", "");
    return StringUtils::replaceAlls(temp,"\\r\\n:", "");
}

void parserUrlToFile(string &filePath,string &destfilePath)
{
    clock_t tstart,tend;
    double timeCnt= 0.0;
    std::ifstream fin(filePath.c_str());
    std::ofstream fout(destfilePath.c_str());
    string line, linetemp;
    assert(fin.is_open());
    assert(fout.is_open());
    int lineNum = 0;
    int urlcount = 0;
    fout << "Url Parser to file begin!" << std::endl;
    while (getline(fin, line))
    {
        tstart = clock();

        /*
        if (lineNum == 100) {
            break;
        }
        */
        std::cout << "-----------------------------------------------------" << std::endl;
        lineNum++;
        linetemp = toString(line);
        std::cout << lineNum << "th line. linetemp:" << linetemp << ". size:" << linetemp.size() << std::endl;
        std::cout << "-----------------------------------------------------" << std::endl;
       UrlDetectorOptions_T sfault(Default);
        UrlDetector *parser = new UrlDetector(linetemp,sfault);
        std::cout << "detected begin()" << std::endl;
        list<Url> urls = parser->detect();
        delete(parser);
        std::cout << "detected end()" << std::endl;
        
        for (Url url : urls) {
            fout << "The " << urlcount << "th Url:" << url.getOriginalUrl() << std::endl;
        }
        
        std::cout << "-----------------------------------------------------" << std::endl;
        tend = clock();
        timeCnt+= tend-tstart;
        std::cout<<"cost time :"<<(double)(tend-tstart)/CLOCKS_PER_SEC<<"s"<<std::endl;
        std::cout << "-----------------------------------------------------" << std::endl;
        //sleep(1);
        
    }
    fin.close();
    std::cout << "-----------------------------------------------------" << std::endl;
    fout << "The Url parser end. Url count is :" << std::endl;
    fout.close();
    std::cout << "url count:" << urlcount <<" time:"<<(double)(1000*timeCnt/CLOCKS_PER_SEC)<<"ms"<< std::endl;
    std::cout << "average:"<< (double)(timeCnt/CLOCKS_PER_SEC/lineNum)<<"s"<<std::endl;
}

int main(int argc,char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./a.out  <srcfilename> <destfilename>" << std::endl;
        return -1;
    }
    string arg1 = argv[1];
    string arg2 = argv[2];
    parserUrlToFile(arg1,arg2);

    /*
    string test = "https://user:pass@www.baidu.com:8080/parts?quert=c++#pars";

    UrlDetectorOptions_T test1(Default);
    UrlDetector *parser = new UrlDetector(test,test1);
    list<Url> urls = parser->detect();
    delete(parser);
    for(Url url:urls)
    {
        std::cout<<"url:"<<url.getOriginalUrl()<<std::endl;
    }
    */
    return 0;
}

