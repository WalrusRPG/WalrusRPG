#include "Texture.h"
#include "utility/Rect.h"
#include "render/Pixel.h"
#include "utility/misc.h"

using WalrusRPG::Graphics::Black;
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Utils::Rect;

namespace
{
    texture_data_t loadPNG(char *data)
    {
        // TODO : stuff
        UNUSED(data);
        return nullptr;
    }
}

Texture::Texture(char *data) : data((texture_data_t) data)
{
}

Texture::~Texture()
{
    // Don't deallocate for now since we still hardcode the data
    // delete (data);
}

Rect Texture::get_dimensions()
{
    return Rect(0, 0, data[0], data[1]);
}

const Pixel Texture::get_pixel(unsigned x, unsigned y)
{
    return Pixel(data[2 + data[0] * y + x]);
}
