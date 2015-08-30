#include "Texture.h"
#include "utility/Rect.h"
#include <cstdint>

using WalrusRPG::Graphics::Texture;
using WalrusRPG::Utils::Rect;

namespace {
    /*texture_type_t*/ char* loadPNG(char *data) {
        // stuff
        return nullptr;
    }
}

Texture::Texture(char *data)
{
    // TODO
    // load data from texture
    // texture = loadPNG(data);
}

Texture::~Texture()
{
    // destroy(texture);
}

WalrusRPG::Utils::Rect Texture::get_dimensions()
{
    // return Rect(0, 0, texture[0], texture[1]);
    return Rect(0, 0, 0, 0);
}
