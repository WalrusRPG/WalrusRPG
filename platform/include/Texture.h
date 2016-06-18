#ifndef INCLUDE_TEXTURE_H
#define INCLUDE_TEXTURE_H

/*
 * Texture.h
 * Texture backend abstraction
 */

#include "utility/Rect.h"
#include "platform.h"
#include "render/Pixel.h"
#include "piaf/Archive.h"
//#include "PLATFORM/texture_type.h"

namespace WalrusRPG
{
    namespace Graphics
    {
        // This class only exists to get the common functions
        class Texture
        {
          private:
          public:
            // Mmmh, the smell of that awesome platform-based hack.
            texture_data_t data;

            Texture(WalrusRPG::PIAF::File entry);
            Texture(char *data = nullptr);
            ~Texture();
            // Getters
            const WalrusRPG::Utils::Rect get_dimensions();
            const WalrusRPG::Graphics::Pixel get_pixel(unsigned x, unsigned y);
        };
    }
}

#endif
