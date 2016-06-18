#ifndef INCLUDE_PLAYER_ENTITY_H
#define INCLUDE_PLAYER_ENTITY_H

#include "Entity.h"
#include "render/Animator.h"

namespace WalrusRPG
{
    class Map;
    class PlayerEntity : public Entity
    {
      protected:
        Animator animator;

      public:
        bool controllable;
        int direction;
        PlayerEntity(States::StateMap &container, float x, float y, unsigned w,
                     unsigned h, WalrusRPG::Renderer *tset, unsigned sprite_id);
        ~PlayerEntity();
        virtual void update() override;
        virtual void render(Camera &camera) const override;
    };
}
#endif