; Kernel entry point code
;
; This code should always be at the start of our kernel image.
; It ensures we always start executing kernel code at the right
; function regardless of where it is located in our kernel image.
;
[bits 32]
[extern main]
pop dword [fonts]
pop dword [vbe_mode_info]
call main
jmp $
global vbe_mode_info
vbe_mode_info dd 0
global fonts
fonts dd 0
