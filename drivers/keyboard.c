#include "kernel/low_level.h"
#include "kernel/utils.h"
#include "kernel/idt.h"

/* Keyboard data register - stores what keyboard wants to send */
#define KEYBOARD_DATA_REG 0x60

/* Keyboard control register */
#define KEYBOARD_CTRL_REG 0x64

/* Scancode bit set when the key was just released */
#define KEYBOARD_KEY_RELEASE 0x80

/* Keyboard using IRQ1 */
#define KEYBOARD_IRQ_NUM 1

/* Max number of scan codes we can handle */
#define NUM_KEYMAP_ENTRIES 128

/* Scan code to key mapping. Indexed by scan code. */
int keymap[NUM_KEYMAP_ENTRIES] = { 0 };

char keypress_buffer = '\0';

/* Key map values for special keys */
#define KEYBOARD_ERROR      0x1000
#define ESC_KEY             0x1001
#define CTRL_KEY            0x1002
#define LEFT_SHIFT_KEY      0x1003
#define RIGHT_SHIFT_KEY     0x1004
#define KEYPAD_ASTERISK_KEY 0x1005
#define ALT_KEY             0x1006
#define CAPS_LOCK_KEY       0x1007
#define F1_KEY              0x1008
#define F2_KEY              0x1009
#define F3_KEY              0x1010
#define F4_KEY              0x1011
#define F5_KEY              0x1012
#define F6_KEY              0x1013
#define F7_KEY              0x1014
#define F8_KEY              0x1015
#define F9_KEY              0x1016
#define F10_KEY             0x1017
#define NUM_LOCK_KEY        0x1018
#define SCROLL_LOCK_KEY     0x1019
#define KEYPAD_7            0x1020
#define KEYPAD_8            0x1021
#define KEYPAD_9            0x1022
#define KEYPAD_MINUS        0x1023
#define KEYPAD_4            0x1024
#define KEYPAD_5            0x1025
#define KEYPAD_6            0x1026
#define KEYPAD_PLUS         0x1027
#define KEYPAD_1            0x1028
#define KEYPAD_2            0x1029
#define KEYPAD_3            0x1030
#define KEYPAD_0            0x1031
#define KEYPAD_DECIMAL      0x1032
#define ALT_SYS_RSQ_KEY     0x1033
#define FN_KEY              0x1034
#define SUPER_KEY           0x1035
#define F11_KEY             0x1036
#define F12_KEY             0x1037

/* Flag to indicate of Caps Lock is on or not */
int capslock_flag = 0;

/* Flag to indicate if Left or Right Shift is held down */
int shift_held_flag = 0;

/* Handler for keyboard interrupts */
static void keyboard_handler(struct isr_params *isrp)
{
  unsigned char scancode;

  /* Read data from keyboard */
  scancode = port_byte_in(KEYBOARD_DATA_REG);

  /* Handle key release */
  if (scancode & KEYBOARD_KEY_RELEASE)
  {
    int key_pressed = keymap[scancode - KEYBOARD_KEY_RELEASE];

    switch(key_pressed)
    {
      case LEFT_SHIFT_KEY:
      case RIGHT_SHIFT_KEY:
        shift_held_flag = 0;
        break;
    }
  }
  /* Handle key press */
  else
  {
    /* Holding a key down generates multiple key press interrupts */

    int key_pressed = keymap[scancode];

    switch(key_pressed)
    {
      case LEFT_SHIFT_KEY:
      case RIGHT_SHIFT_KEY:
        shift_held_flag = 1;
        break;

      case CAPS_LOCK_KEY:
        capslock_flag = !capslock_flag;
        break;

      default:
        if (capslock_flag || shift_held_flag)
        {
          if (key_pressed >= 'a' && key_pressed <= 'z')
          {
            key_pressed -= 32;
          }
          else if (shift_held_flag)
          {
            switch(key_pressed)
            {
              case '/': key_pressed = '?'; break;
              /* TODO More to add here */
            }
          }
        }
        keypress_buffer = key_pressed;
        break;
    }
  }
}

