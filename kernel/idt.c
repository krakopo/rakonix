#include "kernel/types.h"
#include "kernel/utils.h"

struct idt_entry
{
  uint16_t base_lo;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_hi;
};

struct idt_ptr
{
  uint16_t limit;
  uint32_t base;
};

#define NUM_IDT_ENTRIES 256

// Declare the IDT
struct idt_entry idt[NUM_IDT_ENTRIES];
struct idt_ptr idtp;

// This is an assembly routine defined in idt.asm
extern void idt_load();

void idt_set_gate(uint8_t num,
                  uint32_t base,
                  uint16_t sel,
                  uint8_t flags)
{
  idt[num].base_lo = base;
  idt[num].sel = sel;
  idt[num].flags = flags;
  idt[num].base_hi = base >> 16;
}

void idt_install()
{
  // Setup the IDT pointer
  idtp.limit = (sizeof(struct idt_entry) * NUM_IDT_ENTRIES) - 1;
  idtp.base = (uint32_t) &idt;

  // Clear out the IDT, initializing to zeros
  memset((unsigned char *) &idt, 0, sizeof(struct idt_entry) * NUM_IDT_ENTRIES);

  // Add new ISRs to the IDT here using idt_set_gate

  // Call assembly routine to load the IDT
  idt_load();
}
