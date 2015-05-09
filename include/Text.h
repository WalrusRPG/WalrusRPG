#ifndef INCLUDE_TEXT_H
#define INCLUDE_TEXT_H

#include <string>

namespace WalrusRPG
{
    namespace Graphics
    {
        namespace Text
        {
            void print_char(char c, unsigned x, unsigned y);

            void print_string(const char *str, unsigned x, unsigned y);
        }
    }
}


#endif
