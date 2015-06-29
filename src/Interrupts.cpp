#include "Interrupts.h"
#include "Graphics.h"

#define INTERRUPTS WalrusRPG::Interrupts

#define INTERRUPT_CONTROLLER 0xDC000000
volatile unsigned *irq_status = (unsigned *) (INTERRUPT_CONTROLLER + 0x0);
volatile unsigned *interrupt_select = (unsigned *) (INTERRUPT_CONTROLLER + 0xC);
unsigned interrupt_select_bkp;
volatile unsigned *interrupt_enable = (unsigned *) (INTERRUPT_CONTROLLER + 0x10);
volatile unsigned *interrupt_enable_clear = (unsigned *) (INTERRUPT_CONTROLLER + 0x14);
unsigned interrupt_enable_bkp;

volatile unsigned *interrupt_pointer = (unsigned *) 0x38;
unsigned interrupt_pointer_bkp;

// Interrupt source 21 is the LCD
#define INTERRUPT_MASK (1 << 21)

void INTERRUPTS::init()
{
    interrupt_select_bkp = *interrupt_select;
    *interrupt_select = 0; // All IRQ for now

    interrupt_enable_bkp = *interrupt_enable;
    *interrupt_enable = INTERRUPT_MASK;
    *interrupt_enable_clear = ~(INTERRUPT_MASK);

    interrupt_pointer_bkp = *interrupt_pointer;
    *interrupt_pointer = (unsigned) &isr;

    // Enable IRQ in the CPU
    asm("mrs r1, cpsr \n\t"
            "bic r1, r1, #0x80 \n\t"
            "msr cpsr, r1"
            : : : "r1");
}

void INTERRUPTS::off()
{
    // Disable IRQ in the CPU
    asm("mrs r1, cpsr \n\t"
            "orr r1, r1, #0x80 \n\t"
            "msr cpsr, r1"
            : : : "r1");

    *interrupt_select = interrupt_select_bkp;

    *interrupt_enable = interrupt_enable_bkp;
    *interrupt_enable_clear = ~(interrupt_enable_bkp);

    *interrupt_pointer = interrupt_pointer_bkp;
}

void __attribute__((interrupt("IRQ"))) INTERRUPTS::isr()
{
    if (*irq_status & (1 << 21))
        WalrusRPG::Graphics::vsync_isr();
}
