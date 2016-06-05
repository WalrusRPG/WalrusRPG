#ifndef INCLUDE_STATEMAP_H
#define INCLUDE_STATEMAP_H

#include "engine/State.h"
#include "piaf/Archive.h"
#include "Map.h"
#include "render/Font.h"
#include "textbox/Textbox.h"
#include "PlayerEntity.h"

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
            WalrusRPG::PIAF::Archive *data;
            WalrusRPG::Graphics::Texture tex_haeccity;
            WalrusRPG::Graphics::Font txt;
            WalrusRPG::Textbox box;
            WalrusRPG::PlayerEntity p;

          public:
            StateMap(int x, int y, Map &map);
            void render(unsigned dt);
            void update(unsigned dt);
            void putchar_haeccity(unsigned char c, unsigned x, unsigned y);
            void putchar_haeccity_tint(unsigned char c, unsigned x, unsigned y,
                                       const WalrusRPG::Graphics::Pixel &col);
            void print_haeccity(const char *str, unsigned x, unsigned y,
                                const WalrusRPG::Graphics::Pixel &col);
        };
    }
}
#endif
