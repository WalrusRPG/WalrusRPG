#ifndef INCLUDE_ANIMATOR_H
#define INCLUDE_ANIMATOR_H

#include <TINYSTL/vector.h>
#include <TINYSTL/unordered_map.h>
#include "Rect.h"

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
    };

    class Animator
    {
      public:
        tinystl::unordered_map<unsigned, Animation> animations;


      protected:
        unsigned elapsed_time;

      public:
        Animator();
        void add_animation(int index, Animation anim);
        void update(unsigned dt);
        unsigned get_animation_frame(unsigned id);
    };
}

#endif
