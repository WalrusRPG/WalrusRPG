#include <cstddef>
#include <cstring>
#include <memory>
#include <TINYSTL/string.h>
#include "Quirks.h"
#include "Logger.h"
#include "Interrupts.h"
#include "platform.h"


using namespace WalrusRPG;
using namespace Nspire;
using tinystl::string;

namespace
{
    static char *base_path = nullptr;
}

void Quirks::init(const char *argv_0)
{
    WalrusRPG::Logger::log("Quirks init");
    Interrupts::init();
    // Find last '/' occurence and remove the trailing characters
    // so we get rid of the executable name.
    int last_slash_occurence = -1;
    for (unsigned i = 0; argv_0[i] != '\0'; i++)
    {
        if (argv_0[i] == '/')
            last_slash_occurence = i;
    }
    if (last_slash_occurence > 0)
    {
        base_path = new char[last_slash_occurence + 2];
        strncpy(base_path, argv_0, last_slash_occurence);
        base_path[last_slash_occurence] = '/';
        base_path[last_slash_occurence + 1] = '\0';
    }
}

void Quirks::deinit()
{
    WalrusRPG::Logger::log("Quirks deinit");
    Interrupts::off();
    delete[] base_path;
}

std::unique_ptr<char> Quirks::solve_absolute_path(const char *path)
{
    const char nspire_suffix[] = ".tns";
    std::unique_ptr<char> result(
        new char[strlen(base_path) + strlen(path) + strlen(nspire_suffix) + 1]);
    strcpy(result.get(), base_path);
    strcpy(&result.get()[strlen(base_path)], path);
    strcpy(&result.get()[strlen(base_path) + strlen(path)], nspire_suffix);
    result.get()[strlen(base_path) + strlen(path) + strlen(nspire_suffix)] = '\0';
    return result;
}

bool Quirks::get_key(keycode_t key)
{
    return isKeyPressed(key);
}
