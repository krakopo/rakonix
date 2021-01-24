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

extern void *memory_regions;
extern int *num_memory_regions;

struct memory_region {
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint32_t type;
} __attribute__((packed));

#define MEMORY_REGION_FREE 1

void main()
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

  /* Print memory information */
	int i;
	struct memory_region *m;
	struct memory_region *kheap = NULL;
	for (i = 0; i < *num_memory_regions; i++) {
		m = (struct memory_region *) (memory_regions + sizeof(struct memory_region) * i);

		/* Skip non-free regions */
		if (m->type != MEMORY_REGION_FREE) {
			continue;
		}

		/* Skip base addr of zero */
		if (m->base_addr_high + m->base_addr_low == 0) {
			continue;
		}

		/* TODO Assert m->base_addr_high and m->length_high are zero */
		kheap = m;
	}

	if (kheap != NULL) {
		printf("Heap memory at address 0x%x size %d bytes\n", kheap->base_addr_low, kheap->length_low);
	}

  /* Setup ACPI */
  init_acpi();

  /* Run kernel shell */
  run_shell();

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
