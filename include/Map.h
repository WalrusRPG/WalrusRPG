#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include "Camera.h"
#include "Entity.h"

namespace WalrusRPG
{
	class Map
	{
		protected:
			// <Tiles> data;
			// <Tileset> tileset;
			unsigned int width;
			unsigned int height;
			unsigned *layer0;
			unsigned *layer1;

		public:
			Map(int width, int height, unsigned *layer0, unsigned *layer1);
			~Map();
			void render(Camera &camera, float dt) const;
			void update(float dt);
			bool entity_collide(Entity &entity) const;
	};
}

#endif

