#ifndef LOGGER_H
#define LOGGER_H
#define FUNCTION_NAME __func__
#define FUNCTION_LINE __LINE__
#define DEBUG

#include <iostream>
#include <string>
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <time.h>

enum LogPriority {
    InfoPriority, WarningPriority, ErrorPriority, Fatal, DebugPriority
};

class LOG {
private:
    static LogPriority priority;
    static std::ostream& out_;

    static std::string PrintTime();

    template <typename... Args>
    static void doPrint(std::ostream& out, Args&&... args);

    template <typename ... Args>
    static void func_body(const char* name, int line, const char* caller, const char* message);

public:
    static void set_priority(LogPriority new_priority);

    template <typename ... Args>
    static void INFO(const char* function, int line, const char* message, Args... args);

    template <typename ... Args>
    static void WARNING(const char* function, int line, const char* message, Args... args);

    template <typename ... Args>
    static void ERROR(const char* function, int line, const char* message, Args... args);

    template <typename ... Args>
    static void FATAL(const char* function, int line, const char* message, Args... args);

    template <typename ... Args>
#ifdef DEBUG
    static void LOG_DEBUG(const char* function, int line, const char* message, Args... args);
#endif
};

LogPriority LOG::priority = InfoPriority;
std::ostream& LOG::out_ = std::cout;

#endif //LOGGER_H