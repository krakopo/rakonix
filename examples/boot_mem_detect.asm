[bits 16]
[org 0x7c00]

; Detect available memory using BIOS function 0xE820 with INT 0x15
;
; Steps:
;
;   First call:
;      1. Set ES:DI to destination buffer address for the list of memory regions
;      2. Set EBX to zero
;      3. Set EDX to magic number 0x534D4150
;      4. Set EAX to 0xE820
;      5. Set ECX to 24
;      6. Execute instruction INT 0x15
;      7. If successful, EAX will be set to 0x534D4150 and the carry flag will be clear
;      8. EBX will be some value which must not change for the next call to work
;      9. CL will contain the number of bytes stored at ES:DI (probably 20)
;     10. Now you can read the information contained at ES:DI (see below for details)
;
;   Subsequent calls:
;     1. Increment DI by the number of bytes in CL
;     2. Reset EAX to 0xE820 and ECX to 24
;     3. Execute instruction INT 0x15
;     4. Now you can read the information contained at ES:DI (see below for details)
;     5. Repeat steps 1-4 until EBX is 0
;
; Format of 20 byte buffer returned:
;   8 bytes: Starting address of memory region
;   8 bytes: Length of memory region
;   4 bytes: Type of memory region (1 = Free, 2 = Reserved, ? = Reserved)

mov di, 0xB000
mov ebx, 0
mov edx, 0x534D4150
mov eax, 0xE820
mov ecx, 24
int 0x15

jc end        ; BIOS call failed if carry set ...
cmp eax, edx  ; ... or if EAX not equal to 0x534D4150
jne end

; Check the size of the buffer is 20 bytes. It might be 24 in some cases but
; apparently this is rare so we only handle the 20 byte case for now.
cmp cl, 20
jne end

print_memory_region:
; Save value of EBX on stack for next call
push ebx
; Base addr of memory region
; Little endian: most significant byte in highest address
; So start printing (16-bit) values from high to low addresses
mov bx, [di + 6]
call print_hex
mov bx, [di + 4]
call print_hex
mov bx, [di + 2]
call print_hex
mov bx, [di]
call print_hex
call print_space

; Length of memory region
add di, 8
mov bx, [di + 6]
call print_hex
mov bx, [di + 4]
call print_hex
mov bx, [di + 2]
call print_hex
mov bx, [di]
call print_hex
call print_space

; Type of memory region
add di, 8
mov bx, [di + 2]
call print_hex
mov bx, [di]
call print_hex
call print_newline

; Make the next call
pop ebx
cmp ebx, 0      ; Done if ebx is zero
je end
add di, 4       ; Add last 4 bytes to DI (20 total, value in CL)
mov eax, 0xE820
mov ecx, 24
int 0x15
jc end
jmp print_memory_region

end:
jmp $

%include "print.asm"

times 510-($-$$) db 0
dw 0xaa55
