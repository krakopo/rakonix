;
; The Boot Loader
;
; We use a two stage boot loader. This is stage 1 (the second and last stage).
; This stage does all the work that needs to be done before transferring
; control to the kernel.
;

[bits 16]

mov [BOOT_DRIVE], dl      ; Boot stage 1 stores boot drive in DL so save it

mov bx, MSG_BOOT1         ; Print stage 1 starting message
call print_string
call print_newline

KERNEL_OFFSET equ 0x2000  ; This is where we are going to load the kernel
                          ; into memory. We will jmp here to transfer control
                          ; to the kernel.

;mov ax, 0x4f02 ; set vbe mode function
;mov bx, 0x0144 ; desired mode
;or bx, 0x4000  ; set bit 14 for linearframebuffer
;int 0x10

call load_kernel          ; Load the kernel into memory

jmp switch_to_pm          ; Switch to protected mode and jmp to BEGIN_PM

jmp $                     ; Never get here

%include "print.asm"      ; Print functions
%include "disk.asm"       ; Disk read functions

load_kernel:
  pusha                   ; Save registers

  mov bx, MSG_LOAD_KERNEL ; Print loading the kernel message
  call print_string
  call print_newline

  mov bx, KERNEL_OFFSET   ; Read into KERNEL_OFFSET
  ; On the next line we load just enough sectors because qemu doesn't
  ; seem to let us read more than what exists in the bootable disk
  ; image we provide it. I guess that makes sense but we need to keep
  ; this in mind. If our kernel code ever exceeds this we need to change
  ; it to read more sectors otherwise we wont load the kernel fully.
  mov al, 25              ; Number of sectors KERNEL_SIZE_CHECK_MARKER
  mov dl, [BOOT_DRIVE]    ; From our boot drive
  mov cl, 0x3             ; Start from offset 0x3 (after boot loaders)
  call disk_read

  popa                    ; Restore registers
  ret

%include "gdt.asm"            ; Defines our GDT
%include "protected_mode.asm" ; Code to enter protected mode

[bits 32]                     ; Protected mode code below here: 32-bits

BEGIN_PM:
  mov ebx, MSG_PROTECT_MODE   ; Print message that we are in protected mode
  call print_string_pm

  jmp KERNEL_OFFSET           ; Run the kernel!

  jmp $                       ; Should never get here

; Global variables

BOOT_DRIVE db 0
MSG_BOOT1        db "Starting boot stage 1", 0
MSG_PROTECT_MODE db "Successfully entered 32-bit Protected Mode", 0
MSG_LOAD_KERNEL  db "Loading kernel in memory", 0

; Need multiple of sector size so we know where to load the kernel from.
; This wastes some space but keeping things on sector boundaries simplifies
; the logic when loading into memory from disk.
times 512-($-$$) db 0

