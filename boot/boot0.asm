;
; The Boot Loader
;
; We use a two stage boot loader. The reason for this is that we want
; the flexibility of doing more things than we can fit into 512 bytes.
; 512 bytes is the total number of bytes that the BIOS loads from disk
; for the boot loader.
;
; This is stage 0. Because this stage must fit in 512 bytes, we keep it
; simple. The only thing we do here is load stage 1 from disk. The code
; we load for stage 1 is immediately after the stage 0 code on disk.
;

[bits 16]                ; This is real mode: 16-bits

[org 0x7c00]             ; The BIOS will load us at this memory address so
                         ; we inform the assembler using 'org' (origin).
                         ; The assembler will in turn offset memory accesses
                         ; by this value for us.

mov bp, 0x9000           ; Setup the stack
mov sp, bp

BOOT1_OFFSET equ 0x1000  ; Where we will load the 2nd stage of our boot loader

mov [BOOT_DRIVE], dl     ; BIOS stores boot drive in DL so save it

mov bx, MSG_BOOT0        ; Print stage 0 starting message
call print_string
call print_newline

call load_boot1          ; Load the boot stage 1 code into memory

mov dl, [BOOT_DRIVE]     ; Pass the boot drive info to boot stage 1
jmp BOOT1_OFFSET         ; Transfer control to boot stage 1

jmp $                    ; Should never get here

%include "print.asm"     ; Print functions
%include "disk.asm"      ; Disk read functions

load_boot1:
  pusha                  ; Save registers

  mov bx, MSG_LOAD_BOOT1 ; Print loading stage 1 message
  call print_string
  call print_newline

  mov bx, BOOT1_OFFSET   ; Read into BOOT1_OFFSET
  mov al, 1              ; Number of sectors BOOT1_SIZE_CHECK_MARKER
  mov cl, 0x2            ; Starting the 2nd sector (ie after boot sector)
  mov dl, [BOOT_DRIVE]   ; From our boot drive
  call disk_read

  popa                   ; Restore registers
  ret

; Global variables

BOOT_DRIVE     db 0
MSG_BOOT0      db "Starting boot stage 0", 0
MSG_LOAD_BOOT1 db "Loading boot stage 1 into memory", 0

times 510-($-$$) db 0    ; Fill the next bytes up to 510 with zeros

dw 0xaa55                ; Set bytes 511 ans 512 to the magic number that
                         ; the BIOS uses to identify the boot sector.

