#ifndef INCLUDE_CAMERA_H
#define INCLUDE_CAMERA_H

#include "Rect.h"

namespace WalrusRPG
{
    class Camera
    {
      protected:
        // So, how will track camera's position? Top left or center?
        signed x; // You'll probably want to switch over signed coordonates.
        signed y;
        unsigned render_area_width; // What if you only want to display the map on a part
                                    // of the screen?
        unsigned render_area_height;

        // Do you want to use classes for coordinates and allow them to have vector-based
        // mathematics? With operator overriding that could be doable to have
        // Vector2 a(3, 2); Vector2 b(1, 2); Vector3 c = a+b;
        // Vector2 destination;
        // Vector2 velocity;
        // Vector2 acceleration;

      public:
        Camera(signed x, signed y);
        ~Camera();
        // This doesn't need any render as it's the utility which helps rendering. Unless
        // you want to show debnug things.
        // void render(float dt) const;
        void update(unsigned dt);

        void set_x(signed x);
        signed get_x() const;
        void set_y(signed y);
        signed get_y() const;

        // Can you see me, senpai ? >.<
        bool is_visible(const WalrusRPG::Utils::Rect &object) const;
    };
}

#endif
