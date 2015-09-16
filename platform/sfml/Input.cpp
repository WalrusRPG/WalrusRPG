#include "Input.h"
#include "sfwindow.h"
#include <SFML/Window/Keyboard.hpp>

#define INPUT WalrusRPG::Input
using sf::Keyboard;

bool INPUT::key_a()
{
    return window.hasFocus() &&
           (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Z));
}

bool INPUT::key_b()
{
    return window.hasFocus() && Keyboard::isKeyPressed(Keyboard::X);
}

bool INPUT::key_l()
{
    return window.hasFocus() &&
           (Keyboard::isKeyPressed(Keyboard::Q) || Keyboard::isKeyPressed(Keyboard::A));
}

bool INPUT::key_r()
{
    return window.hasFocus() && Keyboard::isKeyPressed(Keyboard::S);
}

bool INPUT::key_up()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::Up));
}

bool INPUT::key_down()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::Down));
}

bool INPUT::key_left()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::Left));
}

bool INPUT::key_right()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::Right));
}

bool INPUT::key_start()
{
    return window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Return);
}

bool INPUT::key_select()
{
    return window.hasFocus() && Keyboard::isKeyPressed(Keyboard::BackSpace);
}
