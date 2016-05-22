#ifndef INCLUDE_ENTITY_H
#define INCLUDE_ENTITY_H

#include "utility/Rect.h"
#include "render/Camera.h"
#include "render/TileRenderer.h"

namespace WalrusRPG
{
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


      public:
        WalrusRPG::Renderer *tset;
        // position of top-left corner
        float x, y;

        // dimensions
        unsigned w, h;

        // velocity
        float vx, vy;

        Entity(float x, float y, unsigned w, unsigned h, WalrusRPG::Renderer *tset,
               unsigned sprite_id);
        ~Entity();
        void render(Camera &camera, unsigned dt) const;
        void update(unsigned dt);
    };
}

#endif
