; A simple boot sector program that loops forever

loop:
jmp loop
times 510-($-$$) db 0 ; The program needs to be exactly 512 bytes with the
                      ; last two bytes being the magic number 0xaa55.
                      ; This command tells the assembler to pad up to the
                      ; 510th bit with zeros (db 0).
dw 0xaa55
