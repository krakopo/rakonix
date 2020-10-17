#include "kernel/utils.h"
#include "kernel/types.h"
#include "drivers/screen.h"
#include "drivers/vesa.h"
#include "kernel/acpi.h"

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
  printf("Welcome to RakoNIX!\n\n");
  reset_text_colour();

  /* Setup IDT and install default interrupt handlers */
  idt_install();

  /* Handle timer interrupts */
  timer_install();

  /* Handle keyboard interrupts */
  keyboard_install();

  /* Enable interrupts using the STI (Set Interrupt Flag) instruction */
  __asm__ __volatile__ ("sti");

  /* Print some information about the CPU */
  cpuid();

  /* Setup ACPI */
  init_acpi();

  /* Run kernel shell */
  run_shell();

  /* Print exit banner */
  set_text_colour(BLACK, CYAN);
  printf("Kernel exiting. Bye!\n");
  reset_text_colour();

  /* Timer interrupt test */
  int t = 3;
  printf("Timer interrupt test:");
  while (t)
  {
    printf(" %d", t--);
    sleep(1);
  }
  printf("\n");

  /* Divide by zero test */
  printf("Divison by zero interrupt test:");
  int z = 0;
  z = 10 / z;
}
