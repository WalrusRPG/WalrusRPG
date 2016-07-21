#ifndef INCLUDE_CXFB_H
#define INCLUDE_CXFB_H

#include "utility/Rect.h"
#include <cstdint>

namespace Nspire
{
    namespace CXfb
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
        void draw_pixel_tint(int x, int y, uint16_t color, uint16_t tint);
        void draw_sprite_sheet(const uint16_t *sheet, int x, int y,
                               const WalrusRPG::Utils::Rect &window);
        void draw_sprite_sheet_tint(const uint16_t *sheet, int x, int y,
                                    const WalrusRPG::Utils::Rect &window, uint16_t tint);


        /*
         * Sprite manipulation
         */

        uint16_t sprite_pixel_get(const uint16_t *sprite, uint32_t x, uint32_t y);
    }
}

#endif
