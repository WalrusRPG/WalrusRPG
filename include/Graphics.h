#ifndef INCLUDE_GRAPHICS_H
#define INCLUDE_GRAPHICS_H

namespace WalrusRPG
{
    namespace Graphics
    {
        typedef struct Rect Rect_t;
        struct Rect
        {
            int x, y;
            unsigned w, h;
        };

        /*
	 * Buffer management
	 */

        void buffer_allocate();
        void buffer_free();
        void buffer_swap();
        void buffer_fill(unsigned color);


        /*
	 * Misc LCD functions
	 */

        void lcd_vsync();


        /*
	 * Drawing
	 */

        void draw_pixel(unsigned x, unsigned y, unsigned short color);
        void draw_sprite_sheet(const unsigned short *sheet, int x, int y, const Rect_t *window);


        /*
	 * Sprite manipulation
	 */

        unsigned short sprite_pixel_get(const unsigned short *sprite, unsigned x, unsigned y);
    }
}

#endif
