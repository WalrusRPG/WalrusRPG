#include "Logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
using namespace WalrusRPG;

namespace
{
    void print_premessage(const char *type)
    {
        char date_buffer[256];
        time_t now = time(0);
        strftime(date_buffer, 256, "%Y-%m-%d %H:%M:%S", localtime(&now));
        printf("%s %5s : ", date_buffer, type);
    }
}

void Logger::log(const char *fmt, ...)
{
    print_premessage("[LOG]");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    puts("");
}

void Logger::debug(const char *fmt, ...)
{
    print_premessage("[DEBUG]");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    puts("");
}

void Logger::warn(const char *fmt, ...)
{
    print_premessage("[WARN]");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    puts("");
}

void Logger::error(const char *fmt, ...)
{
    print_premessage("[ERROR]");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    puts("");
}
