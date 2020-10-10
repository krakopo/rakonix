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

  /* Print some information about the CPU */
  cpuid();

  /* Timer interrupt test */
  int t = 3;

  printf("Timer interrupt test:");
  while (t)
  {
    printf(" %d", t--);
    sleep(1);
  }
  printf("\n");

  // Search for RSDP
  int i = 0;
  for (i = 0x000E0000; i < 0x000FFFFF; i += 16)
  {
    const unsigned char *mem = (const unsigned char *) i;
    if (memcmp(mem, (const unsigned char *) "RSD PTR ", 8) == 0) {
      printf("Found RSDP at address 0x%x\n", i);
      break;
    }
  }

  run_shell();

  /* Print exit banner */
  set_text_colour(BLACK, CYAN);
  printf("Kernel exiting. Bye!\n");
  reset_text_colour();

  /* Divide by zero test */
  int z = 0;
  z = 10 / z;
}
