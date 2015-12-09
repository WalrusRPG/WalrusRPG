#include "Texture.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cstdint>
#include "utility/misc.h"
#include "render/Pixel.h"

#define TEXTURE WalrusRPG::Graphics::Texture

using WalrusRPG::Graphics::Pixel;

TEXTURE::Texture(char *data)
:data()
{
    uint16_t *data_16 = (uint16_t *) data;
    this->data.create(data_16[0], data_16[1]);
    sf::Uint8 *pixels = new sf::Uint8[data_16[0] * data_16[1] * 4];
    for (unsigned y = 0; y < data_16[1]; y++)
    {
        for (unsigned x = 0; x < data_16[0]; x++)
        {
            Pixel pix(data_16[3 + y * data_16[0] + x]);
            unsigned pixel_offset = data_16[0] * y + x;
            pixels[4 * pixel_offset] = pix.r << 3;
            pixels[4 * pixel_offset + 1] = pix.g << 2;
            pixels[4 * pixel_offset + 2] = pix.b << 3;
            pixels[4 * pixel_offset + 3] = pix.value == data_16[2] ? 0 : 255;
        }
    }
    this->data.update(pixels);

    delete[] pixels;

}

TEXTURE::Texture(WalrusRPG::PIAF::File entry)
:data()
{
    // UNUSED(data);
    // TOOD : load from PIAF
    // this->data.loadFromFile("art/overworld.png");
    this->data.loadFromMemory(entry.get(), entry.file_size);
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
