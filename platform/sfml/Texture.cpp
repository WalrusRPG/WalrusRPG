#include "Texture.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/OpenGL.hpp>
#include <cstdint>
#include "utility/misc.h"

#define TEXTURE WalrusRPG::Graphics::Texture

TEXTURE::Texture(char *data) : data()
{
    UNUSED(data);
    // TOOD : load from PIAF
    this->data.loadFromFile("art/overworld.png");
}

TEXTURE::~Texture()
{
}

WalrusRPG::Utils::Rect TEXTURE::get_dimensions()
{
    sf::Vector2u size = data.getSize();
    return WalrusRPG::Utils::Rect(0, 0, size.x, size.y);
}

const WalrusRPG::Graphics::Pixel TEXTURE::get_pixel(unsigned x, unsigned y)
{
    UNUSED(x);
    UNUSED(y);
    // TODO : return the actual value
    return WalrusRPG::Graphics::Pixel(0);
}
