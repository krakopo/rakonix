; Execute the LIDT instruction to load the IDT we crafted

[bits 32]

[global idt_load]

[extern idtp]

idt_load:
  lidt[idtp]
  ret
