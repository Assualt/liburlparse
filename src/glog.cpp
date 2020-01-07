#include "glog.h"

Log::Log() : _level(LOG_DEBUG_LEVEL), _LibUrlParser("URL Parser") {}
void Log::getTime() {
    time_t localtimer;
    time(&localtimer);
    char buf[TimeBuf_MAXSIZE];
    memset(buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "(%H:%M:%S)", localtime(&localtimer));
    _time = buf;
    memset(buf, 0, sizeof(buf));
}
void Log::getDate() {
    time_t localDate;
    time(&localDate);
    char buf[DateBuf_MAXSIZE];
    memset(buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&localDate));
    _date = buf;
    memset(buf, 0, sizeof(buf));
}
Log &Log::setLevel(LOG_LEVEL level) {
    _level = level;
    return *this;
}
Log &Log::log() {
    getTime();
    getDate();
    return *this;
}
Log &Log::format(const std::string &fmt_str, ...) {
    int n = ((int)fmt_str.size()) *
            2; /* Reserve two times as much as the length of the fmt_str */
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while (1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the
                                         unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        int final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    _log_data = std::string(formatted.get());
    return *this;
}
const string Log::getLevel() {
    string t;
    switch (_level) {
    case LOG_DEBUG_LEVEL:
        t = "DEBUG";
        break;
    case LOG_WARN_LEVEL:
        t = "WARNNING";
        break;
    case LOG_ERR_LEVEL:
        t = "ERROR";
        break;
    default:
        t = "DEBUG";
        break;
    }
    return t;
}
void Log::toFile() {
    string log = _LibUrlParser + " " + _time + " [root - :" + this->getLevel() +
            "] " + _log_data;
    string filename = LOG_PATH + "." + _date;
    std::ofstream fout(filename, ios::app);
    fout << log << std::endl;
    fout.close();
}
/*
int main()
{
        int a = 10086, b = 10010;
        Log glog;
        int i = 10;
        while (i--)
        {
                Log().log().setLevel(LOG_DEBUG_LEVEL).format("%d-%d-%s", a,
b,"this is just a beginning to loops").toFile(); sleep(1);
        }
        // std::cout<<"TimeFormat:"<<TimeFormat()<<std::endl;
        // sleep(1);
        // std::cout<<"TimeFormat:"<<TimeFormat()<<std::endl;
        return 0;
}
*/
