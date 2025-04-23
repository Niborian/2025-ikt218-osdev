#include "irq.h"
#include "common.h"
#include "libc/stddef.h"

#define IRQ_COUNT 16

struct int_handler_t {
    isr_t handler;
    void* data;
    uint8_t num;
};

static struct int_handler_t irq_handlers[IRQ_COUNT];

void init_irq() {
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].handler = 0;
        irq_handlers[i].data = 0;
        irq_handlers[i].num = i;
    }
}

void register_irq_handler(uint8_t irq, isr_t handler, void* context) {
    irq_handlers[irq].handler = handler;
    irq_handlers[irq].data = context;
}

void irq_controller(registers_t* regs) {
    if (regs->int_no >= 40) outb(0xA0, 0x20); // slave
    outb(0x20, 0x20);                         // master

    // Dispatch to registered handler
    uint8_t irq = regs->int_no - 32;
    if (irq < IRQ_COUNT && irq_handlers[irq].handler) {
        irq_handlers[irq].handler(regs, irq_handlers[irq].data);
    }
}

void start_irq() {
    // Initialize handler array
    init_irq();
    
    // Initialize the PIC
    // ICW1: Initialize both PICs
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // ICW2: Set interrupt offsets
    outb(0x21, 0x20);  // Master PIC: IRQ0-7 -> INT 0x20-0x27
    outb(0xA1, 0x28);  // Slave PIC: IRQ8-15 -> INT 0x28-0x2F
    
    // ICW3: Configure master/slave relationship
    outb(0x21, 0x04);  // Tell master PIC slave is at IRQ2
    outb(0xA1, 0x02);  // Tell slave PIC its identity (2)
    
    // ICW4: Set mode
    outb(0x21, 0x01);  // 8086/88 mode
    outb(0xA1, 0x01);  // 8086/88 mode
    
    // OCW1: Mask interrupts (except keyboard)
    outb(0x21, 0xFD);  // Enable only IRQ1 (keyboard)
    outb(0xA1, 0xFF);  // Mask all slave interrupts
}

