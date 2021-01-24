[bits 16]

; Detect available memory using BIOS function 0xE820 with INT 0x15.
; See memory detect example for details on how this works.
;
; Input:  DI set to buffer address to store memory map
;         AX set to addres to store number of memory map entries
;
; Output: Return number of memory map entries in address stored in AX

detect_memory:

pusha
push eax

mov ebx, 0
mov edx, 0x534D4150
mov eax, 0xE820
mov ecx, 24
int 0x15

jc detect_memory_fail  ; BIOS call failed if carry set ...
cmp eax, edx           ; ... or if EAX not equal to 0x534D4150
jne detect_memory_fail

next_memory_region:
; CX contains length of memory region, add to DI in case we make another call
add di, cx
; Found a memory region, increment return value
pop eax
mov ecx, [eax]
add ecx, 1
mov [eax], ecx
push eax

; We're done if ebx is zero
cmp ebx, 0
je detect_memory_end

; Prepare for next call
; DI incremented by length in CX above
mov eax, 0xE820
mov ecx, 24
int 0x15
jc detect_memory_fail
jmp next_memory_region

detect_memory_end:
pop eax
popa
ret

detect_memory_fail:
pop eax
mov ebx, 0
mov [eax], ebx
push eax
jmp detect_memory_end
