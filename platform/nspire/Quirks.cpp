#include <cstddef>
#include <cstring>
#include <memory>
#include <TINYSTL/string.h>
#include "Quirks.h"
#include "Interrupts.h"


using namespace WalrusRPG;
using namespace Nspire;
using tinystl::string;

namespace
{
    static char* base_path = nullptr;
}

void Quirks::init(const char *argv_0)
{
    Interrupts::init();
    // Find last '/' occurence and remove the trailing characters
    // so we get rid of the executable name.
    int last_slash_occurence = -1;
    for (unsigned i = 0; argv_0[i] != '\0'; i++)
    {
        if (argv_0[i] == '/')
            last_slash_occurence = i;
    }
    if( last_slash_occurence > 0)
    {
        base_path = new char[last_slash_occurence+2];
        strncpy(base_path, argv_0, last_slash_occurence);
        base_path[last_slash_occurence] = '/';
        base_path[last_slash_occurence+1] = '\0';
    }
}

void Quirks::deinit()
{
    Interrupts::off();
    delete[] base_path;
}

std::unique_ptr<char> Quirks::solve_absolute_path(const char *path)
{
    std::unique_ptr<char> result(new char[strlen(base_path) + strlen(path)]);
    strcpy(result.get(), base_path);
    strcpy(&result.get()[strlen(base_path)], path);
    return result;
}
