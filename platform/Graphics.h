#ifndef INCLUDE_GRAPHICS_H
#define INCLUDE_GRAPHICS_H

/*
 * Graphics.h
 * Graphics backend abstraction
 */

#include <cstdint>
#include "render/Pixel.h"
#include "utility/Rect.h"

namespace WalrusRPG
{
    namespace Graphics
    {
        void init();
        void deinit();

        /*
         * Allows doing stuff before drawing, the GC requires this,
         * and it's a nice to have
         */
        void frame_begin();

        /*
         * Signal the graphics system that we're done drawing so that
         * the frame can be pushed to the screen
         */
        void frame_end();

        /*
         * Current prototype is the same as the nspire fb implementation
         * for now, need to add a texture type and get rid of the Rect
         * (not sure the GC supports drawing parts of a texture, but it
         * may be worth trying
         */
        void put_sprite(const uint16_t *sheet, int x, int y,
                        const WalrusRPG::Utils::Rect &window);

        /*
         * Set a background color or texture to be used by frame_begin()
         * To be defined further
         */
        void set_bg(const WalrusRPG::Graphics::Pixel &new_bg);
    }
}

#endif
