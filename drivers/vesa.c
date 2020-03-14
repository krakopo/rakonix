#include "vesa.h"

int xcursor = 0;
int ycursor = 0;

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 16;

#define DEFAULT_FGCOLOUR 0xffffff // WHITE
#define DEFAULT_BGCOLOUR 0x000000 // BLACK

int current_fgcolour = DEFAULT_FGCOLOUR;
int current_bgcolour = DEFAULT_BGCOLOUR;

void putpixel(int x, int y, int colour)
{
  unsigned char *screen = (unsigned char *) vbe_mode_info->framebuffer;
  unsigned int where = y * vbe_mode_info->pitch + x * 4;
  screen[where] = colour & 0xff;
  screen[where + 1] = (colour >> 8) & 0xff;
  screen[where + 2] = (colour >> 16) & 0xff;
}

void drawfontchar(unsigned char *font, unsigned char c, int x, int y, int fgcolour, int bgcolour)
{
  int cx, cy;
  int mask[8] = {1,2,4,8,16,32,64,128};
  unsigned char *glyph = font + (unsigned int)c * 16 /* bytes per glyph */;

  for(cy = 0; cy < FONT_HEIGHT; cy++)
  {
    for(cx = 0; cx < FONT_WIDTH; cx++)
    {
      int colour = glyph[cy] & mask[cx] ? fgcolour : bgcolour;
      putpixel(x + ((FONT_WIDTH-1) - cx), y+cy, colour);
    }
  }
}

void drawchar(unsigned char c, int x, int y, int fgcolour, int bgcolour)
{
  if (fgcolour < 0)
  {
    fgcolour = current_fgcolour;
  }

  if (bgcolour < 0)
  {
    bgcolour = current_bgcolour;
  }

  if (x >= 0 && y >= 0)
  {
    drawfontchar(fonts, c, x, y, fgcolour, bgcolour);
  }
  else
  {
    drawfontchar(fonts, c, xcursor, ycursor, fgcolour, bgcolour);
    xcursor += FONT_WIDTH;
    if (xcursor >= vbe_mode_info->width)
    {
      xcursor = 0;
      ycursor += FONT_HEIGHT;
    }
  }
}

void vesa_print(char *str)
{
  char *p = str;
  while (*p != '\0')
  {
    if (*p == '\n')
    {
      xcursor = 0;
      ycursor += FONT_HEIGHT;
    }
    else if (*p == '\b')
    {
        if (xcursor > 0) {
            xcursor -= FONT_WIDTH;
            drawchar(' ', -1, -1, -1, -1);
            xcursor -= FONT_WIDTH;
        }
    }
    else
    {
      drawchar(*p, -1, -1, -1, -1);
    }
    p++;
  }
}

int vesa_supported()
{
  return vbe_mode_info->framebuffer != 0;
}

void vesa_clear_screen()
{
  int i, j;
  for (j = 0; j < vbe_mode_info->height; j++)
  {
    for (i = 0; i < vbe_mode_info->width; i++)
    {
      putpixel(i, j, current_bgcolour);
    }
  }
}

void vesa_set_text_colour(int bgcolour, int fgcolour)
{
  current_bgcolour = bgcolour;
  current_fgcolour = fgcolour;
}

void vesa_reset_text_colour()
{
  vesa_set_text_colour(DEFAULT_BGCOLOUR, DEFAULT_FGCOLOUR);
}
