#ifndef INCLUDE_FONT_H
#define INCLUDE_FONT_H

#include "Graphics.h"
#include "Texture.h"
#include "piaf/Archive.h"
#include "utility/Rect.h"

namespace WalrusRPG
{
	namespace Font
	{
		struct CharacterParameters
		{
			// Sprite clip
			WalrusRPG::Utils::Rect dimensions;
			// Character rendering offset
			int8_t x_offset, y_offset;
		};

		class Font
		{

		public:
		 uint8_t baseline;
		 CharacterParameters chars[256];
		 WalrusRPG::Graphics::Texture& font_tex;

			Font(WalrusRPG::Graphics::Texture& font_tex, WalrusRPG::PIAF::File font_config);
			~Font();
		};
	}
}

#endif