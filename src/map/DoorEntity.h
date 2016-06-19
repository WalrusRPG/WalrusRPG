#ifndef INCLUDE_DOOR_ENTITY_H
#define INCLUDE_DOOR_ENTITY_H

#include "Entity.h"
#include "render/Animator.h"

namespace WalrusRPG
{
    class Map;
    class DoorEntity : public Entity
    {
      protected:
        unsigned sprite_closed_id;
        unsigned sprite_opened_id;
        Animator animator;
        bool opened;

      public:
        const char *msg;
        DoorEntity(States::StateMap &container, float x, float y, unsigned w, unsigned h,
                   WalrusRPG::Renderer *tset, unsigned sprite_closed_id,
                   unsigned sprite_opened_id);
        ~DoorEntity();
        virtual void update() override;
        virtual void render(Camera &camera) const override;
        virtual void interact_with(Entity &origin, InteractionType type) override;
    };
}
#endif