#ifndef _GLOG_H
#define _GLOG_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <memory>
#include <fstream>
#include "unistd.h"
#include "time.h"
#include "assert.h"
using namespace std;

const string LOG_PATH = "/home/coremail/logs/urlParser.log";

//定义日志级别 分别是 DEBUG WARN ERROR
enum LOG_LEVEL
{
	LOG_DEBUG_LEVEL = 3,
	LOG_WARN_LEVEL = 2,
	LOG_ERR_LEVEL =	1
};

//定义时间buf
#define TimeBuf_MAXSIZE 32
#define DateBuf_MAXSIZE 20

class Log
{
public:
	Log();
	Log& log();
	Log& setLevel(LOG_LEVEL level);
	Log& format(const std::string &fmt_str, ...);
	 
	const string getLevel();
	void toFile();
private:
	void getTime();
	void getDate();
	string _time;
	LOG_LEVEL _level;
	string _log_data;	
	string _LibUrlParser;
	string _date;
};

#endif
