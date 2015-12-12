#include "Texture.h"
#include "piaf/Archive.h"
#include "utility/Rect.h"
#include "render/Pixel.h"
#include "utility/misc.h"
#include "lodepng.h"
#include "Input.h"
#include "Graphics.h"
using WalrusRPG::Graphics::Black;
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Utils::Rect;
using WalrusRPG::PIAF::File;

namespace
{
    /*
    texture_data_t loadPNG(char *data)
    {
        // TODO : stuff
        UNUSED(data);
        return nullptr;
    }
    */
}

Texture::Texture(File entry)
{
    unsigned char *pic;
    unsigned width, height;

    signed result =
        lodepng_decode_memory(&pic, &width, &height, (unsigned char *) entry.get(),
                              entry.file_size, LCT_RGBA, 8);
    UNUSED(result);

    data = new uint16_t[width * height + 3];
    data[0] = width;
    data[1] = height;
    bool transparency_set(false);
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++)
        {
            bool is_transparent = (pic[(y * width + x) * 4 + 3] == 0);
            if (is_transparent && transparency_set)
            {
                data[y * width + x + 3] = data[2];
                continue;
            }
            uint16_t color = (pic[(y * width + x) * 4] >> 3) << 11;
            color |= (pic[(y * width + x) * 4 + 1] >> 2) << 5;
            color |= (pic[(y * width + x) * 4 + 2] >> 3);
            if (is_transparent && !transparency_set)
            {
                data[2] = color;
                transparency_set = true;
            }
            data[y * width + x + 3] = color;
        }
    }
    delete[] pic;
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
