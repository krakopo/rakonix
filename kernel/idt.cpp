#include "kernel/idt.hpp"
#include "kernel/low_level.hpp"
#include "kernel/utils.hpp"
#include "drivers/screen.hpp"

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
extern "C" void idt_load();

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

/*
 * Array of exception messages corresponding to interrupt number.
 * For example, interrupt 0 is a divide by zero exception.
 */
const char *exception_messages[] =
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
extern "C" void idt_default_handler(struct isr_params *isrp)
{
  if (isrp->int_no < 32)
  {
    printf_colour(
      BLACK, RED, "\nUnhandled exception @ EIP = 0x%x: %s\n",
      isrp->eip, exception_messages[isrp->int_no]);

    wait_interrupt(); /* Emits HLT instruction */
    /* We're already in an interrupt handler so HLT wont return */
  }
  else
  {
    printf("Unhandled interrupt number: 0x%x\n", isrp->int_no);
  }
}

/*
 * This is an array of function pointers.
 * We use this to handle custom IRQ handlers for a given IRQ.
 */
typedef void (* irq_handler_t)(struct isr_params *);
irq_handler_t irq_custom_handlers[16] = { 0 };

/* Install customer IRQ handler */
void irq_install_custom_handler(int irq_num, irq_handler_t handler)
{
  irq_custom_handlers[irq_num] = handler;
}

/*
 * Each of the IRQ ISRs point to this function, rather than
 * the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
 * to be told when you are done servicing them, so you need
 * to send them an "End of Interrupt" command (0x20). There
 * are two 8259 chips: The first exists at 0x20, the second
 * exists at 0xA0. If the second controller (an IRQ from 8 to
 * 15) gets an interrupt, you need to acknowledge the
 * interrupt at BOTH controllers, otherwise, you only send
 * an EOI command to the first controller. If you don't send
 * an EOI, you won't raise any more IRQs.
 */
extern "C" void irq_handler(struct isr_params *isrp)
{
  /* If we have a custom handler for this IRQ, run it */
  irq_handler_t handler = irq_custom_handlers[isrp->int_no - 32];
  if (handler)
  {
    handler(isrp);
  }

  /*
   * If the IDT entry that was invoked was greater than 40
   * (meaning IRQ8 - 15), then we need to send an EOI to
   * the slave controller.
   */
  if (isrp->int_no >= 40)
  {
    port_byte_out(0xA0, 0x20);
  }

  /* We always need to send an EOI to the master interrupt controller. */
  port_byte_out(0x20, 0x20);
}

/* All our assembly ISR entry points */
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

/* All our assembly IRQ entry points */
extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

/*
 * Normally, IRQs 0 to 7 are mapped to entries 8 to 15.
 * This is a problem in protected mode, because IDT entry 8 is a
 * Double Fault! Without remapping, every time IRQ0 fires, you get
 * a Double Fault Exception, which is not actually what's happening.
 * So here we send commands to the Programmable Interrupt Controller
 * (PICs - also called the 8259's) in order to make IRQ0 to 15 be
 * remapped to IDT entries 32 to 47.
 */
void irq_remap()
{
  port_byte_out(0x20, 0x11);
  port_byte_out(0xA0, 0x11);
  port_byte_out(0x21, 0x20);
  port_byte_out(0xA1, 0x28);
  port_byte_out(0x21, 0x04);
  port_byte_out(0xA1, 0x02);
  port_byte_out(0x21, 0x01);
  port_byte_out(0xA1, 0x01);
  port_byte_out(0x21, 0x0);
  port_byte_out(0xA1, 0x0);
}

/* Finally, the function to install the IDT */
void idt_install()
{
  /* Setup the IDT pointer */
  idtp.limit = (sizeof(struct idt_entry) * NUM_IDT_ENTRIES) - 1;
  idtp.base = (uint32_t) idt;

  /* Clear out the IDT, initializing to zeros */
  memset(idt, 0, sizeof(struct idt_entry) * NUM_IDT_ENTRIES);

  /* Remap interrupt numbers for our PICs */
  irq_remap();

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
  idt_set_handler(32, (uint32_t)  irq0, 0x8, 0x8e);
  idt_set_handler(33, (uint32_t)  irq1, 0x8, 0x8e);
  idt_set_handler(34, (uint32_t)  irq2, 0x8, 0x8e);
  idt_set_handler(35, (uint32_t)  irq3, 0x8, 0x8e);
  idt_set_handler(36, (uint32_t)  irq4, 0x8, 0x8e);
  idt_set_handler(37, (uint32_t)  irq5, 0x8, 0x8e);
  idt_set_handler(38, (uint32_t)  irq6, 0x8, 0x8e);
  idt_set_handler(39, (uint32_t)  irq7, 0x8, 0x8e);
  idt_set_handler(40, (uint32_t)  irq8, 0x8, 0x8e);
  idt_set_handler(41, (uint32_t)  irq9, 0x8, 0x8e);
  idt_set_handler(42, (uint32_t) irq10, 0x8, 0x8e);
  idt_set_handler(43, (uint32_t) irq11, 0x8, 0x8e);
  idt_set_handler(44, (uint32_t) irq12, 0x8, 0x8e);
  idt_set_handler(45, (uint32_t) irq13, 0x8, 0x8e);
  idt_set_handler(46, (uint32_t) irq14, 0x8, 0x8e);
  idt_set_handler(47, (uint32_t) irq15, 0x8, 0x8e);

  /* Call the assembly routine to load the IDT we have prepared */
  idt_load();
}
