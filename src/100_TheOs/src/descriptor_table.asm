[GLOBAL gdt_flush]    ; Allows the C code to call gdt_flush().
gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the GDT pointer.

    mov ax, 0x10      ; 0x10 is the offset to our data segment.
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; Jump to the code segment selector (0x08) and flush the pipeline.
.flush:
    ret

[GLOBAL idt_flush]    ; Allows the C code to call idt_flush().

idt_flush:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
    lidt [eax]        ; Load the IDT pointer.
    ret