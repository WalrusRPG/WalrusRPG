#ifndef INCLUDE_TIMERS_H
#define INCLUDE_TIMERS_H

#include <cstdint>

namespace Nspire
{
    namespace Timers
    {
        void init(uint32_t timer);
        void restore(uint32_t timer);
        void mode(uint32_t timer, bool free_run, bool oneshot, bool interrupt,
                  uint32_t div, bool full_width_counter);
        void load(uint32_t timer, uint32_t value);
        uint32_t read(uint32_t timer);
    }
}

#endif
