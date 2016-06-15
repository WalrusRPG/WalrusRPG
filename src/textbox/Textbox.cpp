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
using WalrusRPG::Utils::Rect;

namespace
{
    /**
     * This is a replacment of strlen to allow skipping tokens as they might
     * contain null-terminating characters, making strlen stop earlier than
     * planned.
     */
    signed strlen_tokens(const char *str)
    {
        signed len = 0;
        for (; str[len]; ++len)
        {
            if (str[len] == MAGIC_TOKEN)
                len += COMMAND_LEGNTH;
        }
        return len;
    }
}

Textbox::Textbox(Rect dimensions, Font fnt)
    : fnt(fnt), buffer(0), buffer_index(-1), global_string_offset(0),
      current_color(Graphics::White), letter_wait(0), letter_wait_cooldown(5),
      dimensions(dimensions), state(Waiting),
      color_before_line{{0xFFFF}, {0xFFFF}, {0xFFFF}}
{
}

Textbox::Textbox(Font fnt) : Textbox({4, 4, 220, 32}, fnt)
{
}

Textbox::~Textbox()
{
}

void Textbox::set_text(const char *new_text)
{
    // Clearing the state variables.
    letter_wait = 0;
    letter_wait_cooldown = 2;
    buffer_index = -1;
    global_string_offset = 0;
    nb_line_to_update = 0;
    for (unsigned i = 0; i < nb_lines; ++i)
    {
        line_nb_characters[i] = 0;
        line_widths[i] = 0;
    }

    buffer.clear();
    // Parsing the passed string into a token list.
    // TODO : Convert the vector into a dynamically allocated array?
    for (signed i = 0; i < strlen_tokens(new_text); ++i)
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

/**
 * Makes the text box advance of one or more characters/tokens.
 */
void Textbox::add_letter(unsigned nb_letters)
{
    if (state != Updating || buffer.size() <= 0)
        return;

    // Mmh, you who enters here, try to forget how the core logic is programmed.
    // Myself don't have a frigging clue on how to clean it but it *works*.
    // If you ever clean it, I'll be eternally thankful :-°
    // Actually, it works as it does right now, but the signedness is messy as hell
    // and changing it would most likely break it everywhere.
    for (unsigned i = 0;
         (i < nb_letters) &&
         (buffer_index < 0 || buffer_index < static_cast<signed>(buffer.size()) - 1) &&
         state == Updating;
         ++i)
    {
        // As the index starts with -1, increment it before doing anything.
        ++buffer_index;
        // Parsing commands.
        if (buffer[buffer_index].c == MAGIC_TOKEN)
        {
            letter_wait = letter_wait_cooldown;
            switch (buffer[buffer_index].routine)
            {
                // wait a bit
                case 0x81:
                    letter_wait = buffer[buffer_index].arg1;
                    // Override the "skip this character"
                    i++;
                    break;
                // Force Full state
                case 0x82:
                    state = Full;
                    // Override the "skip this character"
                    i++;
                    break;
                // Puts the character to the origin. Best used after 0x82
                case 0x83:
                    nb_line_to_update = 0;
                    line_nb_characters[0] = 0;
            }
            line_nb_characters[nb_line_to_update]++;
            i--;
        }
        else
        {
            // The frigging static cast...
            CharacterParameters &p =
                fnt.chars[static_cast<signed>(buffer[buffer_index].c)];
            TextboxChar &t = buffer[buffer_index];
            // Manual line-return.
            if (t.c == '\n')
            {
                if (nb_line_to_update + 1 >= nb_lines)
                {
                    // No need to go back in the array. Just have to go forward.
                    line_nb_characters[nb_line_to_update]++;
                    state = Full;
                    return;
                }
                nb_line_to_update++;
            }
            // If adding the character would make the line too big for the text_box
            if (line_widths[nb_line_to_update] + p.dimensions.width + 1 >
                dimensions.width)
            {
                if (nb_line_to_update + 1 >= nb_lines)
                {
                    // Here to avoid getting to lose that character (or the last ones)
                    // We have to put the reading head one character backwards.
                    --buffer_index;
                    state = Full;
                    return;
                }
                nb_line_to_update++;
            }
            // Just adding the correct space width
            if (t.c == ' ')
                line_widths[nb_line_to_update] += fnt.space_width;
            else
                line_widths[nb_line_to_update] += p.dimensions.width + 1;
            // Putting the parsed character in the current text line.
            line_nb_characters[nb_line_to_update]++;
            letter_wait = letter_wait_cooldown;
        }
    }
    // Check if the text box finished its work
    if (buffer_index >= static_cast<signed>(buffer.size() - 1))
    {
        state = Done;
    }
    // You prefer having to wait for characters, no?
}

void Textbox::update(unsigned dt)
{
    // Small state machine.
    switch (state)
    {
        case Waiting:
            return;
            break;
        case Updating:
            // Time-based update.
            if ((buffer_index >= 0) &&
                (buffer_index >= static_cast<signed>(buffer.size())))
                return;
            letter_wait -= dt;
            if (letter_wait <= 0)
            {
                unsigned add = (-letter_wait) / letter_wait_cooldown + 1;
                add_letter(add);
            }
            break;
        case Full:
            // TODO?: Change the trigger (button) into something else (like a function)?
            if (key_pressed(Key::K_A))
            {
                for (unsigned i = 0; i < nb_lines - 1; ++i)
                    global_string_offset += line_nb_characters[i];

                line_widths[0] = line_widths[nb_lines - 1];
                line_nb_characters[0] = line_nb_characters[nb_lines - 1];
                color_before_line[0] = color_before_line[nb_lines - 1];

                for (unsigned i = 1; i < nb_lines; ++i)
                {
                    line_widths[i] = 0;
                    line_nb_characters[i] = 0;
                }
                nb_line_to_update = 1;
                state = Updating;
            }
        default:
            break;
    }
}

void Textbox::render(unsigned dt)
{
    UNUSED(dt);
    if (buffer_index < 0)
        return;
    // TODO : store the last character's color to correctly reapply it if a line return
    // happens?
    put_rectangle(dimensions, Graphics::Black);
    unsigned global_index = global_string_offset;
    for (unsigned l = 0; l < nb_lines; l++)
    {
        unsigned cur_x = dimensions.x;
        unsigned cur_y = dimensions.y + l * fnt.baseline;
        current_color = color_before_line[l];
        for (unsigned line_index = 0; line_index < line_nb_characters[l]; ++line_index)
        {
            TextboxChar b = buffer[global_index + line_index];
            char c = b.c;
            if (c == MAGIC_TOKEN)
            {
                switch (b.routine)
                {
                    // Change current color (16-bit mode)
                    case 0x01:
                        current_color = ((b.arg1 << 8) + b.arg2);
                        break;
                    // Change current color (RGB mode)
                    case 0x02:
                        current_color = {b.arg1, b.arg2, b.arg3};
                        break;
                }
                continue;
            }
            fnt.draw(cur_x, cur_y, c, current_color);
            // *shrugs*
            if (c == '\n')
                continue;
            else if (c == ' ')
                cur_x += fnt.space_width;
            else
                cur_x += fnt.chars[static_cast<signed>(c)].dimensions.width + 1;
        }
        global_index += line_nb_characters[l];
        if (l < nb_lines - 1)
            color_before_line[l + 1] = current_color;
    }
    // State indicator.
    Pixel indicator_color = Graphics::Black;
    if (state == Full)
        indicator_color = Graphics::Red;
    else if (state == Done)
    {
        indicator_color = Graphics::Blue;
    }

    if (indicator_color != Graphics::Black)
        put_rectangle({dimensions.x + static_cast<signed>(dimensions.width) - 3,
                       dimensions.y + static_cast<signed>(dimensions.height) - 3, 3, 3},
                      indicator_color);
}
