; Access to disk in real mode using BIOS functions

[bits 16]

; Read from disk
;
; Input:
;   AL = Number of sectors to read
;   DL = Drive to read from
;   CL = Offset to read from (sector size boundary)
;   ES:BX = Memory address where we will read into
;
disk_read:

  pusha

  push ax       ; Save number of sectors to read on stack
                ; Note reading over the stack (thus corrupting the stack)
                ; is possible if the stack pointer is too low.
                ; TODO Add a check for this

  mov ah, 0x02  ; BIOS read sector function
  mov ch, 0x00  ; Cylinder 0
  mov dh, 0x00  ; Head 0

  int 0x13      ; BIOS interrupt for read sector function

  jc disk_error ; Jump if error (carry flag would be set)

  ; Check for disk read error

  pop dx        ; Number of sectors we wanted to read
  cmp dl, al    ; AL now contains number of sectors we actually read
  jne disk_error

  popa
  ret

disk_error:
  mov bx, DISK_ERROR_MSG
  call print_string
  ; TODO Print value of ah (return value), dl and al
  jmp $

DISK_ERROR_MSG: db "Disk read error!", 0
