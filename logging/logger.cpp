#include "logger.h"

std::string LOG::PrintTime() {
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    std::string currentTimeStr = std::ctime(&currentTime_t);
    return currentTimeStr;
}

template <typename... Args>
void LOG::doPrint(std::ostream& out, Args&&... args)
{
    (out << ... << args);
}

template <typename ... Args>
void LOG::func_body(const char* name, int line, const char* caller, const char* message) {
    out_ << LOG::PrintTime();
    out_ << '[' << name << ':' << line << ']' << '\t';
    out_ << '[' << caller << ']' << '\t';
    out_ << message << '\t';
}

void LOG::set_priority(LogPriority new_priority) {
    priority = new_priority;
}

template <typename ... Args>
void LOG::INFO(const char* function, int line, const char* message, Args... args) {
    if (priority <= InfoPriority) {
        func_body(function, line, __func__, message);
        doPrint(out_, args...);
        out_ << '\n';
    }
}

template <typename ... Args>
void LOG::WARNING(const char* function, int line, const char* message, Args... args) {
    if (priority <= WarningPriority) {
        func_body(function, line, __func__, message);
        doPrint(out_, args...);
        out_ << '\n';
    }
}

template <typename ... Args>
void LOG::ERROR(const char* function, int line, const char* message, Args... args) {
    if (priority <= ErrorPriority) {
        func_body(function, line, __func__, message);
        doPrint(out_, args...);
        out_ << '\n';
    }
}

template <typename ... Args>
void LOG::FATAL(const char* function, int line, const char* message, Args... args) {
    func_body(function, line, __func__, message);
    doPrint(out_, args...);
    out_ << '\n';
    exit(EXIT_FAILURE);
}

template <typename ... Args>
void LOG::LOG_DEBUG(const char* function, int line, const char* message, Args... args) {
    if (priority <= DebugPriority) {
        func_body(function, line, __func__, message);
        doPrint(out_, args...);
        out_ << '\n';
    }
}