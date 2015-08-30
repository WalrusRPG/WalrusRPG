#ifndef INCLUDE_TEXTURE_H
#define INCLUDE_TEXTURE_H

/*
 * Texture.h
 * Texture backend abstraction
 */

#include "utility/Rect.h"
//#include "PLATFORM/texture_type.h"

namespace WalrusRPG
{
    namespace Graphics
    {
        // This class only exists to get the common functions
        class Texture
        {
          private:
            // texture_type_t texture;
          public:
            // The get function is implemented by the child
            // TextureHolder &get_texture();
            Texture(char *data);
            ~Texture();
            WalrusRPG::Utils::Rect get_dimensions();
        };
    }
}

#endif
