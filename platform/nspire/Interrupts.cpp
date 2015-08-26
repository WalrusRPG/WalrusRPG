#include "Interrupts.h"
#include "Graphics.h"

#define INTERRUPTS WalrusRPG::Interrupts

#define INTERRUPT_CONTROLLER 0xDC000000
volatile uint32_t *irq_status = (uint32_t *) (INTERRUPT_CONTROLLER + 0x0);
volatile uint32_t *interrupt_select = (uint32_t *) (INTERRUPT_CONTROLLER + 0xC);
uint32_t interrupt_select_bkp;
volatile uint32_t *interrupt_enable = (uint32_t *) (INTERRUPT_CONTROLLER + 0x10);
volatile uint32_t *interrupt_enable_clear = (uint32_t *) (INTERRUPT_CONTROLLER + 0x14);
uint32_t interrupt_enable_bkp;

volatile uint32_t *interrupt_pointer = (uint32_t *) 0x38;
uint32_t interrupt_pointer_bkp;

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
    *interrupt_pointer = (uint32_t) &isr;

    // Enable IRQ in the CPU
    asm("mrs r1, cpsr \n\t"
        "bic r1, r1, #0x80 \n\t"
        "msr cpsr, r1"
        :
        :
        : "r1");
}

void INTERRUPTS::off()
{
    // Disable IRQ in the CPU
    asm("mrs r1, cpsr \n\t"
        "orr r1, r1, #0x80 \n\t"
        "msr cpsr, r1"
        :
        :
        : "r1");

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
