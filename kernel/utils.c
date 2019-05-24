#include "drivers/screen.h"

unsigned char *memset(unsigned char *dest, unsigned char value, int num_bytes)
{
  int i = 0;

  for (i = 0; i < num_bytes; i++)
  {
    dest[i] = value;
  }

  return dest;
}

void print_hex(unsigned int i)
{
#define CHARCOUNT 11 // e.g., 0xDEADBEEF\0
  char hexstr[CHARCOUNT];

  hexstr[0] = '0';
  hexstr[1] = 'x';

  int curr_digit = CHARCOUNT - 1;

  hexstr[curr_digit] = '\0';
  curr_digit--;

  while (i != 0)
  {
    int remainder = i % 16;

    if (remainder >= 10)
    {
      hexstr[curr_digit] = 'A' + remainder - 10;
    }
    else
    {
      hexstr[curr_digit] = '0' + remainder;
    }

    i = i / 16;

    curr_digit--;
  }

  // Add leading zeros so we have CHARCOUNT characters
  while (curr_digit > 1)
  {
    hexstr[curr_digit] = '0';
    curr_digit--;
  }

  print(hexstr);
}
