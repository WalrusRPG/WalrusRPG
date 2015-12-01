#ifndef INCLUDE_GRAPHICS_H
#define INCLUDE_GRAPHICS_H

/*
 * Graphics.h
 * Graphics backend abstraction
 */

#include <cstdint>
#include "render/Pixel.h"
#include "utility/Rect.h"
#include "Texture.h"

namespace WalrusRPG
{
    namespace Graphics
    {
        void init();
        void deinit();

        /*
         * Allows doing stuff before drawing, the GC requires this,
         * and it's a nice to have
         */
        void frame_begin();

        /*
         * Signal the graphics system that we're done drawing so that
         * the frame can be pushed to the screen
         */
        void frame_end();

        /*
         * Draws a sprite with clipping given as window.
         */
        void put_sprite(const Texture &sheet, int x, int y,
                        const WalrusRPG::Utils::Rect &window);

        /*
         * Draws a sprite with clipping given as window and color
         * tinting.
         */
        void put_sprite_tint(const Texture &sheet, int x, int y,
                             const WalrusRPG::Utils::Rect &window,
                             const WalrusRPG::Graphics::Pixel &color);

        /*
         * Fill the screen with a color
         */
        void fill(const WalrusRPG::Graphics::Pixel &color);

        /*
         * Draws a pixel on the screen.
         */
        void put_pixel(uint16_t x, uint16_t y, const WalrusRPG::Graphics::Pixel &color);
    }
}

#endif
