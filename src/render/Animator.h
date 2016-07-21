#ifndef INCLUDE_ANIMATOR_H
#define INCLUDE_ANIMATOR_H

#include <TINYSTL/unordered_map.h>
#include <TINYSTL/vector.h>
#include <utility>
namespace WalrusRPG
{
    struct Frame
    {
        unsigned frame;
        unsigned duration; // 1f = 1s
    };
    struct Animation
    {
        /*
            This thing? It's just to remove the last arugment from the structured
           initilizer constructor.
            Yeah. it's shitty af to have such constructors defined but it looks like C++
           *does* want this.
         */
        Animation(tinystl::vector<WalrusRPG::Frame> stripe, bool looping)
            : stripe(stripe), looping(looping)
        {
        }
        Animation() : stripe(), looping()
        {
        }
        Animation(const Animation &a) noexcept : stripe(a.stripe), looping(a.looping)
        {
        }
        Animation(Animation &&a) noexcept : stripe(std::move(a.stripe)),
                                            looping(std::move(a.looping))
        {
        }
        Animation &operator=(Animation arg)
        {
            std::swap(stripe, arg.stripe);
            std::swap(looping, arg.looping);
            return *this;
        }
        tinystl::vector<WalrusRPG::Frame> stripe;
        bool looping;
        int duration;
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
        void update();
        unsigned get_animation_frame(unsigned id) const;
    };
}

#endif
