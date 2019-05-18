; A boot sector that boots a C kernel in 32-bit protected mode

[org 0x7c00]

KERNEL_OFFSET equ 0x1000 ; This is where we will load the kernel into memory

mov [BOOT_DRIVE], dl     ; BIOS stores boot drive in DL so save it

mov bp, 0x9000           ; Setup the stack
mov sp, bp

mov bx, MSG_REAL_MODE    ; Say hi from 16-bit real mode
call print_string
call print_newline

call load_kernel         ; Load the kernel into memory

call switch_to_pm        ; Switch to protected mode

jmp $

; Includes

%include "print.asm"
%include "disk.asm"
%include "gdt.asm"
%include "protected_mode.asm"

[bits 16]

; load_kernel
load_kernel:
	mov bx, MSG_LOAD_KERNEL ; Say we're loading the kernel now
	call print_string
  call print_newline
	mov bx, KERNEL_OFFSET   ; Read into KERNEL_OFFSET
  ; Next line we load just enough sectors because qemu doesn't
  ; seem to let us read more than what exists in the bootable
  ; disk image we provide it. I guess that makes sense but we
  ; need to keep this in mind. If our kernel code ever exceeds this
  ; we need to change it to read more sectors otherwise we wont load
  ; the kernel fully.
	mov dh, 3               ; 3 sectors
	mov dl, [BOOT_DRIVE]    ; From our boot drive
	call disk_read
	ret

[bits 32]
; This is where we start after entering protected mode
BEGIN_PM:
	mov ebx, MSG_PROTECT_MODE ; Say hit from 32-bit protected mode
  call print_string_pm
	call KERNEL_OFFSET        ; Run our kernel!
	jmp $

; Global variables
BOOT_DRIVE       db 0
MSG_REAL_MODE    db "Started in 16-bit Real Mode", 0
MSG_PROTECT_MODE db "Successfully entered 32-bit Protected Mode", 0
MSG_LOAD_KERNEL  db "Loading kernel in memory", 0

; Boot sector padding
times 510-($-$$) db 0
dw 0xaa55
