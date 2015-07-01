#ifndef INCLUDE_TIMERS_H
#define INCLUDE_TIMERS_H

namespace WalrusRPG
{
    namespace Timers
    {
        void init(unsigned timer);
        void restore(unsigned timer);
        void mode(unsigned timer, bool free_run, bool oneshot, bool interrupt,
                  unsigned div, bool full_width_counter);
        void load(unsigned timer, unsigned value);
        unsigned read(unsigned timer);
    }
}

#endif
