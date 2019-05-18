[bits 16]

; Read dh sectors to es:bx from drive dl
;
disk_read:
  pusha
  push dx       ; Save dx on stack so we can recall how many sectors
                ; we expect to read
  mov ah, 0x02  ; BIOS read sector function
  mov al, dh    ; Read dh sectors
  mov ch, 0x00  ; Cylinder 0
  mov dh, 0x00  ; Head 0
  mov cl, 0x02  ; Start reading from 2nd sector (ie sector after boot sector)
  int 0x13      ; BIOS interrupt for read sector function

  jc disk_error ; Jump if error (carry flag would be set)

  ; Check for error, al will contain number of sectors actually read
  pop dx
  cmp dh, al
  jne disk_error
  popa
  ret

disk_error:
  mov bx, DISK_ERROR_MSG
  call print_string
  jmp $

DISK_ERROR_MSG: db "Disk read error!", 0
