#include "kernel/utils.hpp"
#include "kernel/types.hpp"
#include "drivers/screen.hpp"
#include "drivers/vesa.hpp"
#include "kernel/acpi.hpp"
#include "kernel/kernel.hpp"

/* Forward declarations */
void idt_install();
void keyboard_install();
void timer_install();
void run_shell();

void main()
{
  Kernel k;
  theKernel = &k;
  k.openShell();
}

Kernel::Kernel()
{
  /* Clear the screen */
  clear_screen();

  /* Print welcome banner */
  printf_colour(BLACK, YELLOW, "Welcome to RakoNIX!\n\n");

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
}

Kernel::~Kernel()
{
  /* Print exit banner */
  printf_colour(BLACK, CYAN, "\nKernel exiting. Bye!\n");

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

void Kernel::openShell()
{
  run_shell();
}
