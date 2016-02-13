#include <cstring>
#include "Quirks.h"
#include "utility/misc.h"
#include "sfwindow.h"

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
    std::unique_ptr<char> result(new char[strlen(path)+1]);
    strcpy(result.get(), path);
    return result;
}

bool Quirks::get_key(keycode_t key)
{
    return sf::Keyboard::isKeyPressed(key) && window.hasFocus();
}
