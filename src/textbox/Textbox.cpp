#include "Textbox.h"
#include <cstring>
#include <cstdlib>
#include "utility/misc.h"
#include "input/Input.h"

using WalrusRPG::MAGIC_TOKEN;
using WalrusRPG::COMMAND_LEGNTH;
using WalrusRPG::Textbox;
using WalrusRPG::TextboxState;
using WalrusRPG::TextboxChar;
using WalrusRPG::Graphics::Font;
using WalrusRPG::Graphics::CharacterParameters;
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::Input::Key;

namespace
{
    size_t strlen_tokens(const char *str)
    {
        size_t len = 0;
        for (; str[len]; ++len)
        {
            if (str[len] == MAGIC_TOKEN)
                len += COMMAND_LEGNTH;
        }
        return len;
    }
}

Textbox::Textbox(Font fnt)
    : fnt(fnt), buffer(0), buffer_index(-1), current_color(0, 0, 0), letter_wait(0),
      letter_wait_cooldown(10), dimensions(40, 4, 200, 32), state(Waiting),
      global_string_offset(0)
{
}

Textbox::~Textbox()
{
}

void Textbox::set_text(char *new_text)
{
    letter_wait = 0;
    letter_wait_cooldown = 10;
    buffer_index = -1;
    global_string_offset = 0;
    nb_line_to_update = 0;
    for (int i = 0; i < nb_lines; ++i)
    {
        line_nb_characters[i] = 0;
        line_widths[i] = 0;
    }

    buffer.clear();
    for (size_t i = 0; i < strlen_tokens(new_text); ++i)
    {
        TextboxChar t;
        if (new_text[i] == MAGIC_TOKEN)
        {
            t.c = MAGIC_TOKEN;
            t.routine = new_text[i + 1];
            t.arg1 = new_text[i + 2];
            t.arg2 = new_text[i + 3];
            t.arg3 = new_text[i + 4];
            i += COMMAND_LEGNTH;
        }
        else
        {
            t.c = new_text[i];
            t.routine = 0;
            t.arg1 = 0;
            t.arg2 = 0;
            t.arg3 = 0;
        }
        buffer.push_back(t);
    }
    state = Updating;
}

void Textbox::add_letter(unsigned nb_letters)
{
    if (state != Updating)
        return;
    if (buffer.size() <= 0)
        return;
    for (unsigned i = 0;
         (i < nb_letters) && (buffer_index < 0 || buffer_index < buffer.size() - 1); ++i)
    {
        ++buffer_index;
        if (buffer[buffer_index].c == MAGIC_TOKEN)
        {
            switch (buffer[buffer_index].routine)
            {
                // wait a bit
                case 0x81:
                    letter_wait = buffer[buffer_index].arg1;
                    break;
            }
            line_nb_characters[nb_line_to_update]++;
        }
        else
        {
            CharacterParameters &p = fnt.chars[buffer[buffer_index].c];
            TextboxChar &t = buffer[buffer_index];

            if (t.c == '\n')
            {
                if (nb_line_to_update + 1 >= nb_lines)
                {
                    line_nb_characters[nb_line_to_update]++;
                    // --buffer_index;
                    state = Full;
                    return;
                }
                nb_line_to_update++;
            }
            if (line_widths[nb_line_to_update] + p.dimensions.width + 1 >
                dimensions.width)
            {
                if (nb_line_to_update + 1 >= nb_lines)
                {
                    --buffer_index;
                    state = Full;
                    return;
                }
                nb_line_to_update++;
            }
            if (t.c == ' ')
                line_widths[nb_line_to_update] += fnt.space_width;
            else
                line_widths[nb_line_to_update] += p.dimensions.width + 1;

            line_nb_characters[nb_line_to_update]++;
        }
    }
    if (buffer_index >= buffer.size() - 1)
    {
        state = Done;
    }
    letter_wait = letter_wait_cooldown;
}

void Textbox::update(unsigned dt)
{
    switch (state)
    {
        case Waiting:
            return;
            break;
        case Updating:
            if ((buffer_index >= 0) && (buffer_index >= buffer.size()))
                return;
            letter_wait -= dt;
            if (letter_wait <= 0)
            {
                unsigned add = (-letter_wait) / letter_wait_cooldown + 1;
                add_letter(add);
            }
            break;
        case Full:
            if (key_pressed(Key::K_A))
            {
                for (int i = 0; i < nb_lines - 1; ++i)
                    global_string_offset += line_nb_characters[i];

                line_widths[0] = line_widths[nb_lines - 1];
                line_nb_characters[0] = line_nb_characters[nb_lines - 1];

                for (int i = 1; i < nb_lines; ++i)
                {
                    line_widths[i] = 0;
                    line_nb_characters[i] = 0;
                }
                nb_line_to_update = 1;
                state = Updating;
            }
    }
}

void Textbox::render(unsigned dt)
{
    if (buffer_index < 0)
        return;
    current_color = 0xFFFF;
    put_rectangle(dimensions, Graphics::DarkGray);
    // signed cur_x_max = cur_x + dimensions.width;
    unsigned global_index = global_string_offset;
    for (unsigned l = 0; l < nb_lines; l++)
    {
        unsigned cur_x = dimensions.x;
        unsigned cur_y = dimensions.y + l * fnt.baseline;
        for (int line_index = 0; line_index < line_nb_characters[l]; ++line_index)
        {
            TextboxChar b = buffer[global_index + line_index];
            char c = b.c;
            if (c == MAGIC_TOKEN)
            {
                switch (b.routine)
                {
                    // Change current color
                    case 0x01:
                        current_color = ((b.arg1 << 8) + b.arg2);
                        break;
                }
                continue;
            }
            fnt.draw(cur_x, cur_y, c, current_color);

            if (c == '\n')
                continue;
            else if (c == ' ')
                cur_x += fnt.space_width;
            else
                cur_x += fnt.chars[c].dimensions.width + 1;
        }
        global_index += line_nb_characters[l];
    }
    if (state == Full)
    {
        put_rectangle({dimensions.x + static_cast<signed>(dimensions.width),
                       dimensions.y + static_cast<signed>(dimensions.height), 3, 3},
                      Graphics::Red);
    }
    if (state == Done)
    {
        put_rectangle({dimensions.x + static_cast<signed>(dimensions.width),
                       dimensions.y + static_cast<signed>(dimensions.height), 3, 3},
                      Graphics::Blue);
    }
}