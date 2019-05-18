[bits 16]

; Switch to protected mode

switch_to_pm:

  ; Turn off interrupts
  cli

  ; Load global descriptor table which defines the protected mode
  ; segments eg for code and data
  lgdt [gdt_descriptor]

  ; To make the swtich to protected mode, set first bit of special register cr0
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax

  ; Make far jump (ie to a new segment) to our 32-bit code. This also forces
  ; the CPU to flush its cache of pre-fetched and real-mode decoded
  ; instructions, which can cause problems.
  jmp CODE_SEG:init_pm

[bits 32]

; Initilize registers and stack once in protected mode

init_pm:

  ; In protected mode the old segments are meaningless so point the
  ; segment registers to the data selector defined in our GDT.
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; Update the stack position
  mov ebp, 0x9000
  mov esp, ebp

  ; Call some defined entry point function
  call BEGIN_PM
