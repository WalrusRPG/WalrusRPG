#include "Input.h"
#include "sfwindow.h"
#include <SFML/Window/Keyboard.hpp>

#define INPUT WalrusRPG::Input

bool INPUT::key_a()
{
    return window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
}

bool INPUT::key_b()
{
    return window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace);
}

bool INPUT::key_up()
{
    return window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::W);
}

bool INPUT::key_down()
{
    return window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

bool INPUT::key_left()
{
    return window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::A);
}

bool INPUT::key_right()
{
    return window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}
