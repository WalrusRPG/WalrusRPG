#ifndef INCLUDE_TALK_ENTITY_H
#define INCLUDE_TALK_ENTITY_H

#include "Entity.h"
#include "render/Animator.h"

namespace WalrusRPG
{
    class Map;
    class TalkEntity : public Entity
    {
      protected:
        Animator animator;

      public:
        const char *msg;
        TalkEntity(States::StateMap &container, float x, float y, unsigned w, unsigned h,
                   WalrusRPG::Renderer *tset, unsigned sprite_id, const char *msg);
        ~TalkEntity();
        virtual void update(unsigned dt) override;
        virtual void render(Camera &camera, unsigned dt) const override;
        virtual void interact_with(Entity &origin, InteractionType type) override;
    };
}
#endif