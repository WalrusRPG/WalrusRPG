#ifndef INCLUDE_LOGGER_H
#define INCLUDE_LOGGER_H
namespace WalrusRPG
{
    // Logging function to show content on screen (with IMGUI branch), console or
    // eventually in a file (for nspire?)
    namespace Logger
    {
        void init();
        void deinit();
        void log(const char *fmt, ...);
        void debug(const char *fmt, ...);
        void warn(const char *fmt, ...);
        void error(const char *fmt, ...);

#ifdef IMGUI
        void debug_render();
#endif
    }
}

#endif
