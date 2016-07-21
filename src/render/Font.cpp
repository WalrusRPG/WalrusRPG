#include "Font.h"
#include "Logger.h"
#include "utility/misc.h"
#include <cstdarg>
#include <cstdio>
#include <string.h>
#include <zlib.h>

using namespace WalrusRPG; /*::Logger*/
using WalrusRPG::Graphics::Font;
using WalrusRPG::Graphics::FontException;
using WalrusRPG::Graphics::CharacterParameters;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Graphics::Pixel;

Font::Font(Texture &font_tex, WalrusRPG::PIAF::File font_config)
    : baseline(0), space_width(1), font_tex(font_tex)
{
    const uint8_t *ptr = font_config.get();
    // TODO : forgot to put the version and the font type
    if (strncmp((const char *) ptr, "WFONT", 4) != 0)
    {
        Logger::error("%s: Bad magic cookie", __FILE__);
#ifdef WRPG_EXCEPTIONS
        throw FontException("%s: Bad magic cookie", __FILE__);
#endif
    }
    uint32_t expected_checksum = read_big_endian_value<uint32_t>(&ptr[4]);
    uint32_t calculated_checksum =
        crc32(0L, (const unsigned char *) (&ptr[8]), font_config.file_size - 8);
    if (expected_checksum != calculated_checksum)
    {
        Logger::error("%s: Bad checksum : %x != %x\n", __FILE__, expected_checksum,
                      calculated_checksum);
#ifdef WRPG_EXCEPTIONS
        throw FontException("%s: Bad checksum : %x != %x\n", __FILE__, expected_checksum,
                            calculated_checksum);
#endif
    }

    uint32_t font_verion = read_big_endian_value<uint32_t>(&ptr[8]);
    if (font_verion != FONT_VERSION)
    {
        Logger::error("%s: Wront font version : %x != %x\n", __FILE__, FONT_VERSION,
                      font_verion);
#ifdef WRPG_EXCEPTIONS
        throw FontException("%s: Wront font version : %x != %x\n", __FILE__, FONT_VERSION,
                            font_verion);
#endif
    }

    baseline = read_big_endian_value<uint32_t>(&ptr[12]);
    space_width = read_big_endian_value<uint32_t>(&ptr[20]);

    // Stupid thing to accelerate a biiiit the font loading, I think.
    uint8_t *current_char = (uint8_t *) ptr + 24;
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

void Font::draw(uint16_t x, uint16_t y, const char c) const
{
    uint8_t c2 = (uint8_t) c;
    put_sprite(font_tex, x + chars[c2].x_offset, y + chars[c2].y_offset,
               chars[c2].dimensions);
}

void Font::draw(uint16_t x, uint16_t y, const char c, const Pixel &col) const
{
    uint8_t c2 = (uint8_t) c;
    put_sprite_tint(font_tex, x + chars[c2].x_offset, y + chars[c2].y_offset,
                    chars[c2].dimensions, col);
}

// Note : I wonder if clang/g++ compiles correctly inlines with dynamic functions or
// stuff... Because damn the duplicated code.

void Font::draw(uint16_t x, uint16_t y, const char *str) const
{
    for (unsigned i = 0; str[i] && x < 320; i++)
    {
        unsigned char c = str[i];
        if (c == 32)
        {
            x += space_width;
            continue;
        }
        draw(x, y, c);
        x += chars[c].dimensions.width + 1;
    }
}

void Font::draw(uint16_t x, uint16_t y, const char *str, const Pixel &col) const
{
    for (unsigned i = 0; str[i] && x < 320; i++)
    {
        unsigned char c = str[i];
        if (c == 32)
        {
            x += space_width;
            continue;
        }
        draw(x, y, c, col);
        x += chars[c].dimensions.width + 1;
    }
}

void Font::draw_format(uint16_t x, uint16_t y, const char *format, ...) const
{
    char buffer[513] = {0};

    va_list args;
    va_start(args, format);
    int size = vsnprintf(buffer, 512, format, args);
    va_end(args);
    if (size < 0)
        return;
    draw(x, y, buffer);
}

void Font::draw_format(uint16_t x, uint16_t y, const Pixel &col, const char *format,
                       ...) const
{
    char buffer[513] = {0};

    va_list args;
    va_start(args, format);
    int size = vsnprintf(buffer, 512, format, args);
    va_end(args);
    if (size < 0)
        return;
    draw(x, y, buffer, col);
}
