#include "interrupts.h"

// Wrapper for start_keyboard
void start_irq(void) {
    start_keyboard();
}

// Wrapper for init_idt (if that's the actual function name)
void start_idt(void) {
    init_idt();
}