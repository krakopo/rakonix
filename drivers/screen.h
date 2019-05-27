#ifndef SCREEN_H
#define SCREEN_H

/*
 * BIOS colour attribute codes:
 *
 * Use high bits for background and low bits for text colour.
 */
#define BLACK          0x0
#define BLUE           0x1
#define GREEN          0x2
#define CYAN           0x3
#define RED            0x4
#define MAGENTA        0x5
#define BROWN          0x6
#define LIGHT_GRAY     0x7
#define DARK_GRAY      0x8
#define LIGHT_BLUE     0x9
#define LIGHT_GREEN    0xA
#define LIGHT_CYAN     0xB
#define LIGHT_RED      0xC
#define LIGHT_MAGENTA  0xD
#define YELLOW         0xE
#define WHITE          0xF

void print(char *message);
void set_text_colour(unsigned short bgcolour, unsigned short fgcolour);
void reset_text_colour();
void clear_screen();

#endif /* SCREEN_H */
