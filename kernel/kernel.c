#include "drivers/screen.h"

/* Forward declaration for function to setup IDT */
void idt_install();

void main()
{
  /* Print welcome banner */
  print_with_attr("\nWelcome to RakoNIX!\n", YELLOW_ON_BLACK);

  /* Start handling interrupts */
  idt_install();
}
