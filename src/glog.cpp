#include "glog.h"

Log::Log() : _level(LOG_DEBUG_LEVEL) {}
Log &Log::setLevel(LOG_LEVEL level) {
    _level = level;
    return *this;
}
Log &Log::log() {
    _currenttime = getCurrentTime();
    return *this;
}
Log &Log::format(const std::string &fmt_str, ...) {
    int n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while (true) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the
                                         unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str.c_str());
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
std::string Log::getLevel() {
    std::string t;
    switch (_level) {
    case LOG_DEBUG_LEVEL:
        return "DEBUG";
    case LOG_WARN_LEVEL:
        return "WARNING";
    case LOG_ERR_LEVEL:
        return "ERROR";
        break;
    default:
        return "DEBUG";
    }
    return "DEBUG";
}
void Log::toFile() {
    std::string log = "(" + _currenttime + ")[root - :" + this->getLevel() + "] " + _log_data;
    Logger::getLogger().outStream(log);
}
#ifdef LOG_MAIN
int main() {
    int a = 10086, b = 10010;
    Log glog;
    int i = 10;
    while (i--) {
        Log().log().setLevel(LOG_DEBUG_LEVEL).format("%d-%d-%s", a, b, "this is just a beginning to loops").toFile();
        sleep(1);
    }
    // std::cout<<"TimeFormat:"<<TimeFormat()<<std::endl;
    // sleep(1);
    // std::cout<<"TimeFormat:"<<TimeFormat()<<std::endl;
    return 0;
}
#endif
