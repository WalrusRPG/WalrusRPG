#ifndef INCLUDE_FONT_H
#define INCLUDE_FONT_H

#include "Graphics.h"
#include "Texture.h"
#include "piaf/Archive.h"
#include "utility/Rect.h"

namespace WalrusRPG
{
    namespace Graphics
    {
        struct CharacterParameters
        {
            // Sprite clip
            WalrusRPG::Utils::Rect dimensions;
            // Character rendering offset
            int16_t x_offset, y_offset;
        };

        class Font
        {
          public:
            // Font height.
            uint8_t baseline;
            // Variable to override space's rendering width.
            uint8_t space_width;
            // Character dimensions/offset container.
            CharacterParameters chars[256];
            // Font texture
            // TODO?: Determine if using a reference or a variable.
            WalrusRPG::Graphics::Texture &font_tex;

            Font(WalrusRPG::Graphics::Texture &font_tex,
                 WalrusRPG::PIAF::File font_config);
            ~Font();

            void draw(uint16_t x, uint16_t y, const char c) const;
            void draw(uint16_t x, uint16_t y, const char c,
                      const WalrusRPG::Graphics::Pixel &col) const;
            void draw(uint16_t x, uint16_t y, const char *str) const;
            void draw(uint16_t x, uint16_t y, const char *str,
                      const WalrusRPG::Graphics::Pixel &col) const;
            void draw_format(uint16_t x, uint16_t y, const char *format, ...) const;
            void draw_format(uint16_t x, uint16_t y,
                             const WalrusRPG::Graphics::Pixel &col, const char *format,
                             ...) const;
        };

        class FontException : public std::exception
        {
          private:
            char msg[1024];

          public:
            FontException(const char *format, ...);
            virtual ~FontException();

            const char *what() const throw();
        };
    }
}

#endif