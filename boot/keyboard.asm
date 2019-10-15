; Keyboard functions

%ifndef KEYBOARD_ASM
%define KEYBOARD_ASM

[bits 16]

ENTER_KEY equ 13

; ASCII value of key pressed returned in AL
keyboard_wait_for_key_press:
  xor ax, ax
  int 0x16
  ret

keyboard_wait_for_enter_key:
  get_next_key_press:
  xor ax, ax
  int 0x16
  cmp al, ENTER_KEY
  jne get_next_key_press
  ret

keyboard_get_number:
  push bx
  push dx
  push cx

  mov bx, 0
  get_next_number:
  call keyboard_wait_for_key_press
  cmp al, ENTER_KEY
  je get_number_enter_key
  cmp al, '0'
  jl get_next_number
  cmp al, '9'
  jg get_next_number
  sub al, '0'
  mov ah, 0
  push ax
  add bx, 1
  push bx
  mov bx, ax
  call print_decimal
  pop bx
  jmp get_next_number

  get_number_enter_key:
  cmp bx, 0
  je get_next_number
  call print_newline

  mov dx, 0
  mov cx, 1
  input_to_number:
  pop ax
  push dx
  mul cx
  pop dx
  add dx, ax
  mov ax, cx
  mov cx, 10
  push dx
  mul cx
  pop dx
  mov cx, ax
  sub bx, 1
  cmp bx, 0
  jne input_to_number  

  finish_get_number:
  mov ax, dx
  pop cx
  pop dx
  pop bx
  ret

%endif ; KEYBOARD_ASM
