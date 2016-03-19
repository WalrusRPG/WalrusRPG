#include "Pixel.h"

using WalrusRPG::Graphics::Pixel;

Pixel::Pixel(std::uint16_t color) : value(color)
{
}

Pixel::Pixel(Pixel &pix) : value(pix.value)
{
}

Pixel::Pixel(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
    : b(blue >> 3), g(green >> 2), r(red >> 3)
{
}

Pixel::operator std::uint16_t() const
{
    return value;
}

Pixel &Pixel::operator=(unsigned value)
{
    this->value = value;
    return *this;
}

bool Pixel::operator==(const Pixel &col)
{
    return value == col.value;
}

#define CONST_COLOR(COLOR, r, g, b) const Pixel WalrusRPG::Graphics::COLOR(r, g, b)

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
