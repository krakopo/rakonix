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

char *itoa(unsigned int i, char *str, int base)
{
  if (i == 0)
  {
    str[0] = '0';
    str[1] = '\0';
    return str;
  }

  unsigned int j = i;

  int num_digits = 0;

  while (j != 0)
  {
    num_digits++;
    j = j / base;
  }

  str[num_digits] = '\0';
  int curr_digit = num_digits - 1;

  while (curr_digit >= 0)
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

static void call_cpuid(unsigned int input, unsigned int *output)
{
  __asm__ __volatile__(
    /* Assembly to run CPUID instruction */
    "cpuid"

    /* Outputs to EAX, EBX, ECX and EDX */
    : "=a" (output[0]), /* Saves EAX register into variable */
      "=b" (output[1]), /* Saves EBX register into variable */
      "=c" (output[2]), /* Saves ECX register into variable */
      "=d" (output[3])  /* Saves EDX register into variable */

    /* Input is EAX */
    : "a" (input)       /* Sets EAX */ );
}

void cpuid()
{
  /*
   * Check for the CPUID instruction by trying to modify the ID flag in EFLAGS.
   * If the ID flag cannot by modified, the CPUID instruction is not supported.
   */

  /* EFLAGS ID field is bit number 21 */
  const unsigned int EFLAGS_ID_BIT = 0x200000;

  /* Get current value of EFLAGS using PUSHF & POP */
  unsigned int eflags = 0;
  __asm__ __volatile__ ("pushf; pop %0" : "=r" (eflags));

  /* Set the ID bit and update EFLAGS using POPF */
  unsigned int old_eflags = eflags;
  eflags |= EFLAGS_ID_BIT;
  __asm__ __volatile__ ("push %0; popf" : : "r" (eflags));

  /* Re-read EFLAGS to see if our change is still there */
  __asm__ __volatile__ ("pushf; pop %0" : "=r" (eflags));

  if (!(eflags & EFLAGS_ID_BIT))
  {
    printf("CPUID instruction is not supported.\n");
    return;
  }

  /*
   * Execute the CPUID instruction with EAX equal to 0x80000000. If the value
   * returned in EAX is greater than 0x80000000, the brand string feature is
   * supported and we can use 0x80000002 to 0x80000004 to identify the processor.
   */

  const unsigned int CPUID_INPUT_DETECT = 0x80000000;

  unsigned int cpuid_eax_out = 0;
  unsigned int cpu_vendor_str[4];

  call_cpuid(CPUID_INPUT_DETECT, cpu_vendor_str);

  cpuid_eax_out = cpu_vendor_str[0]; /* Save EAX */

  cpu_vendor_str[0] = cpu_vendor_str[1]; /* String starts at EBX */

  /* Swap 1 and 3 since the string is EBX + EDX + ECX */
  cpu_vendor_str[1] = cpu_vendor_str[3];

  cpu_vendor_str[3] = 0; /* Set NULL terminator */

  printf("CPU Vendor: %s\n", (char *) cpu_vendor_str);

  if (cpuid_eax_out > CPUID_INPUT_DETECT)
  {
    unsigned int cpu_brand_str[12];

    const unsigned int CPUID_BRAND_STRING_INPUT_1 = 0x80000002;
    const unsigned int CPUID_BRAND_STRING_INPUT_2 = 0x80000003;
    const unsigned int CPUID_BRAND_STRING_INPUT_3 = 0x80000004;

    call_cpuid(CPUID_BRAND_STRING_INPUT_1, cpu_brand_str);
    call_cpuid(CPUID_BRAND_STRING_INPUT_2, cpu_brand_str + 4);
    call_cpuid(CPUID_BRAND_STRING_INPUT_3, cpu_brand_str + 8);

    printf("CPU Brand: %s\n", cpu_brand_str);
  }
}
