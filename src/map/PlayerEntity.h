#ifndef INCLUDE_PLAYER_ENTITY_H
#define INCLUDE_PLAYER_ENTITY_H

#include "Entity.h"

namespace WalrusRPG
{
    class Map;
    class PlayerEntity : public Entity
    {
      public:
        PlayerEntity(float x, float y, unsigned w, unsigned h, WalrusRPG::Renderer *tset,
                     unsigned sprite_id);
        ~PlayerEntity();
        void update(unsigned dt) override;
        virtual void render(Camera &camera, unsigned dt) const override;
    };
}
#endif