#ifndef INCLUDE_ENTITY_H
#define INCLUDE_ENTITY_H

#include "Rect.h"
#include "Camera.h"
#include "Tileset.h"

namespace WalrusRPG
{
    /**
	 * Well, for now, this class will be a non abstract for ALPHA PROGRAMMING REASONS.
	 * Expect this sooner or later to be abstract.
	 * I don't know at this moment how will we manage the different classes heriting or compositing from this, if we use components or heritance.
	 */
    class Entity
    {
      protected:
        WalrusRPG::Utils::Rect coords;
        WalrusRPG::Tileset *tset;
        unsigned sprite_id;

      public:
        Entity(int x, int y, unsigned w, unsigned h, WalrusRPG::Tileset *tset, unsigned sprite_id);
        ~Entity();
        void render(Camera &camera, unsigned dt) const;
        void update(unsigned dt);
    };
}

#endif
