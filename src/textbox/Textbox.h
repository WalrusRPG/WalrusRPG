#ifndef INCLUDE_TEXTBOX_H
#define INCLUDE_TEXTBOX_H

#include "Graphics.h"
#include "render/Font.h"
#include "utility/Rect.h"
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

    enum TextboxState
    {
        Waiting,
        Updating,
        Full,
        Done
    };

    /**
     * Textbox is a class doing what a classic text box should do in a game,
     * which means showing text, character(s) after character(s), applying
     * effects to the text and so on.
     */
    class Textbox
    {
        // TODO?: Rename it into A CONSTANT_NAME
        static constexpr unsigned nb_lines = 3;

      private:
        Graphics::Font fnt;
        tinystl::vector<TextboxChar> buffer;
        // Global text index position.
        signed buffer_index;
        // This allows not losing our progress in the text to render the right section.
        unsigned global_string_offset;
        // Current text container to write characters in.
        unsigned nb_line_to_update;
        unsigned line_nb_characters[nb_lines];
        unsigned line_widths[nb_lines];
        Graphics::Pixel current_color;
        signed letter_wait;
        signed letter_wait_cooldown;
        Utils::Rect dimensions;

        void add_letter(unsigned nb_letters);

      public:
        TextboxState state;
        Textbox(Graphics::Font fnt);
        ~Textbox();

        void set_text(char *new_msg);

        void update(unsigned dt);
        void render(unsigned dt);
    };
}
#endif