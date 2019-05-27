#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "kernel/low_level.h"
#include "kernel/utils.h"
#include "kernel/idt.h"

#define NUM_KEYMAP_ENTRIES 128

/* Keyboard data register - stores what keyboard wants to send */
#define KEYBOARD_DATA_REG 0x60

/* Keyboard control register */
#define KEYBOARD_CTRL_REG 0x64

/* Scancode bit set when the key was just released */
#define KEYBOARD_KEY_RELEASE 0x80

/* Keyboard using IRQ1 */
#define KEYBOARD_IRQ_NUM 1


unsigned char keymap_us[NUM_KEYMAP_ENTRIES] = {
  0, /* scan code 0, unknown key */
  1, /* scan code 1, ESC key */
  '1', /* scan code 2, 1 key */
  '2', /* scan code 3, 2 key */
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  '0',
  '-',
  '=',
  '\b', /* Backspace */
  '\t', /* Tab */
  'q',
  'w',
  'e',
  'r',
  't',
  'y',
  'u',
  'i',
  'o',
  'p',
  '[',
  ']',
  '\n', /* Enter */
  2, /* Ctrl */
  'a',
  's',
  'd',
  'f',
  'g',
  'h',
  'j',
  'k',
  'l',
  ';',
  '\'',
  '`',
  3, /* Left Shift */
  '\\',
  'z',
  'x',
  'c',
  'v',
  'b',
  'n',
  'm',
  ',',
  '.',
  '/',
  4, /* Right Shift */
  '*',
  5, /* Alt */
  ' ', /* Spacebar */
  6, /* Caps Lock */
  7, /* F1 */
  8, /* F2 */
  9, /* F3 */
  10, /* F4 */
  11, /* F5 */
  12, /* F6 */
  13, /* F7 */
  14, /* F8 */
  15, /* F9 */
  16, /* F10 */
  17, /* Num Lock */
  18, /* Scroll Lock */
  19, /* Home key */
  20, /* Up Arrow */
  21, /* Page Up */
  '-',
  22, /* Left Arrow */
  23,
  24, /* Right Arrow */
  '+',
  25, /* End key */
  26, /* Down Arrow */
  27, /* Insert key */
  28, /* Delete key */
  29,
  30,
  31,
  32, /* F11 */
  33, /* F12 */
  0 /* Undefined */
};


/* Handler for keyboard interrupts */
void keyboard_handler(struct isr_params *isrp)
{
  unsigned char scancode;

  /* Read data from keyboard */
  scancode = port_byte_in(KEYBOARD_DATA_REG);

  /* Handle key release */
  if (scancode & KEYBOARD_KEY_RELEASE)
  {
    /* TODO Implement */
  }
  /* Handle key press */
  else
  {
    /* Holding a key down generates multiple key press interrupts */

    print("\nKey pressed: ");
    print_hex(scancode);
    print(" key: ");
    print_char(keymap_us[scancode], -1, -1);
    print("\n");
  }
}

/* Add keyboard handler to IDT */
void keyboard_install()
{
  irq_install_custom_handler(KEYBOARD_IRQ_NUM, keyboard_handler);
}
