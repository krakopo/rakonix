#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

/*
 * BIOS colour attribute codes:
 *
 *  Black          0x0
 *  Blue           0x1
 *  Green          0x2
 *  Cyan           0x3
 *  Red            0x4
 *  Magenta        0x5
 *  Brown          0x6
 *  Light Gray     0x7
 *  Dark Gray      0x8
 *  Light Blue     0x9
 *  Light Green    0xA
 *  Light Cyan     0xB
 *  Light Red      0xC
 *  Light Magenta  0xD
 *  Yellow         0xE
 *  White          0xF
 *
 * Use high bits for background and low bits for text colour.
 */
#define WHITE_ON_BLACK 0x0F
#define YELLOW_ON_BLACK 0x0E

// Screen I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void print_char(char character, int col, int row, char attribute_byte);
int get_cursor();
void set_cursor(int offset);
int get_screen_offset(int col, int row);
int handle_scrolling(int offset);
void print_at(char* message, int col, int row, char attribute_byte);
void print(char *message);
void print_with_attr(char *message, char attribute_byte);
void clear_screen();
