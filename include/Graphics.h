#ifndef INCLUDE_GRAPHICS_H
#define INCLUDE_GRAPHICS_H

namespace WalrusRPG{ namespace Graphics {


	/*
	 * Pixel structure
	 * TODO?: Convert this into a class to hide value?
	 */
	union Pixel {
		::uint16_t value;
		struct {
			unsigned r : 5;
			unsigned g : 6;
			unsigned b : 5;
		};

		Pixel(::uint8_t red, ::uint8_t green, ::uint8_t blue) : r(red>>3), g(green>>2), b(blue>>3) {

		}

		// Overloading (unsigned) typecast
		operator ::uint16_t() {
			return value;
		}
		Pixel& operator=(unsigned value) {
			this->value = value;
			return *this;
		}
	};

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
