; A simple boot sector that prints "Hello" using a BIOS routine
; This version uses a helper function we define to print strings

; Tell assembler we want to offset by 0x7c00 since that's where
; the BIOS will load this boot program.
;
[org 0x7c00]

mov bx, hello_msg
call print_string
call print_newline

mov bx, 0x1234
call print_hex
call print_newline

mov bx, 0x5678
call print_hex
call print_newline

mov bx, 0x9abc
call print_hex
call print_newline

mov bx, 0xdeaf
call print_hex
call print_newline

mov bx, $$
call print_hex
call print_newline

mov bx, $
call print_hex
call print_newline

jmp $ ; Repeatedly jump to the current instruction (loops forever)

; Include our library with the print_string function
;
%include "print.asm"

; The string we want to print
;
hello_msg:
  db "Hello world! ", 0 ; Zero indicates end of string

; The include command and string definition above must come after the
; "jmp $" instruction. Otherwise code in the include will be executed
; and bytes in the string will be emmitted where they dont belong.

; Padding before the magic number
times 510-($-$$) db 0

; Last two bytes are boot sector magic number
dw 0xaa55
