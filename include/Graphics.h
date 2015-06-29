#ifndef INCLUDE_GRAPHICS_H
#define INCLUDE_GRAPHICS_H

#include "Rect.h"

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
        void buffer_fill(unsigned color);


        /*
	 * Misc LCD functions
	 */

        void lcd_vsync();
        void vsync_isr();


        /*
	 * Drawing
	 */

        void draw_pixel(unsigned x, unsigned y, unsigned short color);
        void draw_sprite_sheet(const unsigned short *sheet, int x, int y, const WalrusRPG::Utils::Rect &window);


        /*
	 * Sprite manipulation
	 */

        unsigned short sprite_pixel_get(const unsigned short *sprite, unsigned x, unsigned y);
    }
}

#endif
