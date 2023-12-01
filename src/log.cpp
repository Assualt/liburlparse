#include "log.h"

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
    char buffer[512];
    va_list va;
    va_start(va, fmt_str.c_str());
    int size = vsnprintf(buffer, 512, fmt_str.c_str(), va);
    va_end(va);
    _log_data.assign(buffer, size);
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

