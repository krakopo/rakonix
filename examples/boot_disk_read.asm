; Read sectors from boot disk

; Set program origin to 0x7c00 where we expect BIOS will load it
; since this is a boot sector program
[org 0x7c00]

; BIOS stores boot drive in dl so save it
mov [BOOT_DRIVE], dl

; Set stack out of the way at 0x8000
mov bp, 0x8000
mov sp, bp

mov bx, 0x9000       ; Load to address 0x9000
mov al, 2            ; Read 2 sectors
mov cl, 2            ; Start from offset 2 (first sector after boot sector)
mov dl, [BOOT_DRIVE] ; Set disk to use in dl
call disk_read

; Print data read from 2nd sector
mov bx, [0x9000]
call print_hex

call print_newline

; Print data read from 3rd sector
mov bx, [0x9000 + 512]
call print_hex

jmp $

%include "print.asm"
%include "disk.asm"

BOOT_DRIVE: db 0

; Boot sector padding
times 510-($-$$) db 0
dw 0xaa55

; Load the next 2 sectors with some data which we will read
; to test that our disk read logic works
times 256 dw 0xdead ; Sector full of 0xdead
times 256 dw 0xbeef ; Sector full of 0xbeef
