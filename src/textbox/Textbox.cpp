#include "Textbox.h"
#include <cstring>
#include <cstdlib>
#include "utility/misc.h"

using WalrusRPG::MAGIC_TOKEN;
using WalrusRPG::COMMAND_LEGNTH;
using WalrusRPG::Textbox;
using WalrusRPG::Graphics::Font;

namespace
{
	size_t strlen_tokens(const char *str)
	{
		size_t len = 0;
		for(;str[len];++len)
		{
			if(str[len] == MAGIC_TOKEN)
				len +=4;
		}
		return len;
	}
}

Textbox::Textbox(Font fnt)
: fnt(fnt), buffer(), buffer_index(0), current_color(0, 0, 0), letter_wait(0), letter_wait_cooldown(10)
{
}

Textbox::~Textbox()
{
}

void Textbox::set_text(char *new_text)
{
	letter_wait = 0;
	letter_wait_cooldown = 10;
	buffer_index = 0;
	buffer.clear();
	for (size_t i = 0; i < strlen_tokens(new_text); ++i)
	{
		TextboxChar t;
		if(new_text[i] == MAGIC_TOKEN)
		{
			t.c = MAGIC_TOKEN;
			t.routine = new_text[i+1];
			t.arg1 = new_text[i+2];
			t.arg2 = new_text[i+3];
			t.arg3 = new_text[i+4];
			i += COMMAND_LEGNTH;
			// printf("t.routine : %02x\n", t.routine);
		}
		else
		{
			t.c = new_text[i];
			t.routine = 0;
			t.arg1 = 0;
			t.arg2 = 0;
			t.arg3 = 0;
			// printf("t.c : %c\n", t.c);
		}
		buffer.push_back(t);
	}
}


void Textbox::update(unsigned dt)
{
	if(buffer_index >= buffer.size())
		return;
	letter_wait -= dt;
	if(letter_wait <= 0)
	{
		unsigned add = (-letter_wait)/letter_wait_cooldown + 1;
		buffer_index = (buffer_index+add < buffer.size()? buffer_index+add : buffer.size());
		letter_wait = letter_wait_cooldown;

		if(buffer[buffer_index].c == MAGIC_TOKEN)
		{
			switch(buffer[buffer_index].routine)
			{
				// wait a bit
				case 0x81:
				letter_wait = buffer[buffer_index].arg1;
				break;
			}
			buffer_index++;
		}
	}
}

void Textbox::render(unsigned dt)
{
	unsigned cur_x = 0;
	unsigned cur_y = 0;
	current_color = 0xFFFF;

	for (unsigned i = 0; i < buffer_index; ++i)
	{
		char c = buffer[i].c;
		if( c == MAGIC_TOKEN) {
			switch(buffer[i].routine)
			{
				// Change current color
				case 0x01:
				current_color = ((buffer[i].arg1<<8) + buffer[i].arg2);
				break;				
			}
			continue;
		}

		if(fnt.chars[c].dimensions.width + cur_x > 320)
		{
			cur_x = 0;
			cur_y += fnt.baseline;
		}
		fnt.draw(cur_x, cur_y, c, current_color);
		if(c == ' ')
			cur_x += fnt.space_width;
		else
			cur_x += fnt.chars[(unsigned char)c].dimensions.width + 1;
	}
}