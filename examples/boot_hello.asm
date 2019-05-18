; A simple boot sector that prints "Hello" using a BIOS routine

; Interrupt 0x10 with ah = 0x0e calls scrolling teletype BIOS routine
; which will print the value in al

mov ah, 0x0e

mov al, 'H'
int 0x10
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
mov al, 'l'
int 0x10
mov al, 'o'
int 0x10

jmp $ ; Repeatedly jump to the current instruction (loops forever)

; Padding before the magic number
times 510-($-$$) db 0

; Last two bytes are boot sector magic number
dw 0xaa55
