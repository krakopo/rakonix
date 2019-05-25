#include "kernel/types.h"
#include "kernel/utils.h"
#include "drivers/screen.h"

/* Support up to the full 256 IDT entries */
#define NUM_IDT_ENTRIES 256

/* 
 * We need to use "__attribute__((packed))" in the IDT structs below to
 * ensure the compiler doesn't add any padding between elements in each struct.
 */

/* There will be one such struct for each entry in the IDT */
struct idt_entry
{
  /* TODO Describe */

  uint16_t base_lo;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_hi;
} __attribute__((packed));

/* This is what and IDT pointer looks like */
struct idt_ptr
{
  /* TODO Describe */
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

/* The IDT - an array of IDT entries */
struct idt_entry idt[NUM_IDT_ENTRIES];

/* 
 * The IDT pointer which we will pass to the LIDT instruction to
 * load the IDT into the processor.
 */
struct idt_ptr idtp;

/* The assembly routine to call LIDT */
extern void idt_load();

/*
 * We use this function to populate entries in our IDT.
 *
 *  @num   The IDT entry number we want to modify
 *  @base  The address of the function we want to call to handle the interrupt
 *  @sel   Selector bits for this IDT entry
 *  @flags Flag bits for this IDT entry
 */
void idt_set_handler(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
  idt[num].base_lo = base & 0xffff;
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;
  idt[num].base_hi = (base >> 16) & 0xffff;
}

/* This defines what the stack looks like after an ISR was running. */
struct isr_params
{
  /* We pushed the segment addrs last so they are first */
  uint32_t gs, fs, es, ds;

  /* Pushed by PUSHA instruction */
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

  /* Pushed by our 'PUSH BYTE <int #>' and 'PUSH BYTE <err code>' */
  uint32_t int_no, err_code;

  /* Pushed by the processor due to the interrupt */
  uint32_t eip, cs, eflags, useresp, ss;

/*
 * Note no need to use "__attribute__((packed))" since everything
 * here is 32-bits (ie, no padding needed). But we use it anyway
 * to signify that it does need to be packed.
 */
} __attribute__((packed));

/*
 * Array of exception messages corresponding to interrupt number.
 * For example, interrupt 0 is a divide by zero exception.
 */
unsigned char *exception_messages[] =
{
  /*
   * TODO Investigate
   * For some reason this array blows up the size of kernel.bin,
   * even with just a single item. Not sure what causes `ld` to
   * produce such a larger binary.
   */

  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Integer Overflow Detected",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",
  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

/* The default C-language entry point for ISRs */
void idt_default_handler(struct isr_params *isrp)
{
  if (isrp->int_no < 32)
  {
    set_text_colour(BLACK, LIGHT_RED);
    print("\nUnhandled Interrupt @ EIP = ");
    print_hex(isrp->eip);
    print(": ");
    print(exception_messages[isrp->int_no]);
    print("\n");
    for(;;);
  }
}

/* All our assembly ISR entry points */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* Finally, the function to install the IDT */
void idt_install()
{
  /* Setup the IDT pointer */
  idtp.limit = (sizeof(struct idt_entry) * NUM_IDT_ENTRIES) - 1;
  idtp.base = (uint32_t) idt;

  /* Clear out the IDT, initializing to zeros */
  memset((unsigned char *) idt, 0, sizeof(struct idt_entry) * NUM_IDT_ENTRIES);

  /* Set ISRs for the IDT entries */
  idt_set_handler( 0, (uint32_t)  isr0, 0x8, 0x8e);
  idt_set_handler( 1, (uint32_t)  isr1, 0x8, 0x8e);
  idt_set_handler( 2, (uint32_t)  isr2, 0x8, 0x8e);
  idt_set_handler( 3, (uint32_t)  isr3, 0x8, 0x8e);
  idt_set_handler( 4, (uint32_t)  isr4, 0x8, 0x8e);
  idt_set_handler( 5, (uint32_t)  isr5, 0x8, 0x8e);
  idt_set_handler( 6, (uint32_t)  isr6, 0x8, 0x8e);
  idt_set_handler( 7, (uint32_t)  isr7, 0x8, 0x8e);
  idt_set_handler( 8, (uint32_t)  isr8, 0x8, 0x8e);
  idt_set_handler( 9, (uint32_t)  isr9, 0x8, 0x8e);
  idt_set_handler(10, (uint32_t) isr10, 0x8, 0x8e);
  idt_set_handler(11, (uint32_t) isr11, 0x8, 0x8e);
  idt_set_handler(12, (uint32_t) isr12, 0x8, 0x8e);
  idt_set_handler(13, (uint32_t) isr13, 0x8, 0x8e);
  idt_set_handler(14, (uint32_t) isr14, 0x8, 0x8e);
  idt_set_handler(15, (uint32_t) isr15, 0x8, 0x8e);
  idt_set_handler(16, (uint32_t) isr16, 0x8, 0x8e);
  idt_set_handler(17, (uint32_t) isr17, 0x8, 0x8e);
  idt_set_handler(18, (uint32_t) isr18, 0x8, 0x8e);
  idt_set_handler(19, (uint32_t) isr19, 0x8, 0x8e);
  idt_set_handler(20, (uint32_t) isr20, 0x8, 0x8e);
  idt_set_handler(21, (uint32_t) isr21, 0x8, 0x8e);
  idt_set_handler(22, (uint32_t) isr22, 0x8, 0x8e);
  idt_set_handler(23, (uint32_t) isr23, 0x8, 0x8e);
  idt_set_handler(24, (uint32_t) isr24, 0x8, 0x8e);
  idt_set_handler(25, (uint32_t) isr25, 0x8, 0x8e);
  idt_set_handler(26, (uint32_t) isr26, 0x8, 0x8e);
  idt_set_handler(27, (uint32_t) isr27, 0x8, 0x8e);
  idt_set_handler(28, (uint32_t) isr28, 0x8, 0x8e);
  idt_set_handler(29, (uint32_t) isr29, 0x8, 0x8e);
  idt_set_handler(30, (uint32_t) isr30, 0x8, 0x8e);
  idt_set_handler(31, (uint32_t) isr31, 0x8, 0x8e);

  /* Call the assembly routine to load the IDT we have prepared */
  idt_load();
}
