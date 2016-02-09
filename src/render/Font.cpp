#include <string.h>
#include <zlib.h>
#include "Font.h"
#include "utility/misc.h"

using WalrusRPG::Font::Font;
using WalrusRPG::Font::CharacterParameters;
using WalrusRPG::Graphics::Texture;

Font::Font(Texture& font_tex, WalrusRPG::PIAF::File font_config)
	: font_tex(font_tex)
{
	const uint8_t* ptr = font_config.get();
	// TODO : parse file
	// TODO : forgot to put the version and the font type
	if(strncmp((const char*)ptr, "WFONT", 4) != 0)
	{
		// TODO : wrong header
	}
	uint32_t expected_checksum = read_big_endian_value<uint32_t>(&ptr[4]);
	if(crc32(0L, (unsigned char *) (&ptr[8]), 1 + (6*4 + 2)*255))
	{
		// TODO : bad checksum
	}
	baseline = read_big_endian_value<uint8_t>(&ptr[8]);
	for (int i = 0; i < 256; ++i)
	{
		const uint8_t* current_char = ptr + 9 + (6*4+2)*i;
		chars[i].dimensions.x = read_big_endian_value<signed>(current_char);
		chars[i].dimensions.y = read_big_endian_value<signed>(current_char+4);
		chars[i].dimensions.width = read_big_endian_value<unsigned>(current_char+8);
		chars[i].dimensions.height = read_big_endian_value<unsigned>(current_char+12);
		chars[i].x_offset = read_big_endian_value<unsigned>(current_char+16);
		chars[i].y_offset = read_big_endian_value<unsigned>(current_char+18);
	}
}

Font::~Font()
{
}