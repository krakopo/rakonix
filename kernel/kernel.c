#include "kernel/utils.h"
#include "kernel/types.h"
#include "drivers/screen.h"
#include "drivers/vesa.h"

/* Forward declarations */
void idt_install();
void keyboard_install();
void timer_install();
void run_shell();

void main()
{
  /* Clear the screen */
  clear_screen();

  /* Print welcome banner */
  set_text_colour(BLACK, YELLOW);
  printf("Welcome to RakoNIX!\n");
  reset_text_colour();

  /* Setup IDT and install default interrupt handlers */
  idt_install();

  /* Handle timer interrupts */
  timer_install();

  /* Handle keyboard interrupts */
  keyboard_install();

  /* Enable interrupts using the STI (Set Interrupt Flag) instruction */
  __asm__ __volatile__ ("sti");

  run_shell();

#if 0
  /* Print some information about the CPU */
  cpuid();

  /* Divide by zero test */
  int e = 10 / 0;

  /* Timer interrupt test */
  int t = 0;

  while (1)
  {
    sleep(1);
    printf("0x%x\n", t);
    t++;
  }
#endif

  /* Print exit banner */
  set_text_colour(BLACK, RED);
  printf("Kernel exiting. Bye!\n");
  reset_text_colour();
}
