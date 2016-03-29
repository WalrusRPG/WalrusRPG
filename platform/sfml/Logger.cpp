#include "Logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
using namespace WalrusRPG;

namespace
{
    // TODO : Find a better name
    /**
     * Prints the timestamp and the message category/type.
     */
    void print_premessage(const char *type)
    {
        char date_buffer[256];
        time_t now = time(0);
        strftime(date_buffer, 256, "%Y-%m-%d %H:%M:%S", localtime(&now));
        printf("%s %5s : ", date_buffer, type);
    }
}

// NOTE : I really wish there would be a better way to handle these stupid va_lists. So
// much redundant code...

void Logger::log(const char *fmt, ...)
{
    print_premessage("  [LOG]");
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
    print_premessage(" [WARN]");
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
