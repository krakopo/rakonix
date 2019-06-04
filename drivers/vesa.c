void putpixel(unsigned char *screen, int x, int y, int color)
{
  unsigned int where = y * 0x1000 + x * 4;
  screen[where] = color & 0xff;
  screen[where + 1] = (color >> 8) & 0xff;
  screen[where + 2] = (color >> 16) & 0xff;
}
