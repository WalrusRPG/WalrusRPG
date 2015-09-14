#include "Input.h"
#include "sfwindow.h"
#include <SFML/Window/Keyboard.hpp>

#define INPUT WalrusRPG::Input
using sf::Keyboard;

bool INPUT::key_a()
{
    return window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Return);
}

bool INPUT::key_b()
{
    return window.hasFocus() && Keyboard::isKeyPressed(Keyboard::BackSpace);
}

bool INPUT::key_up()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up));
}

bool INPUT::key_down()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down));
}

bool INPUT::key_left()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left));
}

bool INPUT::key_right()
{
    return window.hasFocus() && (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right));
}