/* Populate the scan code to key map for a US keyboard */
static void populate_keymap_US()
{
  keymap[0x00] = KEYBOARD_ERROR;
  keymap[0x01] = ESC_KEY;
  keymap[0x02] = '1';
  keymap[0x03] = '2';
  keymap[0x04] = '3';
  keymap[0x05] = '4';
  keymap[0x06] = '5';
  keymap[0x07] = '6';
  keymap[0x08] = '7';
  keymap[0x09] = '8';
  keymap[0x0A] = '9';
  keymap[0x0B] = '0';
  keymap[0x0C] = '-';
  keymap[0x0D] = '=';
  keymap[0x0E] = '\b'; /* Backspace */
  keymap[0x0F] = '\t'; /* Tab */
  keymap[0x10] = 'q';
  keymap[0x11] = 'w';
  keymap[0x12] = 'e';
  keymap[0x13] = 'r';
  keymap[0x14] = 't';
  keymap[0x15] = 'y';
  keymap[0x16] = 'u';
  keymap[0x17] = 'i';
  keymap[0x18] = 'o';
  keymap[0x19] = 'p';
  keymap[0x1A] = '[';
  keymap[0x1B] = ']';
  keymap[0x1C] = '\n'; /* Enter */
  keymap[0x1D] = CTRL_KEY;
  keymap[0x1E] = 'a';
  keymap[0x1F] = 's';
  keymap[0x20] = 'd';
  keymap[0x21] = 'f';
  keymap[0x22] = 'g';
  keymap[0x23] = 'h';
  keymap[0x24] = 'j';
  keymap[0x25] = 'k';
  keymap[0x26] = 'l';
  keymap[0x27] = ';';
  keymap[0x28] = '\'';
  keymap[0x29] = '`';
  keymap[0x2A] = LEFT_SHIFT_KEY;
  keymap[0x2B] = '\\';
  keymap[0x2C] = 'z';
  keymap[0x2D] = 'x';
  keymap[0x2E] = 'c';
  keymap[0x2F] = 'v';
  keymap[0x30] = 'b';
  keymap[0x31] = 'n';
  keymap[0x32] = 'm';
  keymap[0x33] = ',';
  keymap[0x34] = '.';
  keymap[0x35] = '/';
  keymap[0x36] = RIGHT_SHIFT_KEY;
  keymap[0x37] = KEYPAD_ASTERISK_KEY;
  keymap[0x38] = ALT_KEY;
  keymap[0x39] = ' ';
  keymap[0x3A] = CAPS_LOCK_KEY;
  keymap[0x3B] = F1_KEY;
  keymap[0x3C] = F2_KEY;
  keymap[0x3D] = F3_KEY;
  keymap[0x3E] = F4_KEY;
  keymap[0x3F] = F5_KEY;
  keymap[0x40] = F6_KEY;
  keymap[0x41] = F7_KEY;
  keymap[0x42] = F8_KEY;
  keymap[0x43] = F9_KEY;
  keymap[0x44] = F10_KEY;
  keymap[0x45] = NUM_LOCK_KEY;
  keymap[0x46] = SCROLL_LOCK_KEY;
  keymap[0x47] = KEYPAD_7;
  keymap[0x48] = KEYPAD_8;
  keymap[0x49] = KEYPAD_9;
  keymap[0x4A] = KEYPAD_MINUS;
  keymap[0x4B] = KEYPAD_4;
  keymap[0x4C] = KEYPAD_5;
  keymap[0x4D] = KEYPAD_6;
  keymap[0x4E] = KEYPAD_PLUS;
  keymap[0x4F] = KEYPAD_1;
  keymap[0x50] = KEYPAD_2;
  keymap[0x51] = KEYPAD_3;
  keymap[0x52] = KEYPAD_0;
  keymap[0x53] = KEYPAD_DECIMAL;
  keymap[0x54] = ALT_SYS_RSQ_KEY;
  keymap[0x55] = FN_KEY;
  keymap[0x56] = SUPER_KEY;
  keymap[0x57] = F11_KEY;
  keymap[0x58] = F12_KEY;
}

/* Add keyboard handler to IDT */
void keyboard_install()
{
  /* Handle scan codes from US keyboard by default */
  populate_keymap_US();

  irq_install_custom_handler(KEYBOARD_IRQ_NUM, keyboard_handler);
}

/*
 * Get next character from the keyboard
 *
 * Blocks
 */
char keyboard_read() {
    /* TODO Don't want to poll like this, chews up a lot of CPU */
    while (keypress_buffer == '\0') {
        usleep(10 * 1000);
    }
    char c = keypress_buffer;
    keypress_buffer = '\0';
    return c;
}
