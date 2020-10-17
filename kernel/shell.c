/*
 * This is the shell for the kernel.
 *
 */

#include "utils.h"
#include "drivers/keyboard.h"

#define MAX_COMMAND_LENGTH 64

void getcommand_keyboard(char *buffer, int buffsize) {
    int readcount = 0;
    while (readcount < (buffsize - 1)) {
        char c = keyboard_read();
        if (c == '\n') {
            break;
        } else {
            if (c == '\b') {
                if (readcount > 0) {
                    readcount--;
                    printf("\b%c_", c);
                }
            } else {
                *(buffer + readcount) = c;
                readcount++;
                printf("\b%c_", c);
            }
        }
    }
    printf("\b\n");
    *(buffer + readcount) = '\0';
}

void run_shell() {
    printf("\nStarting kernel shell\n");
    char command[MAX_COMMAND_LENGTH] = { '\0' };
    while (1) {
        printf("> _");
        getcommand_keyboard(command, MAX_COMMAND_LENGTH);
        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strlen(command) == 0) {
            continue;
        } else {
            printf("Unrecognized command: '%s'\n", command);
        }
    }
}
