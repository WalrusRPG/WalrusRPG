#include <os.h>
#include "Timers.h"

#define TIMER 0x900D0000
volatile unsigned *timer_ctl = (unsigned *) (TIMER + 0x08);
volatile unsigned *timer_load = (unsigned *) (TIMER);
volatile unsigned *timer_value = (unsigned *) (TIMER + 0x04);
unsigned timer_ctl_bkp[2], timer_load_bkp[2];

#define TIMERS WalrusRPG::Timers

void TIMERS::init(unsigned timer)
{
    timer_ctl_bkp[timer] = timer_ctl[8 * timer];
    timer_load_bkp[timer] = timer_load[8 * timer];
}

void TIMERS::restore(unsigned timer)
{
    timer_ctl[8 * timer] &= ~(1 << 7);
    timer_ctl[8 * timer] = timer_ctl_bkp[timer] & ~(1 << 7);
    timer_load[8 * timer] = timer_load_bkp[timer];
    timer_ctl[8 * timer] = timer_ctl_bkp[timer];
}

void TIMERS::mode(unsigned timer, bool free_run, bool oneshot, bool interrupt,
                  unsigned div, bool full_width_counter)
{
    unsigned mode = 0;

    if (!free_run)
        mode |= (1 << 6);

    if (oneshot)
        mode |= (1 << 0);

    if (interrupt)
        mode |= (1 << 5);

    if (full_width_counter)
        mode |= (1 << 1);

    switch (div)
    {
        case 256:
            mode |= (1 << 3);
            break;

        case 16:
            mode |= (1 << 2);
            break;
    }

    timer_ctl[8 * timer] &= ~(1 << 7);
    timer_ctl[8 * timer] = mode;
    timer_ctl[8 * timer] |= (1 << 7);
}

void TIMERS::load(unsigned timer, unsigned value)
{
    timer_load[8 * timer] = value;
}

unsigned TIMERS::read(unsigned timer)
{
    return timer_value[8 * timer];
}
