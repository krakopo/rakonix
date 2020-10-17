#include "vesa.h"

int xcursor = 0;
int ycursor = 0;

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 16;

/* Colour codes are coded as 0xRRGGBB */
static int vesa_colour_codes[] =
{
  /* BLACK          */ 0x000000,
  /* BLUE           */ 0x0000ff,
  /* GREEN          */ 0x00ff00,
  /* CYAN           */ 0x00ffff,
  /* RED            */ 0xff0000,
  /* MAGENTA        */ 0xff00ff,
  /* BROWN          */ 0xa52a2a,
  /* LIGHT_GRAY     */ 0xd3d3d3,
  /* DARK_GRAY      */ 0xa9a9a9,
  /* LIGHT_BLUE     */ 0xadd8e6,
  /* LIGHT_GREEN    */ 0x90ee90,
  /* LIGHT_CYAN     */ 0xe0ffff,
  /* LIGHT_RED      */ 0xff9d9d,
  /* LIGHT_MAGENTA  */ 0xff9dff,
  /* YELLOW         */ 0xffff00,
  /* WHITE          */ 0xffffff
};

#define DEFAULT_FGCOLOUR 0xffffff // WHITE
#define DEFAULT_BGCOLOUR 0x000000 // BLACK

int current_fgcolour = DEFAULT_FGCOLOUR;
int current_bgcolour = DEFAULT_BGCOLOUR;

void putpixel(int x, int y, int colour)
{
  unsigned char *screen = (unsigned char *) vbe_mode_info->framebuffer;
  unsigned int where = y * vbe_mode_info->pitch + x * 4;

  /* Draw blue component */
  screen[where] = colour & 0xff;

  /* Draw green component */
  screen[where + 1] = (colour >> 8) & 0xff;

  /* Draw red component */
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

// Input colour codes are from BIOS (see drivers/screen.h)
void vesa_set_text_colour(int bgcolour, int fgcolour)
{
  current_bgcolour = vesa_colour_codes[bgcolour];
  current_fgcolour = vesa_colour_codes[fgcolour];
}

void vesa_reset_text_colour()
{
  current_bgcolour = DEFAULT_BGCOLOUR;
  current_fgcolour = DEFAULT_FGCOLOUR;
}
