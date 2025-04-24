// kernel.cpp
extern "C" {
    #include "libc/system.h"
    #include "memory/memory.h"
    #include "common.h"
    #include "interrupts.h"
    #include "song/song.h"
    #include "monitor.h"
    #include "pit.h"
    
    void panic(const char* reason);
    void init_gdt(void);
    void start_idt(void);
    void init_irq(void);
    void start_isr_controllers(void);
    void start_keyboard(void);
    void display_prompt(void);
    char scancode_to_ascii(uint8_t scancode);
    int printf(const char* fmt, ...);
    // Add these declarations if they're defined in your C code
    void keyboard_handler(void);
    void load_interrupt_controller(uint8_t n, isr_t controller, void*);
}

// Memory operators with proper size_t definition
void* operator new(unsigned long size) { return malloc(size); }
void* operator new[](unsigned long size) { return malloc(size); }
void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete[](void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, unsigned long) noexcept { free(ptr); }
void operator delete[](void* ptr, unsigned long) noexcept { free(ptr); }

// Add a test function to verify keyboard IRQ is working
extern "C" void test_key_press(registers* regs, void* context) {
    printf("Key pressed!\n");
}

extern "C" int kernel_main(void) {
    printf("Booting C++ kernel...\n");
    
    // Initialize the essential parts
    start_idt();
    printf("IDT initialized\n");
    
    start_irq();
    printf("IRQ initialized\n");
    
    start_isr_controllers();
    printf("ISRs initialized\n");
    
    // Debug keyboard initialization
    printf("Starting keyboard initialization...\n");
    
    // If you have access to register_interrupt_handler, try registering our test handler
    // Keyboard IRQ is typically IRQ 1 (which maps to interrupt 33 in protected mode)
    printf("Registering test keyboard handler...\n");
    load_interrupt_controller(33, test_key_press, nullptr);
    start_keyboard();
    printf("Keyboard initialized\n");
    
    // Make sure interrupts are enabled
    asm volatile("sti");
    printf("Interrupts enabled\n");
    
    // Print some debug info
    printf("System ready. Try typing. If no input appears, check the following:\n");
    printf("1. IRQ routing correctly set up\n");
    printf("2. Keyboard handler properly registered\n");
    printf("3. Terminal input buffer properly initialized\n");
    
    // Prompt
    printf("Ready. Type something below:\n");
    display_prompt();
    
    // Main loop with some debugging
    uint32_t counter = 0;
    while (true) {
        // Print a heartbeat message every ~5 seconds to verify system is alive
        counter++;
        if (counter % 5000000 == 0) {
            printf("System heartbeat - still running...\n");
        }
        
        asm volatile("hlt");
    }
    
    return 0;
}