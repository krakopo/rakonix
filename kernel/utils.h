#ifndef UTILS_H
#define UTILS_H

unsigned char *memset(unsigned char *dest, unsigned char value, int num_bytes);
unsigned char *memcpy(unsigned char *dest, unsigned char *src, int num_bytes);
void sleep(unsigned int seconds);
void printf(const char *fmt, ...);
char *strcpy(char *to, const char *from);
char *itoa(int i, char *str, int base);
void cpuid();
void set_text_colour(int bgcolour, int fgcolour);
void reset_text_colour();
void clear_screen();

#endif /* UTILS_H */
