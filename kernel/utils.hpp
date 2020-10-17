#ifndef UTILS_H
#define UTILS_H

void *memset(void *dest, unsigned char value, unsigned int num_bytes);
void *memcpy(void *dest, const void *src, unsigned int num_bytes);
int memcmp(const void *s1, const void *s2, unsigned int num_bytes);
void sleep(unsigned int seconds);
void usleep(unsigned int microseconds);
void printf(const char *fmt, ...);
char *strcpy(char *to, const char *from);
int strcmp(const char *s1, const char *s2);
unsigned int strlen(const char *s);
char *itoa(int i, char *str, int base);
void cpuid();
void set_text_colour(int bgcolour, int fgcolour);
void reset_text_colour();
void clear_screen();
void wait_interrupt();
#endif /* UTILS_H */
