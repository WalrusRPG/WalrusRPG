#include "Pixel.h"

#define PIXEL WalrusRPG::Graphics::Pixel

PIXEL::Pixel(std::uint16_t color) : value(color)
{
}

PIXEL::Pixel(Pixel &pix) : value(pix.value)
{
}

PIXEL::Pixel(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
    : b(blue >> 3), g(green >> 2), r(red >> 3)
{
}

PIXEL::operator std::uint16_t() const
{
    return value;
}

PIXEL &PIXEL::operator=(unsigned value)
{
    this->value = value;
    return *this;
}

bool PIXEL::operator==(const PIXEL& col)
{
	return value == col.value;
}

#define CONST_COLOR(color, r, g, b) \
    const WalrusRPG::Graphics::Pixel WalrusRPG::Graphics::color(r, g, b)
CONST_COLOR(Black, 0, 0, 0);
CONST_COLOR(DarkGray, 64, 64, 64);
CONST_COLOR(Gray, 128, 128, 128);
CONST_COLOR(LightGray, 192, 192, 192);
CONST_COLOR(White, 255, 255, 255);

CONST_COLOR(Red, 255, 0, 0);
CONST_COLOR(Green, 0, 255, 0);
CONST_COLOR(Blue, 0, 0, 255);

CONST_COLOR(Yellow, 255, 255, 0);
CONST_COLOR(Cyan, 0, 255, 255);
CONST_COLOR(Magenta, 255, 0, 255);

#undef CONST_COLOR
