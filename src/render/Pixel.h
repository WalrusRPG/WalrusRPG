#ifndef INCLUDE_PIXEL_H
#define INCLUDE_PIXEL_H

#include <cstdint>

namespace WalrusRPG
{
    namespace Graphics
    {
        /*
         * Pixel structure
         */
        class Pixel
        {
          public:
            union
            {
                std::uint16_t value;

              public: // hack to be able to do pixel.r. Clever!
                struct
                {
                    unsigned b : 5;
                    unsigned g : 6;
                    unsigned r : 5;
                };
            };

            Pixel(std::uint16_t color);

            Pixel(Pixel &pix);

            Pixel(std::uint8_t red, std::uint8_t green, std::uint8_t blue);

            // Overloading (unsigned) typecast
            operator std::uint16_t() const;

            Pixel &operator=(unsigned value);

            bool operator==(const Pixel &col);
        };

        extern const Pixel Black;
        extern const Pixel LightGray;
        extern const Pixel Gray;
        extern const Pixel DarkGray;
        extern const Pixel White;
        extern const Pixel Red;
        extern const Pixel Green;
        extern const Pixel Blue;
        extern const Pixel Yellow;
        extern const Pixel Cyan;
        extern const Pixel Magenta;
    }
}


#endif
