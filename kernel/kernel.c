#include "drivers/screen.h"

/* Forward declaration for function to setup IDT */
void idt_install();

void main()
{
  /* Print welcome banner */
  set_text_colour(BLACK, YELLOW);
  print("\nWelcome to RakoNIX!\n");
  reset_text_colour();

  /* Start handling interrupts */
  idt_install();

  /* Enable interrupts using the STI (Set Interrupt Flag) instruction */
  __asm__ __volatile__ ("sti");
}
