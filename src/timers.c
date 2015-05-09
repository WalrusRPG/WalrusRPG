#include <os.h>
#include <timers.h>

#define TIMER 0x900D0000
unsigned timer_ctl_bkp[2], timer_load_bkp[2];

void timer_init(unsigned timer)
{
    volatile unsigned *timer_ctl = (unsigned *) (TIMER + 0x08 + 0x20 * timer);
    volatile unsigned *timer_load = (unsigned *) (TIMER + 0x20 * timer);

    timer_ctl_bkp[timer] = *timer_ctl;
    timer_load_bkp[timer] = *timer_load;
}

void timer_restore(unsigned timer)
{
    volatile unsigned *timer_ctl = (unsigned *) (TIMER + 0x08 + 0x20 * timer);
    volatile unsigned *timer_load = (unsigned *) (TIMER + 0x20 * timer);

    *timer_ctl &= ~(1 << 7);
    *timer_ctl = timer_ctl_bkp[timer] & ~(1 << 7);
    *timer_load = timer_load_bkp[timer];
    *timer_ctl = timer_ctl_bkp[timer];
}

void timer_mode(unsigned timer, unsigned mode)
{
    volatile unsigned *timer_ctl = (unsigned *) (TIMER + 0x08 + 0x20 * timer);

    *timer_ctl &= ~(1 << 7);
    *timer_ctl = mode;
    *timer_ctl |= (1 << 7);
}

void timer_load(unsigned timer, unsigned value)
{
    volatile unsigned *timer_load = (unsigned *) (TIMER + 0x20 * timer);
    *timer_load = value;
}

unsigned timer_read(unsigned timer)
{
    volatile unsigned *timer_value = (unsigned *) (TIMER + 0x04 + 0x20 * timer);
    return *timer_value;
}
