#ifndef INCLUDE_CAMERA_H
#define INCLUDE_CAMERA_H

namespace WalrusRPG
{
	class Camera
	{
		protected:
			// Do you want to use classes for coordinates and allow them to have vector-based mathematics? With operator overriding that could be doable to have
			// Vector2 a(3, 2); Vector2 b(1, 2); Vector3 c = a+b;
			// Vector2 destination;
			// Vector2 velocity;
			// Vector2 acceleration;

		public:
			// TODO!: TEMPOARY PUBLIC VARIABLES. Need to make getter/setters.
			// So, how will track camera's position? Top left or center?
			unsigned x; // You'll probably want to switch over signed coordonates.
			unsigned y;
			unsigned render_area_width; // What if you only want to display the map on a part of the screen?
			unsigned render_area_height;

			Camera();
			~Camera();
			// This doesn't need any render as it's the utility which helps rendering. Unless you want to show debnug things.
			// void render(float dt) const;
			void update(float dt);
	};
}

#endif

