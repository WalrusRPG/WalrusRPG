#ifndef INCLUDE_QUIRKS_H
#define INCLUDE_QUIRKS_H

#include <TINYSTL/string.h>

namespace WalrusRPG
{
    namespace Quirks
    {
        void init(const char *argv_0);
        void deinit();

        // Relative path to absolute path resolving.
        // Exists because at this time Ndless doesn't support relative paths.
        tinystl::string solve_absolute_path(const char *path);
    }
}

#endif
