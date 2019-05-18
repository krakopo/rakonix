; A boot sector that enters 32-bit protected mode

[bits 16]

[org 0x7c00]

; Setup the stack
mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call switch_to_pm ; We never return from here

jmp $

%include "print.asm"
%include "gdt.asm"
%include "protected_mode.asm"

[bits 32]

BEGIN_PM:

  mov ebx, MSG_PROTECT_MODE
  call print_string_pm

  jmp $

; Global variables
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROTECT_MODE db "Successfully entered 32-bit Protected Mode", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
