// Reads a byte from the specified port
unsigned char port_byte_in(unsigned short port)
{
  unsigned char result;

  // "=a" (result) means put AL register in variable RESULT
  // "d" (port) means load EDX with port
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

  return result;
}

// Writes a byte to the specified port
void port_byte_out(unsigned short port, unsigned char data)
{
  // "a" (data) means load EAX with data
  // "d" (port) means load EDX with port
  __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

// Reads a word from the specified port
unsigned short port_word_in(unsigned short port)
{
  unsigned short result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

// Writes a word to the specified port
void port_word_out(unsigned short port, unsigned short data)
{
  __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
