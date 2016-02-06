#include "Graphics.h"
#include "sfwindow.h"
#include <SFML/Graphics.hpp>
#include "utility/misc.h"
#include "utility/minmax.h"

using namespace WalrusRPG; /*::Graphics*/
using WalrusRPG::Utils::Rect;

sf::RenderWindow window;
sf::View view;
sf::RenderTexture buffer;


void Graphics::init()
{
    // window.create(sf::VideoMode::getDesktopMode(), "WalrusRPG",
    // sf::Style::Fullscreen);
    window.create(sf::VideoMode(640, 480), "WalrusRPG");
    window.setFramerateLimit(60);
    view = sf::View(window.getDefaultView());
    buffer.create(320, 240);
}

void Graphics::deinit()
{
    window.close();
}

void Graphics::frame_begin()
{
    window.clear(sf::Color::Black);
}

void Graphics::frame_end()
{
    sf::Sprite sprite(buffer.getTexture());
    sf::Vector2u winsize = window.getSize();
    sf::Event event;
    float scale = min(winsize.x / 320.f, winsize.y / 240.f);

    while (window.pollEvent(event))
    {
    }

    window.setView(view = sf::View(sf::FloatRect(0, 0, winsize.x, winsize.y)));

    buffer.display();
    window.clear();
    sprite.setScale(scale, scale);
    sprite.setPosition((winsize.x - 320.f * scale) / 2, (winsize.y - 240.f * scale) / 2);
    window.draw(sprite);
    window.display();
}

void Graphics::put_sprite(const Texture &sheet, int x, int y, const Rect &window)
{
    sf::Sprite sprite;
    sprite.setTexture(sheet.data);
    sprite.setTextureRect(sf::IntRect(window.x, window.y, window.width, window.height));
    sprite.setPosition(x, y);
    buffer.draw(sprite);
}

void Graphics::put_sprite_tint(const Texture &sheet, int x, int y, const Rect &window,
                               const Pixel &color)
{
    sf::Sprite sprite;
    sprite.setTexture(sheet.data);
    sprite.setTextureRect(sf::IntRect(window.x, window.y, window.width, window.height));
    sprite.setPosition(x, y);
    sprite.setColor(sf::Color(color.r << 3, color.g << 2, color.b << 3, 255));
    buffer.draw(sprite);
}

void Graphics::put_sprite_clipping(const Texture &sheet, int x, int y,
                                   const Rect &sprite_window, const Rect &clipping_window)
{
    const signed &ss_x = sprite_window.x, ss_y = sprite_window.y;
    const signed &ss_w = sprite_window.width, &ss_h = sprite_window.height;
    const signed &cx = clipping_window.x, &cy = clipping_window.y;
    const signed &cw = clipping_window.width, &ch = clipping_window.height;
    const signed lx = x - cx, ly = y - cy;

    if (lx < -ss_w || lx > cw)
        return;
    if (ly < -ss_h || ly > ch)
        return;

    signed fx = x, fy = y;
    signed fssx = ss_x, fssy = ss_y, fssw = ss_w, fssh = ss_h;

    if (lx < 0)
    {
        fssw = ss_w + lx;
        fssx = -lx;
        fx = cx;
    }

    if (lx > cw - ss_w)
    {
        fssw -= lx - (cw - ss_w);
    }

    if (ly > ch - ss_h)
    {
        fssh -= ly - (ch - ss_h);
    }

    if (ly < 0)
    {
        fssh = ss_h + ly;
        fssy = -ly;
        fy = cy;
    }

    sf::Sprite sprite;
    sprite.setTexture(sheet.data);
    sprite.setTextureRect(sf::IntRect(fssx, fssy, fssw, fssh));
    sprite.setPosition(fx, fy);
    buffer.draw(sprite);
}


void Graphics::fill(const Pixel &color)
{
    buffer.clear(sf::Color(color.r << 3, color.g << 2, color.b << 3, 255));
}

void Graphics::put_pixel(uint16_t x, uint16_t y, const Pixel &color)
{
    sf::RectangleShape pixel;
    pixel.setSize(sf::Vector2f(1, 1));
    pixel.setFillColor(sf::Color(color.r << 3, color.g << 2, color.b << 3, 255));
    pixel.setPosition(x, y);
    buffer.draw(pixel);
}

void Graphics::put_horizontal_line(uint16_t x, uint16_t x2, uint16_t y,
                                   const Pixel &color)
{
    put_rectangle({x, y, x2 - x + 1, 1}, color);
}

void Graphics::put_vertical_line(uint16_t x, uint16_t y, uint16_t y2, const Pixel &color)
{
    put_rectangle({x, y, 1, y2 - y + 1}, color);
}

void Graphics::put_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2,
                        const Pixel &color)
{
    sf::Color lineColor(color.r << 3, color.g << 2, color.b << 3, 255);
    sf::Vertex line[] = {sf::Vertex(sf::Vector2f(x, y), lineColor),
                         sf::Vertex(sf::Vector2f(x2 + 1, y2 + 1), lineColor)};

    buffer.draw(line, 2, sf::Lines);
}

void Graphics::put_rectangle(const Rect &rect, const Pixel &color)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(rect.width, rect.height));
    rectangle.setFillColor(sf::Color(color.r << 3, color.g << 2, color.b << 3, 255));
    rectangle.setPosition(rect.x, rect.y);
    buffer.draw(rectangle);
}
