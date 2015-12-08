#ifndef INCLUDE_ANIMATOR_H
#define INCLUDE_ANIMATOR_H

#include <TINYSTL/vector.h>
#include <TINYSTL/unordered_map.h>

namespace WalrusRPG
{
    struct Frame
    {
        unsigned frame;
        unsigned duration; // 1f = 1s
    };
    struct Animation
    {
        tinystl::vector<WalrusRPG::Frame> stripe;
        bool looping;
        int duration;
    };

    class Animator
    {
      public:
        tinystl::vector<Animation> animations;


      protected:
        unsigned elapsed_time;

      public:
        Animator();
        unsigned add_animation(Animation anim);
        void update(unsigned dt);
        unsigned get_animation_frame(unsigned id);
    };
}

#endif
