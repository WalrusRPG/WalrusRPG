#include "Timing.h"
#include "Logger.h"
#include "platform.h"
#include <cstdio>
#include <psp2/rtc.h>

const uint64_t TIMER_PRECISION = sceRtcGetTickResolution();
const uint64_t TIMER_SCALEDOWN = TIMER_PRECISION / TIMER_FREQ;

using namespace WalrusRPG; /*Timing*/
uint64_t startingTime;

void Timing::init()
{
    Logger::log("Timing init");
    sceRtcGetCurrentTick(&startingTime);
}

void Timing::deinit()
{
    Logger::log("Timing deinit");
}

unsigned Timing::gettime()
{
    uint64_t ticks;
    sceRtcGetCurrentTick(&ticks);
    uint64_t t = ticks - startingTime;
    return t / TIMER_SCALEDOWN;
}
