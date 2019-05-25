; Interrupt Service Routines (ISRs)

[bits 32]

; Define our ISRs here.
;
; We declare them as global since we need to be able
; to reference them inside our C program where we
; install the ISR into the IDT entry.

; Interrupt 0: Divide By Zero Exception
global isr0
isr0:
  cli
  push byte 0    ; A normal ISR stub that pops a dummy error code to keep a
                 ; uniform stack frame
  push byte 0
  jmp isr_common_stub

; Interrupt 1: Debug Exception
global isr1
isr1:
  cli
  push byte 0
  push byte 1
  jmp isr_common_stub

global isr2
isr2:

global isr3
isr3:

global isr4
isr4:

global isr5
isr5:

global isr6
isr6:

global isr7
isr7:

; Interrupt 8: Double Fault Exception (with error code!)
global isr8
isr8:
  cli
  push byte 8    ; Note that we DON'T push a value on the stack in this one!
                 ; It pushes one already! Use this type of stub for exceptions
                 ; that pop error codes!
  jmp isr_common_stub

global isr9
isr9:

global isr10
isr10:

global isr11
isr11:

global isr12
isr12:

global isr13
isr13:

global isr14
isr14:

global isr15
isr15:

global isr16
isr16:

global isr17
isr17:

global isr18
isr18:

global isr19
isr19:

global isr20
isr20:

global isr21
isr21:

global isr22
isr22:

global isr23
isr23:

global isr24
isr24:

global isr25
isr25:

global isr26
isr26:

global isr27
isr27:

global isr28
isr28:

global isr29
isr29:

global isr30
isr30:

global isr31
isr31:

; The first 32 entries in the IDT are for interrupts raised by the processor.
; The next 16 entries are for interrupts raised by hardware devices like a
; keyboard or network card.

global irq0
irq0:
  cli
  push byte 0    ; IRQs don't push an error code so use a dummy error
                 ; code to keep things consistent with ISRs
  push byte 32
  jmp isr_common_stub

global irq1
irq1:

global irq2
irq2:

global irq3
irq3:

global irq4
irq4:

global irq5
irq5:

global irq6
irq6:

global irq7
irq7:

global irq8
irq8:

global irq9
irq9:

global irq10
irq10:

global irq11
irq11:

global irq12
irq12:

global irq13
irq13:

global irq14
irq14:

global irq15
irq15:

; We call a C function in here. We need to let the assembler know
; that 'idt_default_handler' exists in another file
extern idt_default_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10       ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp       ; Push us the stack
    push eax

    ; Not sure why we can't just use a regular call here since the
    ; eip register is saved when the interrupt is triggered
    ;
    ;mov eax, idt_default_handler
    ;call eax           ; A special call, preserves the 'eip' register
    ;
    ; Use a regular call
    call idt_default_handler

    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8         ; Cleans up the pushed error code and pushed ISR number
    iret               ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
