[bits 16]
[org 0x7c00]

; Find the RSDP

; Start searching from 0xE0000000
mov bx, 0xe000
mov fs, bx

restart_search:
; DX is the offset into the memory we are searching
; We will increment it by 16 each time since the RSDP is on a 16 byte boundary
mov dx, 0

continue_search:
; CX is the offset into the string we are search for
; We increment CX for each matched character and set CX to zero for a mismatch
; We known we have found the string if CX ever reaches the length of the
; string to match.
mov cx, 0

search_next_char:
; Get char from string to match into AL
mov bx, cx
add bx, rsdp_str
mov al, [bx]
; Get char from memory to search into AH
mov bx, cx
add bx, dx
mov ah, [fs:bx]
; Compare
cmp al, ah 
jne char_mismatch
; Print matching character
;mov ah, 0x0e
;int 0x10
; Check if found all chars
cmp cx, rsdp_str_length - 1
je search_stop
; Move to next char in search
add cx, 1
jmp search_next_char

search_stop:
mov bx, fs
call print_hex
mov ah, 0x0e
mov al, ':'
int 0x10
mov bx, dx
call print_hex
mov ah, 0x0e
mov al, [found]
int 0x10
jmp $

char_mismatch:
cmp dx, 0xfff0
je update_segment
add dx, 16 ; Move to next memory location, search string on 16 byte boundary
jmp continue_search

update_segment:
; Continue searching from 0xF0000000
mov bx, 0xf000
mov fs, bx
jmp restart_search
mov ah, 0x0e
mov al, [not_found]
int 0x10
jmp $

%include "print.asm"

rsdp_str:
  db "RSD PTR "
rsdp_str_length equ $ - rsdp_str

found:
  db "X"

not_found:
  db "F"

times 510-($-$$) db 0
dw 0xaa55
