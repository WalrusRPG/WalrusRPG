#include "Animator.h"

#define ANIMATOR WalrusRPG::Animator

using namespace WalrusRPG;

namespace
{
    unsigned find_frame(const WalrusRPG::Animation &anim, signed frame_time)
    {
        unsigned index = 0;
        do
        {
            frame_time -= anim.stripe[index].duration;
            index++;
            if (index >= anim.stripe.size())
            {
                if (anim.looping)
                    index = 0;
                else
                    return anim.stripe[anim.stripe.size() - 1].frame;
            }
        } while (frame_time > 0);
        return anim.stripe[index].frame;
    }
}

ANIMATOR::Animator()
{
    elapsed_time = 0;
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
