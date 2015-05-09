#include <Pixel.h>

#define PIXEL WalrusRPG::Graphics::Pixel

PIXEL::Pixel(std::uint16_t color) : value(color) {

}

PIXEL::Pixel(Pixel &pix) : value((std::uint8_t)pix) {

}

PIXEL::Pixel(std::uint8_t red, std::uint8_t green, std::uint8_t blue) : r(red>>3), g(green>>2), b(blue>>3) {

}

PIXEL::operator std::uint16_t() const {
  return value;
}

PIXEL& PIXEL::operator=(unsigned value) {
  this->value = value;
	return *this;
}
