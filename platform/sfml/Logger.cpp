#include "Logger.h"
#include <stdio.h>
#include <stdarg.h>

using namespace WalrusRPG;

void Logger::log(const char *fmt, ...)
{
    printf(" [LOG] : ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end (args);
}

void Logger::debug(const char *fmt, ...)
{
    printf("[DEBUG]: ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end (args);
}

void Logger::warn(const char *fmt, ...)
{
    printf("[WARN] : ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end (args);
}

void Logger::error(const char *fmt, ...)
{
    printf("[ERROR]: ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end (args);
}
