; Kernel entry point code
;
; This code should always be at the start of our kernel image.
; It ensures we always start executing kernel code at the right
; function regardless of where it is located in our kernel image.
;
[bits 32]
[extern main]
call main
jmp $
