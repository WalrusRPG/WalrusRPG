#include "Timing.h"
#include "Logger.h"
#include "platform.h"
#include <3ds/svc.h>
#include <cstdio>

constexpr u64 TIMER_PRECISION = 268123480;
constexpr u64 TIMER_SCALEDOWN = TIMER_PRECISION / TIMER_FREQ;

using namespace WalrusRPG; /*Timing*/
u64 startingTime;

void Timing::init()
{
    Logger::log("Timing init");
    startingTime = svcGetSystemTick();
}

void Timing::deinit()
{
    Logger::log("Timing deinit");
}

unsigned Timing::gettime()
{
    u64 t = svcGetSystemTick() - startingTime;
    return t / TIMER_SCALEDOWN;
}
