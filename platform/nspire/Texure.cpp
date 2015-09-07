#include "Texture.h"
#include "utility/Rect.h"
#include "render/Pixel.h"
#include <cstdint>

using WalrusRPG::Graphics::Black;
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Utils::Rect;

namespace
{
    texture_data_t loadPNG(char *data)
    {
        // stuff
        return nullptr;
    }
}

Texture::Texture(char *data) : texture(loadPNG(data))
{
    width = texture[0];
    height = texture[1];
}

Texture::~Texture()
{
    delete (texture);
}

Rect Texture::get_dimensions()
{
    return Rect(0, 0, width, height);
}

const Pixel Texture::get_pixel(unsigned x, unsigned y)
{
    return Pixel(texture[2 + width*y + x]);
}
