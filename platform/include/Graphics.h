#ifndef INCLUDE_GRAPHICS_H
#define INCLUDE_GRAPHICS_H

/*
 * Graphics.h
 * Graphics backend abstraction
 */

#include "Texture.h"
#include "render/Pixel.h"
#include "utility/Rect.h"
#include <cstdint>

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
        void put_sprite(const WalrusRPG::Graphics::Texture &sheet, int x, int y,
                        const WalrusRPG::Utils::Rect &window);

        void put_sprite_clipping(const Texture &sheet, int x, int y,
                                 const WalrusRPG::Utils::Rect &sprite_window,
                                 const WalrusRPG::Utils::Rect &clipping_window);
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

        // Primitives drawing

        /*
         * Draws an horizontal line on the screen.
         */
        void put_horizontal_line(uint16_t x, uint16_t x2, uint16_t y,
                                 const WalrusRPG::Graphics::Pixel &color);

        /*
         * Draws an vertical line on the screen.
         */
        void put_vertical_line(uint16_t x, uint16_t y, uint16_t y2,
                               const WalrusRPG::Graphics::Pixel &color);

        /*
         * Draws a line on the screen. As it uses Bresenham's algorithm, it won't be the
         * most optimized way
         * to draw vertical or horizontal lines.
         */
        void put_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2,
                      const WalrusRPG::Graphics::Pixel &color);

        /*
         * Draws a filled rectangle on the screen.
         */
        void put_rectangle(const WalrusRPG::Utils::Rect &rect,
                           const WalrusRPG::Graphics::Pixel &color);
    }
}

#endif
