#include <os.h>
#include "Timers.h"

#define TIMER 0x900D0000
volatile uint32_t *timer_ctl = (uint32_t *) (TIMER + 0x08);
volatile uint32_t *timer_load = (uint32_t *) (TIMER);
volatile uint32_t *timer_value = (uint32_t *) (TIMER + 0x04);
uint32_t timer_ctl_bkp[2], timer_load_bkp[2];

#define TIMERS WalrusRPG::Timers

void TIMERS::init(uint32_t timer)
{
    timer_ctl_bkp[timer] = timer_ctl[8 * timer];
    timer_load_bkp[timer] = timer_load[8 * timer];
}

void TIMERS::restore(uint32_t timer)
{
    timer_ctl[8 * timer] &= ~(1 << 7);
    timer_ctl[8 * timer] = timer_ctl_bkp[timer] & ~(1 << 7);
    timer_load[8 * timer] = timer_load_bkp[timer];
    timer_ctl[8 * timer] = timer_ctl_bkp[timer];
}

void TIMERS::mode(uint32_t timer, bool free_run, bool oneshot, bool interrupt,
                  uint32_t div, bool full_width_counter)
{
    uint32_t mode = 0;

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

void TIMERS::load(uint32_t timer, uint32_t value)
{
    timer_load[8 * timer] = value;
}

uint32_t TIMERS::read(uint32_t timer)
{
    return timer_value[8 * timer];
}
