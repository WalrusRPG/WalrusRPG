#ifndef INCLUDE_ENTITY_H
#define INCLUDE_ENTITY_H

#include "Camera.h"

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
        unsigned x;
        unsigned y;
        unsigned width;
        unsigned height;

      public:
        Entity();
        ~Entity();
        void render(Camera &camera, unsigned dt) const;
        void update(unsigned dt);
    };
}

#endif
