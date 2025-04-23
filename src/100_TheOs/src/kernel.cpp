extern "C" {
    #include "libc/system.h"
    #include "memory/memory.h"
    #include "common.h"
    #include "interrupts.h"
    #include "song/song.h"
    #include "monitor.h"

    void panic(const char* reason);
    void start_idt(void);
    void start_irq(void);
    void start_isr_controllers(void);
    void start_keyboard(void);
    void display_prompt(void);
    char scancode_to_ascii(uint8_t scancode);
}

// Memory operators
void* operator new(size_t size) { return malloc(size); }
void* operator new[](size_t size) { return malloc(size); }
void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete[](void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, size_t) noexcept { free(ptr); }
void operator delete[](void* ptr, size_t) noexcept { free(ptr); }

extern "C" int kernel_main(void) {
    printf("Booting C++ kernel...\n");

    start_idt();
    printf("IDT initialized\n");

    start_irq(); 
    printf("IRQ initialized\n");

    start_isr_controllers();
    printf("ISRs initialized\n");

    start_keyboard();
   
    asm volatile("sti");
    printf("Interrupts enabled\n");

    // Prompt
    printf("Ready. Type something below:\n");
    display_prompt();

    // Main loop
    while (true) {
        asm volatile("hlt");
    }

    return 0;
}
