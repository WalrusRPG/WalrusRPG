#include <cstring>
#include "Quirks.h"
#include "utility/misc.h"

using namespace WalrusRPG;
using tinystl::string;

void Quirks::init(const char *argv_0)
{
    UNUSED(argv_0);
}

void Quirks::deinit()
{
}

std::unique_ptr<char> Quirks::solve_absolute_path(const char *path)
{
    std::unique_ptr<char> result(new char[strlen(path)]); 
    strcpy(result.get(), path);
    return result;
}
