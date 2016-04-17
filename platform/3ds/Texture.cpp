#include "Texture.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "utility/misc.h"
#include "render/Pixel.h"
#include "lodepng.h"
#include <3ds.h>
#include <sf2d.h>

using namespace WalrusRPG::Graphics; /*Texture*/
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::PIAF::File;
using WalrusRPG::Utils::Rect;

#include "Logger.h"

Texture::Texture(char *data) : data()
{
    uint16_t *data_16 = (uint16_t *) data;
    this->data =
        sf2d_create_texture(data_16[0], data_16[1], TEXFMT_RGB565, SF2D_PLACE_VRAM);
    memcpy(this->data->data, &data_16[3], data_16[0] * data_16[1] * sizeof(uint16_t));
}

Texture::Texture(WalrusRPG::PIAF::File entry)
{
    unsigned char *pic;
    unsigned width, height;

    signed result = lodepng_decode32(&pic, &width, &height, (unsigned char *) entry.get(),
                                     entry.file_size);

    data =
        sf2d_create_texture_mem_RGBA8(pic, width, height, TEXFMT_RGBA8, SF2D_PLACE_RAM);

    Logger::debug("Ready : %p", data);
    free(pic);
}

Texture::~Texture()
{
    sf2d_free_texture(data);
}

const Rect Texture::get_dimensions()
{
    return {0, 0, data->width, data->height};
}

const Pixel Texture::get_pixel(unsigned x, unsigned y)
{
    u32 pixel = sf2d_get_pixel(data, x, y);
    return Pixel(RGBA8_GET_R(pixel), RGBA8_GET_G(pixel), RGBA8_GET_B(pixel));
}
