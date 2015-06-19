#include "Animator.h"
#include "Graphics.h"
#include "Rect.h"

#define ANIMATOR WalrusRPG::Animator
#define FRAME WalrusRPG::Frame
#define UTILS WalrusRPG::Utils

namespace
{
    unsigned find_frame(const WalrusRPG::Animation &anim, signed frame_time)
    {
        unsigned index = 0;
        do
        {
            frame_time -= anim.stripe[index].duration;
            index++;
            if (index >= anim.stripe.size() && anim.looping)
                index -= anim.stripe.size();
            else
                return anim.stripe.size() - 1;
        } while (frame_time > 0);
        return index;
    }
}

ANIMATOR::Animator()
{
}

void ANIMATOR::add_animation(int index, Animation anim)
{
    animations[index] = anim;
}

unsigned ANIMATOR::get_animation_frame(unsigned id)
{
    if (animations[id].stripe.empty())
        return id;
    return find_frame(animations[id], elapsed_time);
}

void ANIMATOR::update(unsigned dt)
{
    elapsed_time += dt;
}
