unsigned char *memset(unsigned char *dest, unsigned char value, int num_bytes)
{
  int i = 0;

  for (i = 0; i < num_bytes; i++)
  {
    dest[i] = value;
  }

  return dest;
}
