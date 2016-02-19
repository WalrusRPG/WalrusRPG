#ifndef INCLUDE_TEXTBOX_H
#define INCLUDE_TEXTBOX_H

#include "Graphics.h"
#include "render/Font.h"
#include "TINYSTL/vector.h"

namespace WalrusRPG
{
	constexpr char MAGIC_TOKEN = '\xFF';
	constexpr unsigned COMMAND_LEGNTH = 4;
	
	struct TextboxChar
	{
		char c;
		uint8_t routine;
		uint8_t arg1;
		uint8_t arg2;
		uint8_t arg3;
	};

	class Textbox
	{
	private:
		Graphics::Font fnt;
		tinystl::vector<TextboxChar> buffer;
		unsigned buffer_index;
		Graphics::Pixel current_color;
		signed letter_wait;
		signed letter_wait_cooldown;
	public:
		Textbox(Graphics::Font fnt);
		~Textbox();
		
		void set_text(char *new_msg);

		void update(unsigned dt);
		void render(unsigned dt);
	};
}
#endif