#ifndef IDT_H
#define IDT_H

#include "kernel/types.h"

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

void idt_set_handler(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void irq_install_custom_handler(int irq_num, void (*handler)(struct isr_params *p));

#endif /* IDT_H */
