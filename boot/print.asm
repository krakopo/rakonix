[bits 16]

; Print the character in al
print_char:
pusha
mov ah, 0x0e
int 0x10
popa
ret

; Print the string at the address stored in bx
;
; We do this by repeatedly calling a BIOS routine that prints a
; character to the screen. The BIOS routine is called by setting
; ah = 0x0e and al = <character to print> then firing interrupt 0x10.
;
print_string:
  pusha
  mov ah, 0x0e
print_next_char:
  mov al, [bx]
  cmp al, 0
  je print_string_finished
  int 0x10
  add bx, 1
  jmp print_next_char
print_string_finished:
  popa
  ret

; Print 16-bit decimal number in bx to screen
;
print_decimal:
  pusha
  mov ax, bx ; store bx in ax for DIV
  mov bx, 0  ; keep count of number of digits to print
print_decimal_loop:
  ; DIV: Divide AX by operand, remainder goes into DX, quotient into AX
  xor dx, dx ; clear dx first
  mov cx, 10
  div cx
  push dx    ; put digit to print on stack
  add bx, 1
  cmp ax, 0
  je print_decimal_print
  jmp print_decimal_loop
print_decimal_print:
  cmp bx, 0
  je print_decimal_end
  pop cx
  call print_digit
  sub bx, 1
  jmp print_decimal_print
print_decimal_end:
  popa
  ret

; Print 16-bit hex number in bx to screen
;
print_hex:
  pusha

  ; First print the hex number prefix
  mov dx, bx
  ;mov bx, HEX_PRE
  ;call print_string

  ; Print each of the 4 hex digits (16 bit number)

  mov cx, dx
  and cx, 0xf000   ; Mask out bits we dont care about
  shr cx, 12       ; Shift right 3 4-bit digits (12 bits)
  call print_digit

  mov cx, dx
  and cx, 0x0f00
  shr cx, 8        ; Shift right 2 4-bit digits (8 bits)
  call print_digit

  mov cx, dx
  and cx, 0x00f0
  shr cx, 4        ; Shift right 1 4-bit digit (4 bits)
  call print_digit

  mov cx, dx
  and cx, 0x000f
  call print_digit

  popa
  ret

; Prefix for printing hex numbers used by print_hex function
;
HEX_PRE:
  db "0x", 0

print_digit:
  pusha
  cmp cl, 9
  jg is_alpha
  mov al, cl
  add al, 48 ; 48 ('0')
  jmp print
is_alpha:
  mov al, cl
  add al, 87 ; 87 = 97 ('a') - 10 (0xa)
print:
  mov ah, 0x0e
  int 0x10
  popa
  ret

;print_digit:
;  pusha
;  cmp cx, 0
;  je zero
;  cmp cx, 1
;  je one
;  cmp cx, 2
;  je two
;  cmp cx, 3
;  je three
;  cmp cx, 4
;  je four
;  cmp cx, 5
;  je five
;  cmp cx, 6
;  je six
;  cmp cx, 7
;  je seven
;  cmp cx, 8
;  je eight
;  cmp cx, 9
;  je nine
;  cmp cx, 10
;  je a
;  cmp cx, 11
;  je b
;  cmp cx, 12
;  je c
;  cmp cx, 13
;  je d
;  cmp cx, 14
;  je e
;  cmp cx, 15
;  je f
;zero:
;  mov al, '0'
;  jmp print
;one:
;  mov al, '1'
;  jmp print
;two:
;  mov al, '2'
;  jmp print
;three:
;  mov al, '3'
;  jmp print
;four:
;  mov al, '4'
;  jmp print
;five:
;  mov al, '5'
;  jmp print
;six:
;  mov al, '6'
;  jmp print
;seven:
;  mov al, '7'
;  jmp print
;eight:
;  mov al, '8'
;  jmp print
;nine:
;  mov al, '9'
;  jmp print
;a:
;  mov al, 'a'
;  jmp print
;b:
;  mov al, 'b'
;  jmp print
;c:
;  mov al, 'c'
;  jmp print
;d:
;  mov al, 'd'
;  jmp print
;e:
;  mov al, 'e'
;  jmp print
;f:
;  mov al, 'f'
;  jmp print
;print:
;  mov ah, 0x0e
;  int 0x10
;  popa
;  ret

; Print a space
print_space:
  pusha
  mov al, ' '
  call print_char
  popa
  ret

; Move cursor to the start of the next line
;
print_newline:
  pusha
  mov ah, 0x0e

  ; Carriage return (\r)
  mov al, 13
  int 0x10

  ; Newline (\n)
  mov al, 10
  int 0x10

  popa
  ret

[bits 32]

; 32-bit protected mode

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; Print the string at the address stored in ebx
print_string_pm:
  pusha
  mov edx, VIDEO_MEMORY       ; Set edx to start of video memory
print_string_pm_loop:
  mov al, [ebx]               ; Store char at ebx in al
  mov ah, WHITE_ON_BLACK      ; Store attributes in ah
  cmp al, 0                   ; Check for end of string (al = 0)
  je print_string_pm_finished ; Jump to end if end of string
  mov [edx], ax               ; Store char and attrs in current character cell
  add ebx, 1                  ; Increment ebx to next char in string
  add edx, 2                  ; Move to next character cell in video memory
  jmp print_string_pm_loop    ; Loop for next char
print_string_pm_finished:
  popa
  ret
