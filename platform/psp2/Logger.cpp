#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <debugnet.h>

using namespace WalrusRPG;

namespace
{
    // PrintConsole *console;
    // TODO : Find a better name
    /**
     * Prints the timestamp and the message category/type.
     */
    void print_premessage(const char *type)
    {
        char date_buffer[256];
        time_t now = time(0);
        strftime(date_buffer, 256, "%H:%M:%S", localtime(&now));
        printf("%s %5s : ", date_buffer, type);
    }
    bool ready;
}

// NOTE : I really wish there would be a better way to handle these stupid va_lists. So
// much redundant code...

void Logger::init()
{
    // console = consoleInit(GFX_BOTTOM, NULL);
    ready = false;
    // int ret = debugNetInit((char*)"192.168.1.13",1337,3);
    // if(ret) ready = true;
}

void Logger::deinit()
{
    if (ready)
        debugNetFinish();
}

void Logger::log(const char *fmt, ...)
{
    if (!ready)
        return;
    // print_premessage("  [LOG]");
    va_list args;
    va_start(args, fmt);
    char *hax = (char *) fmt;
    debugNetPrintf(1, hax, args);
    vprintf(fmt, args);
    va_end(args);
}

void Logger::debug(const char *fmt, ...)
{
    if (!ready)
        return;
    // print_premessage("[DEBUG]");
    va_list args;
    va_start(args, fmt);
    char *hax = (char *) fmt;
    debugNetPrintf(2, hax, args);
    va_end(args);
    puts("");
}

void Logger::warn(const char *fmt, ...)
{
    if (!ready)
        return;
    // print_premessage(" [WARN]");
    va_list args;
    va_start(args, fmt);
    char *hax = (char *) fmt;
    debugNetPrintf(2, hax, args);
    va_end(args);
    puts("");
}

void Logger::error(const char *fmt, ...)
{
    if (!ready)
        return;
    // print_premessage("[ERROR]");
    va_list args;
    va_start(args, fmt);
    char *hax = (char *) fmt;
    debugNetPrintf(3, hax, args);
    va_end(args);
    puts("");
}
