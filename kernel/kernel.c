#include "kernel/utils.h"
#include "kernel/types.h"
#include "drivers/screen.h"
#include "drivers/vesa.h"

/* Forward declarations */
void idt_install();
void keyboard_install();
void timer_install();

void main()
{
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

  cpuid();

  printf("pitch width height: %d %d %d\n", vbe_mode_info->pitch, vbe_mode_info->width, vbe_mode_info->height);
  printf("framebuffer: 0x%x\n", vbe_mode_info->framebuffer);
  while (1);

  int offset = 0;
  for (offset = 0; offset < 32; offset += 8)
  {
    int i = 0;
    int j = 0;

    for (i = 0; i < 8; i++)
    {
      for (j = 0; j < 8; j++)
      {
        switch (offset)
        {
          case 0:
          putpixel(vbe_mode_info->framebuffer, offset + i, offset+ j, 0xffffff);
          break;
          case 8:
          putpixel(vbe_mode_info->framebuffer, offset + i, offset+ j, 0xff0000);
          break;
          case 16:
          putpixel(vbe_mode_info->framebuffer, offset + i, offset+ j, 0x00ff00);
          break;
          case 24:
          putpixel(vbe_mode_info->framebuffer, offset + i, offset+ j, 0x0000ff);
          break;
        }
      }
    }
  }

  while (1);

  /* Divide by zero test */
  //int e = 10 / 0;

  /* Timer interrupt test */
  int t = 0;

  while (1)
  {
    sleep(1);
    printf("0x%x\n", t);
    t++;
  }
}
