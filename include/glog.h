#ifndef _GLOG_H
#define _GLOG_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <memory>
#include <fstream>
#include "unistd.h"
#include "assert.h"
using namespace std;

const std::string LOG_PATH = "/tmp/urlparse.log";

//定义日志级别 分别是 DEBUG WARN ERROR
enum LOG_LEVEL { LOG_DEBUG_LEVEL = 3, LOG_WARN_LEVEL = 2, LOG_ERR_LEVEL = 1 };

//定义时间buf
#define TimeBuf_MAXSIZE 32
#define DateBuf_MAXSIZE 20
#define MAX_TIME_FORMAT_SIZE 50
const static char *timeformat = "%04d-%02d-%02d %02d:%02d:%02d.%06ld";
#include <sys/time.h>
static std::string getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char tempValue[MAX_TIME_FORMAT_SIZE];
    time_t tNow = time(NULL);
    struct tm *t  = localtime(&tNow);
    memset(tempValue, 0 , MAX_TIME_FORMAT_SIZE);
    sprintf(tempValue, timeformat, (t->tm_year+1900),
            (t->tm_mon+1), (t->tm_mday), (t->tm_hour) , (t->tm_min),
            (t->tm_sec), tv.tv_usec);
    return tempValue;
}

class Logger{
private:
    Logger(){}
    ~Logger(){
        _outlogger.close();
    }
public:
    static Logger & getLogger() {
        std::ofstream &out = _logger.getOutStream();
        if(!out.is_open()){ //try to open
            out.open(LOG_PATH);
            if(!out.is_open()){
                std::cout << "open Failed " << LOG_PATH << std::endl;
            }
        }
        return _logger;
    }
    static Logger _logger;
    
    void outStream(const std::string &data) {
        _outlogger << data << std::endl;
    }
    ofstream &getOutStream() { return _outlogger; }
private:
    std::ofstream _outlogger;
};
class Log {
public:
    Log();
    Log &log();
    Log &setLevel(LOG_LEVEL level);
    Log &format(const std::string &fmt_str, ...);

    const std::string getLevel();
    void toFile();
    static Logger _logger;
private:
    std::string _currenttime;
    LOG_LEVEL _level;
    std::string _log_data;
};

#endif
