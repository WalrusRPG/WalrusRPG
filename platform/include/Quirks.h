#ifndef INCLUDE_QUIRKS_H
#define INCLUDE_QUIRKS_H

#include "platform.h"

namespace WalrusRPG
{
    namespace Quirks
    {
        void init();
        void deinit();
        bool get_key(keycode_t key);
    }
}

#endif
