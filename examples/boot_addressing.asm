; The BIOS loads our boot program at 0x7c00
;
; This command tells the assembler to add 0x7c00 to addresses
; before dereferencing.
;
[org 0x7c00]

mov ah, 0x0e

mov al, [the_secret]
int 0x10

; Without [org 0x7c00] at the top of the program, we would need to
; do the following:
;
;mov bx, the_secret
;add bx, 0x7c00
;mov al, [bx]
;int 0x10

jmp $

the_secret:
db "X"

times 510-($-$$) db 0
dw 0xaa55 
