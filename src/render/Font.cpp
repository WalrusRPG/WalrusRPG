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
	uint32_t calculated_checksum = crc32(0L, (const unsigned char *) (&ptr[8]),
		font_config.file_size - 8
		);
	if(expected_checksum != calculated_checksum)
	{
		// printf("Bad checksum : %x != %x\n", expected_checksum, calculated_checksum);
	}

	baseline = read_big_endian_value<uint8_t>(&ptr[8]);
	for (int i = 0; i < 256; ++i)
	{
		const uint8_t* current_char = ptr + 16 + (6*sizeof(uint16_t))*i;
		chars[i].dimensions.x = read_big_endian_value<int16_t>(current_char);
		chars[i].dimensions.y = read_big_endian_value<int16_t>(current_char+2);
		chars[i].dimensions.width = read_big_endian_value<uint16_t>(current_char+4);
		chars[i].dimensions.height = read_big_endian_value<uint16_t>(current_char+6);
		chars[i].x_offset = read_big_endian_value<int16_t>(current_char+8);
		chars[i].y_offset = read_big_endian_value<int16_t>(current_char+10);
	}
}

Font::~Font()
{
}