#include "Quirks.h"
#include "sfwindow.h"

using namespace WalrusRPG;

void Quirks::init()
{
}

void Quirks::deinit()
{
}

bool Quirks::get_key(keycode_t key)
{
    return sf::Keyboard::isKeyPressed(key) && window.hasFocus();
}
