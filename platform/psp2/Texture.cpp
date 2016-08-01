#include "Texture.h"
// #include "lodepng.h"
#include "render/Pixel.h"
#include "utility/misc.h"
#include <cstdint>
#include <cstring>
#include <vita2d.h>

using namespace WalrusRPG::Graphics; /*Texture*/
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::PIAF::File;
using WalrusRPG::Utils::Rect;

#include "Logger.h"

Texture::Texture(char *data) : data()
{
    uint16_t *data_16 = (uint16_t *) data;
    // SCE_GXM_TEXTURE_FORMAT_U5U6U5_RGB
    this->data = vita2d_create_empty_texture_format(data_16[0], data_16[1],
                                                    SCE_GXM_TEXTURE_FORMAT_U5U6U5_RGB);
    memcpy(vita2d_texture_get_datap(this->data), &data_16[3],
           data_16[0] * data_16[1] * sizeof(uint16_t));
}

Texture::Texture(WalrusRPG::PIAF::File entry)
{
    // unsigned char *pic;
    // signed result = lodepng_decode32(&pic, &width, &height, (unsigned char *)
    // entry.get(),
    //                                  entry.file_size);
    data = vita2d_load_PNG_buffer(entry.get());
    // sf2d_create_texture_mem_RGBA8(pic, width, height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    // free(pic);
}

Texture::~Texture()
{
    vita2d_wait_rendering_done();
    vita2d_free_texture(data);
}

const Rect Texture::get_dimensions()
{
    return {0, 0, vita2d_texture_get_width(this->data),
            vita2d_texture_get_height(this->data)};
}

// TODO
const Pixel Texture::get_pixel(unsigned x, unsigned y)
{
    return Pixel(0xFFF0);
    // uint32_t pixel = sf2d_get_pixel(data, x, y);
    // return Pixel(RGBA8_GET_R(pixel), RGBA8_GET_G(pixel), RGBA8_GET_B(pixel));
}
