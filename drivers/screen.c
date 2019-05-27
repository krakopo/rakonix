#include "screen.h"
#include "kernel/low_level.h"

/* Screen constants */
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

/* Screen I/O ports */
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

// Attribute byte for the text we print.
// Controls the background and foreground colour.
unsigned int attribute_byte = 0;

// Given a row and column, return the corresponding screen offset
static int get_screen_offset(int col, int row)
{
  int offset = 0;

  if (col >= 0 && row >= 0)
  {
    offset = (row * 2*MAX_COLS) + (col * 2);
  }

  return offset;
}

// Get the offset of the current cursor location as stored by the
// screen device.
static int get_cursor()
{
  int offset = 0;

  // The screen device uses its control register as an index to
  // select its internal registers of which we are interested in:
  //   reg 14: high byte of cursor's location
  //   reg 15: low byte of cursor's location
  // Once the internal register has been selected we may read or
  // write a byte on the data register.

  port_byte_out(REG_SCREEN_CTRL, 14);
  offset = port_byte_in(REG_SCREEN_DATA) << 8;

  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);

  // Convert from character offset to 2-byte offset
  return offset * 2;
}

// Set the current cursor location for the screen device based on the
// given offset.
static void set_cursor(int offset)
{
  // Convert from 2-byte offset to character offset
  offset /= 2;

  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));

  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (unsigned char)offset);
}

// Do the right thing when we go beyond the last row
static int handle_scrolling(int offset)
{
  int current_row = offset / (2*MAX_COLS);

  if (current_row == MAX_ROWS)
  {
    int new_offset = get_screen_offset(0, MAX_ROWS - 1);

    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    int i = 0;
    for (i = 0; i < 2*MAX_COLS*MAX_ROWS; i++)
    {
      vidmem[i] = vidmem[2*MAX_COLS + i];
    }

    return new_offset;
  }

  return offset;
}

// Print a character on the screen at col, row or at cursor position
static void print_char(char character, int col, int row)
{
  unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
  int offset = 0;

  // If we didn't get a valid attribute byte then use the default.
  if (!attribute_byte)
  {
    reset_text_colour();
  }

  // If we got a valid position on screen use that otherwise use
  // whatever position the cursor is at.
  if (col >= 0 && row >= 0)
  {
    offset = get_screen_offset(col, row);
  }
  else
  {
    offset = get_cursor();
  }

  // If we got a newline character, set the offset at the end of the current
  // row so that when we print the next character it will be on the next row.
  if (character == '\n')
  {
    int current_row = offset / (2*MAX_COLS);
    offset = get_screen_offset(MAX_COLS - 1, current_row);
  }
  // If it's not a newline character just print it.
  else
  {
    vidmem[offset] = character;
    vidmem[offset + 1] = attribute_byte;
  }

  // Increment the offset by 2 (+1 for the attribute byte) for the
  // next character.
  offset += 2;

  // Account for any scrolling required (ie if we're moving past the last row).
  offset = handle_scrolling(offset);

  // Update the cursor value.
  set_cursor(offset);
}

// Print a string at a given column and row
static void print_at(char *message, int col, int row)
{
  char *character = message;

  while (*character != '\0')
  {
    print_char(*character, col, row);
    character++;
  }
}

// Print a string at the current cursor location with default attributes
void print(char *message)
{
  print_at(message, -1, -1);
}

// Clear the screen and set cursor to the top left
void clear_screen()
{
  int row = 0;
  int col = 0;

  for (row = 0; row < MAX_ROWS; row++)
  {
    for (col = 0; col < MAX_COLS; col++)
    {
      print_char(' ', col, row);
    }
  }

  set_cursor(get_screen_offset(0,0));
}

// Set the attribute byte to the desired background and foreground colour
void set_text_colour(unsigned short bgcolour, unsigned short fgcolour)
{
  attribute_byte = (bgcolour << 4) | (fgcolour & 0x0F);
}

// Set the attribute byte to the default background and foreground colour
void reset_text_colour()
{
  set_text_colour(BLACK, WHITE);
}
