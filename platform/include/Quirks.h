#ifndef INCLUDE_QUIRKS_H
#define INCLUDE_QUIRKS_H

#include "platform.h"
#include <TINYSTL/string.h>
#include <memory>

namespace WalrusRPG
{
    namespace Quirks
    {
        void init(const char *argv_0);
        void deinit();

        // Relative path to absolute path resolving.
        // Exists because at this time Ndless doesn't support relative paths.
        std::unique_ptr<char> solve_absolute_path(const char *path);

        bool get_key(keycode_t key);
    }
}

#endif
