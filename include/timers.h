#ifndef INCLUDE_TIMERS_H
#define INCLUDE_TIMERS_H

#ifdef __cplusplus
extern
    "C" {
#endif

// TODO : Move these functions to C++ namespace
// TODO : add defines/const values to make the modes more clearer to read

void timer_init(unsigned timer);
void timer_restore(unsigned timer);
void timer_mode(unsigned timer, unsigned mode);
void timer_load(unsigned timer, unsigned value);
unsigned timer_read(unsigned timer);

#ifdef __cplusplus
}
#endif
#endif
