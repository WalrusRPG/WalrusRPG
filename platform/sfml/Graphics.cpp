#include "Graphics.h"
#include <SFML/Graphics.hpp>
#include "utility/misc.h"

#define GRAPHICS WalrusRPG::Graphics

sf::RenderWindow window;

void GRAPHICS::init()
{
    window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - 160,
                                    sf::VideoMode::getDesktopMode().height / 2 - 120));
    window.create(sf::VideoMode(320, 240), "WalrusRPG");
}

void GRAPHICS::deinit()
{
    window.close();
}

void GRAPHICS::frame_begin()
{
    window.clear(sf::Color::Black);
}

void GRAPHICS::frame_end()
{
    window.display();
}

void GRAPHICS::put_sprite(const Texture &sheet, int x, int y,
                          const WalrusRPG::Utils::Rect &window)
{
    sf::Sprite sprite;
    sprite.setTexture(sheet.data);
    sprite.setTextureRect(sf::IntRect(window.x, window.y, window.width, window.height));
    sprite.setPosition(x, y);
    ::window.draw(sprite);
}

void GRAPHICS::fill(const WalrusRPG::Graphics::Pixel &color)
{
    UNUSED(color);
}
