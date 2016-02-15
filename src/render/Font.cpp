#include <string.h>
#include <zlib.h>
#include "Font.h"
#include "utility/misc.h"

using WalrusRPG::Font::Font;
using WalrusRPG::Font::CharacterParameters;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Graphics::Pixel;

Font::Font(Texture &font_tex, WalrusRPG::PIAF::File font_config)
    : baseline(0), space_width(1), font_tex(font_tex)
{
    const uint8_t *ptr = font_config.get();
    // TODO : parse file
    // TODO : forgot to put the version and the font type
    if (strncmp((const char *) ptr, "WFONT", 4) != 0)
    {
        // TODO : wrong header
    }
    uint32_t expected_checksum = read_big_endian_value<uint32_t>(&ptr[4]);
    uint32_t calculated_checksum =
        crc32(0L, (const unsigned char *) (&ptr[8]), font_config.file_size - 8);
    if (expected_checksum != calculated_checksum)
    {
        // printf("Bad checksum : %x != %x\n", expected_checksum, calculated_checksum);
    }

    baseline = read_big_endian_value<uint8_t>(&ptr[12]);
    space_width = read_big_endian_value<uint32_t>(&ptr[20]);

    // Stupid thing to accelerate a biiiit the font loading, I think.
    uint8_t *current_char = (uint8_t*) ptr + 24;
    for (int i = 0; i < 256; ++i)
    {
        chars[i].dimensions.x = read_big_endian_value<int16_t>(current_char);
        chars[i].dimensions.y = read_big_endian_value<int16_t>(current_char + 2);
        chars[i].dimensions.width = read_big_endian_value<uint16_t>(current_char + 4);
        chars[i].dimensions.height = read_big_endian_value<uint16_t>(current_char + 6);
        chars[i].x_offset = read_big_endian_value<int16_t>(current_char + 8);
        chars[i].y_offset = read_big_endian_value<int16_t>(current_char + 10);
        current_char += (6 * sizeof(uint16_t));
    }
}

Font::~Font()
{
}

void Font::draw(const char c, uint16_t x, uint16_t y)
{
    uint8_t c2 = (uint8_t) c;
    put_sprite(font_tex, x + chars[c2].x_offset, y + chars[c2].y_offset,
               chars[c2].dimensions);
}

void Font::draw(const char c, uint16_t x, uint16_t y, const Pixel &col)
{
    uint8_t c2 = (uint8_t) c;
    put_sprite_tint(font_tex, x + chars[c2].x_offset, y + chars[c2].y_offset,
                    chars[c2].dimensions, col);
}

void Font::draw(const char *str, uint16_t x, uint16_t y)
{
    for (unsigned i = 0; str[i] && x < 320; i++)
    {
        unsigned char c = str[i];
        if (c == 32)
        {
            x += space_width;
            continue;
        }
        draw(c, x, y);
        x += chars[c].dimensions.width + 1;
    }
}

void Font::draw(const char *str, uint16_t x, uint16_t y, const Pixel &col)
{
    for (unsigned i = 0; str[i] && x < 320; i++)
    {
        unsigned char c = str[i];
        if (c == 32)
        {
            x += space_width;
            continue;
        }
        draw(c, x, y, col);
        x += chars[c].dimensions.width + 1;
    }
}