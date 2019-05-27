#include "kernel/utils.h"
#include "drivers/screen.h"

/* Forward declarations */
void idt_install();
void keyboard_install();
void timer_install();

void main()
{
  /* Print welcome banner */
  set_text_colour(BLACK, YELLOW);
  print("\nWelcome to RakoNIX!\n");
  reset_text_colour();

  /* Setup IDT and install default interrupt handlers */
  idt_install();

  /* Handle timer interrupts */
  timer_install();

  /* Handle keyboard interrupts */
  keyboard_install();

  /* Enable interrupts using the STI (Set Interrupt Flag) instruction */
  __asm__ __volatile__ ("sti");

  while (1);

  /* Divide by zero test */
  //int e = 10 / 0;

  /* Timer interrupt test */
  int i = 0;

  while (1)
  {
    sleep(1);
    print_hex(i);
    print("\n");
    i++;
  }
}
