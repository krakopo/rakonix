#include "drivers/screen.h"
#include "kernel/timer.h"

unsigned char *memset(unsigned char *dest, unsigned char value, int num_bytes)
{
  int i = 0;

  for (i = 0; i < num_bytes; i++)
  {
    dest[i] = value;
  }

  return dest;
}

unsigned char *memcpy(unsigned char *dest, unsigned char *src, int num_bytes)
{
  int i = 0;

  for (i = 0; i < num_bytes; i++)
  {
    dest[i] = src[i];
  }

  return dest;
}

void sleep(unsigned int seconds)
{
  timer_wait(seconds_to_ticks(seconds));
}

char *itoa(int i, char *str, int base)
{
  if (i == 0)
  {
    str[0] = '0';
    str[1] = '\0';
    return str;
  }

  int j = i;

  int num_digits = 0;

  while (j != 0)
  {
    num_digits++;
    j = j / base;
  }

  int curr_digit = num_digits;
  str[curr_digit] = '\0';
  curr_digit--;

  while (i != 0)
  {
    int remainder = i % base;

    if (base != 16)
    {
      str[curr_digit] = '0' + remainder;
    }
    else
    {
      if (remainder >= 10)
      {
        str[curr_digit] = 'A' + remainder - 10;
      }
      else
      {
        str[curr_digit] = '0' + remainder;
      }
    }

    i = i / base;

    curr_digit--;
  }

  return str;
}

char *strcpy(char *to, const char *from)
{
  char *t = to;
  const char *f = from;

  while (*f != '\0')
  {
    *t = *f;
    t++;
    f++;
  }

  *t = '\0';

  return to;
}

void printf(const char *fmt, ...)
{
  __builtin_va_list list;
  __builtin_va_start(list, fmt);
  char str[256];
  memset(str, '\0', 256);
  char *c = str;
  const char *f = fmt;
  while (*f != '\0')
  {
    if (*f == '%')
    {
      f++;
      switch (*f)
      {
        case 's':
          strcpy(c, __builtin_va_arg(list, char *));
          break;

        case 'c':
          *c = __builtin_va_arg(list, int);
          break;

        case 'd':
          itoa(__builtin_va_arg(list, int), c, 10);
          break;

        case 'x':
          itoa(__builtin_va_arg(list, int), c, 16);
          break;

        case 'b':
          itoa(__builtin_va_arg(list, int), c, 2);
          break;

        case '%':
        default:
          *c = '%';
          break;
      }
    }
    else
    {
      *c = *f;
    }
    f++;
    while (*c != '\0')
    {
      c++;
    }
  }
  *c = '\0';
  __builtin_va_end(list);
  print(str);
}
