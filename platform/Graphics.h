#ifndef INCLUDE_GRAPHICS_H
#define INCLUDE_GRAPHICS_H

#include <cstdint>
#include "utility/Rect.h"

namespace WalrusRPG
{
    namespace Graphics
    {
        /*
         * Buffer management
         */

        void buffer_allocate();
        void buffer_free();
        void buffer_swap_screen();
        void buffer_swap_render();
        void buffer_fill(uint16_t color);


        /*
         * Misc LCD functions
         */
        void vsync_isr();


        /*
         * Drawing
         */

        void draw_pixel(int x, int y, uint16_t color);
        void draw_sprite_sheet(const uint16_t *sheet, int x, int y,
                               const WalrusRPG::Utils::Rect &window);


        /*
         * Sprite manipulation
         */

        uint16_t sprite_pixel_get(const uint16_t *sprite, uint32_t x, uint32_t y);
    }
}

#endif
