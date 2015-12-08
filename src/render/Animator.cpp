#include "Animator.h"
#include <stdio.h>

#define ANIMATOR WalrusRPG::Animator

using namespace WalrusRPG;

namespace
{
    unsigned get_animation_duration(const WalrusRPG::Animation &anim)
    {
        unsigned duration = 0;
        for (unsigned index = 0; index < anim.stripe.size(); index++)
        {
            duration += anim.stripe[index].duration;
        }
        return duration;
    }

    unsigned find_frame(const WalrusRPG::Animation &anim, int frame_time)
    {
        if (frame_time >= anim.duration && !anim.looping)
        {
            // simple looping checking
            // Also, a flag to detect if the animation ended could be an option
            return anim.stripe[anim.stripe.size() - 1].frame;
        }
        frame_time %= anim.duration;

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

unsigned ANIMATOR::add_animation(Animation anim)
{
    animations.push_back(anim);
    unsigned index = animations.size()-1;
    animations[index] = anim;
    animations[index].duration = get_animation_duration(anim);
    return animations.size();
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
