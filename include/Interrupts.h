#ifndef INCLUDE_INTERRUPTS_H
#define INCLUDE_INTERRUPTS_H

namespace WalrusRPG
{
    namespace Interrupts
    {
        void init();
        void off();
        void __attribute__((interrupt("IRQ"))) isr();
    }
}
#endif
