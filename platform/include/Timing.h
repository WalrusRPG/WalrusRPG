#ifndef INCLUDE_TIMING_H
#define INCLUDE_TIMING_H

/*
 * Timing.h
 * Timing backend abstraction
 */

namespace WalrusRPG
{
    namespace Timing
    {
        void init();
        void deinit();

        unsigned gettime();
    }
}

#endif
