#ifndef INCLUDE_STATEMAP_H
#define INCLUDE_STATEMAP_H

#include "engine/State.h"
#include "piaf/Archive.h"
#include "Map.h"
#include "render/Font.h"
#include "textbox/Textbox.h"

namespace WalrusRPG
{
    namespace States
    {
        class StateMap : public State
        {
          protected:
            bool started;
            Camera camera;
            Map &map;
#if TARGET_SFML
            unsigned active_map_mode;
            ImVec2 scrolling = ImVec2(0.0f, 0.0f);
#endif
            WalrusRPG::PIAF::Archive data;
            WalrusRPG::Graphics::Texture tex_haeccity;
            WalrusRPG::Graphics::Font txt;
            WalrusRPG::Textbox box;

          public:
            StateMap(int x, int y, Map &map);
            void render(unsigned dt);
            void update(unsigned dt);
#if TARGET_SFML
            void draw_background_tile(unsigned mode, float x, float y, float x2, float y2,
                                      unsigned tile);
            void draw_front_tile(unsigned mode, float x, float y, float x2, float y2,
                                 unsigned tile);

            void debug(unsigned dt);
#endif
            void putchar_haeccity(unsigned char c, unsigned x, unsigned y);
            void putchar_haeccity_tint(unsigned char c, unsigned x, unsigned y,
                                       const WalrusRPG::Graphics::Pixel &col);
            void print_haeccity(const char *str, unsigned x, unsigned y,
                                const WalrusRPG::Graphics::Pixel &col);
        };
    }
}
#endif
