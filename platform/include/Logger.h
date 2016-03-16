#ifndef INCLUDE_LOGGER_H
#define INCLUDE_LOGGER_H
namespace WalrusRPG
{
    namespace Logger
    {
        void log(const char *fmt, ...);
        void debug(const char *fmt, ...);
        void warn(const char *fmt, ...);
        void error(const char *fmt, ...);

#ifdef TARGET_SFML
        void debug_render();
#endif
    }
}

#endif