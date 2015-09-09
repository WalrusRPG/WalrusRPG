#include "Input.h"
#include <SFML/Window/Keyboard.hpp>

#define INPUT WalrusRPG::Input

bool INPUT::key_a()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
}

bool INPUT::key_b()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace);
}

bool INPUT::key_up()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::W);
}

bool INPUT::key_down()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

bool INPUT::key_left()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::A);
}

bool INPUT::key_right()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}
