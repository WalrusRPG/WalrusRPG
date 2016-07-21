#ifndef INCLUDE_ENTITY_H
#define INCLUDE_ENTITY_H

#include "render/Camera.h"
#include "render/TileRenderer.h"
#include "utility/Rect.h"

namespace WalrusRPG
{
    namespace States
    {
        class StateMap;
    }
    enum InteractionType
    {
        UNKNOWN,
        CHECK
    };
    /**
         * Well, for now, this class will be a non abstract for ALPHA PROGRAMMING REASONS.
         * Expect this sooner or later to be abstract.
         * I don't know at this moment how will we manage the different classes heriting
     * or compositing from this, if we use components or heritance.
         */
    class Entity
    {
      protected:
        unsigned sprite_id;
        States::StateMap &container;

      public:
        WalrusRPG::Renderer *tset;
        // position of top-left corner
        float x, y;

        // dimensions
        unsigned w, h;

        // velocity
        float vx, vy;

        bool moving;
        bool solid;

        Entity(States::StateMap &container, float x, float y, unsigned w, unsigned h,
               WalrusRPG::Renderer *tset, unsigned sprite_id);
        virtual ~Entity();
        virtual void render(Camera &camera) const;
        virtual void update();
        virtual void interact_with(Entity &origin, InteractionType type);
    };
}

#endif
