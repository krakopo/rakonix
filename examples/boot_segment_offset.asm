; Demonstrate segment offsetting

mov ah, 0x0e

; Prints garbage, incorrect addressing
mov al, [the_secret]
int 0x10

; Prints "X"
mov bx, 0x7c0 ; Not 0x7c00 since segment addressing multiplies this by 16
              ; So we use 0x7c00 / 16 = 0x7c0
mov ds, bx
mov al, [the_secret] ; ds is the default segment in this context
int 0x10

; Prints garbage since es is not set properly
mov al, [es:the_secret]
int 0x10

; Prints "X"
mov bx, 0x7c0
mov es, bx
mov al, [es:the_secret] ; Explicitly use es segment
int 0x10

jmp $

the_secret:
  db "X"

times 510-($-$$) db 0
dw 0xaa55
