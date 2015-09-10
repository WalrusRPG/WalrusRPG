#include "Graphics.h"
#include "engine/main.h"
#include <SFML/Graphics.hpp>
#include "utility/misc.h"

#define GRAPHICS WalrusRPG::Graphics

sf::RenderWindow window;
sf::View view;
sf::RenderTexture buffer;

void GRAPHICS::init()
{
    window.create(sf::VideoMode(320, 240), "WalrusRPG");
    view = sf::View(window.getDefaultView());
    buffer.create(320, 240);
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
    sf::Sprite sprite(buffer.getTexture());
    sf::Vector2u winsize = window.getSize();
    sf::Event event;
    float scale = min(winsize.x / 320.f, winsize.y / 240.f);

    while(window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            WalrusRPG::quit();
        }
    }

    window.setView(view = sf::View(sf::FloatRect(0, 0, winsize.x, winsize.y)));

    buffer.display();
    window.clear();
    sprite.setScale(scale, scale);
    sprite.setPosition((winsize.x - 320.f * scale) / 2, (winsize.y - 240.f * scale) / 2);
    window.draw(sprite);
    window.display();
}

void GRAPHICS::put_sprite(const Texture &sheet, int x, int y,
                          const WalrusRPG::Utils::Rect &window)
{
    sf::Sprite sprite;
    sprite.setTexture(sheet.data);
    sprite.setTextureRect(sf::IntRect(window.x, window.y, window.width, window.height));
    sprite.setPosition(x, y);
    buffer.draw(sprite);
}

void GRAPHICS::fill(const WalrusRPG::Graphics::Pixel &color)
{
    UNUSED(color);
}
