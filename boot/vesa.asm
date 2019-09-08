; Handle interactions with VESA BIOS Extensions (VBE) which must be done
; in real mode (before we switch control to the kernel). This code will be
; used by the second stage of our boot loader to set the VBE mode.
; Any information about VBE (eg video memory address) needed by the kenrel
; will be passed to it by the boot loader.

[bits 16]

VBE_INFO_CODE      equ 0x4f00
VBE_MODE_INFO_CODE equ 0x4f01
VBE_SET_MODE_CODE  equ 0x4f02
VBE_ERROR_CODE     equ 0x004f

vesa_init:

  pusha

  ; Call function to get VBE information
  mov ax, VBE_INFO_CODE
  mov di, vbe_info_structure
  int 0x10
  cmp ax, VBE_ERROR_CODE
  jne vbe_error

  ; Expect "VESA" in the signature

  cmp dword [vbe_info_structure.signature], 'VESA'
  jne vbe_error

  mov bx, [vbe_info_structure.version]
  call print_hex
  call print_newline

  mov cx, [vbe_info_structure.vendor]
  mov bx, [ecx]
  call print_hex
  call print_newline

  mov cx, [vbe_info_structure.product_name]
  mov bx, [ecx]
  call print_hex
  call print_newline

  mov bx, [vbe_info_structure.video_memory]
  call print_hex
  call print_newline

  call print_newline

  ; Loop over array of modes and get information for each
  mov edx, [vbe_info_structure.video_modes]
  loop_modes:
  mov bx, [edx]
  cmp bx, 0xffff ; end of list indicator
  je end_loop_modes
  push edx

  ; Call VBE function to get mode information
  mov ax, VBE_MODE_INFO_CODE
  mov cx, bx
  mov di, vbe_mode_structure
  int 0x10
  cmp ax, VBE_ERROR_CODE
  jne vbe_error

  ; Filter out modes without line frame buffer
  mov dx, [vbe_mode_structure.attributes]
  and dx, 0x80 ; get bit 7
  cmp dx, 0
  je next_mode

  ; Filter out widths != 1024
  mov dx, [vbe_mode_structure.width]
  cmp dx, 1024
  jne next_mode

  ; Print the mode number, width, height and bits per pixel
  mov bx, cx
  call print_hex

  mov ah, 0x0e
  mov al, ' '
  int 0x10

  mov bx, dx
  call print_decimal

  mov ah, 0x0e
  mov al, 'x'
  int 0x10

  mov bx, [vbe_mode_structure.height]
  call print_decimal

  mov ah, 0x0e
  mov al, 'x'
  int 0x10

  mov bx, [vbe_mode_structure.bpp]
  mov bh, 0 ; only want the lower 8 bits
  call print_decimal

  call print_newline

  next_mode:
  pop edx
  add edx, 2
  jmp loop_modes

  end_loop_modes:
  call print_newline

  ; Assume we want mode 0x0144 (1024 x 768 x 32bpp)
  ; TODO Don't assume this, need to hanlde it better. Maybe we pass the desired
  ;      size to vbe_init and if it exists, use it below.

  ; Call VBE function to get mode information
  mov ax, VBE_MODE_INFO_CODE
  mov cx, 0x0144
  mov di, vbe_mode_structure
  int 0x10
  cmp ax, VBE_ERROR_CODE
  jne vbe_error

  mov bx, [vbe_mode_structure.memory_model]
  mov bh, 0 ; only want the lower 8 bits
  call print_hex
  call print_newline

  mov ebx, [vbe_mode_structure.framebuffer]
  push ebx
  shr ebx, 16
  call print_hex
  call print_newline
  pop ebx
  and ebx, 0xffff
  call print_hex
  call print_newline

  mov bx, [vbe_mode_structure.red_mask_num_bits]
  mov bh, 0 ; only want the lower 8 bits
  call print_hex
  call print_newline
  mov bx, [vbe_mode_structure.red_mask_start_bit]
  mov bh, 0 ; only want the lower 8 bits
  call print_hex
  call print_newline
  mov bx, [vbe_mode_structure.green_mask_num_bits]
  mov bh, 0 ; only want the lower 8 bits
  call print_hex
  call print_newline
  mov bx, [vbe_mode_structure.green_mask_start_bit]
  mov bh, 0 ; only want the lower 8 bits
  call print_hex
  call print_newline
  mov bx, [vbe_mode_structure.blue_mask_num_bits]
  mov bh, 0 ; only want the lower 8 bits
  call print_hex
  call print_newline
  mov bx, [vbe_mode_structure.blue_mask_start_bit]
  mov bh, 0 ; only want the lower 8 bits
  call print_hex
  call print_newline
  mov bx, [vbe_mode_structure.pitch]
  call print_hex
  call print_newline

  ; Call VBE function to set the mode
  mov ax, VBE_SET_MODE_CODE ; set vbe mode function
  mov bx, 0x0144 ; desired mode
  or bx, 0x4000  ; set bit 14 for linearframebuffer
  int 0x10
  cmp ax, VBE_ERROR_CODE
  jne vbe_error

  end:
  popa
  ret

vbe_error:
  mov bx, VBE_ERROR_MSG
  call print_string
  call print_newline
  jmp end

VBE_ERROR_MSG db "VBE function error", 0

vbe_info_structure:
  .signature               db "VBE2"  ; Indicate VBE 2.0+
  .version                 dw 0
  .oem                     dd 0
  .capabilities            dd 0
  .video_modes             dd 0
  .video_memory            dw 0
  .software_rev            dw 0
  .vendor                  dd 0
  .product_name            dd 0
  .product_rev             dd 0
  .reserved     times(222) db 0
  .oem_data     times(512) db 0

vbe_mode_structure:
  .attributes               dw 0
  .window_a                 db 0
  .window_b                 db 0
  .granularity              dw 0
  .window_size              dw 0
  .segment_a                dw 0
  .segment_b                dw 0
  .win_func_ptr             dd 0
  .pitch                    dw 0
  .width                    dw 0
  .height                   dw 0
  .w_char                   db 0
  .y_char                   db 0
  .planes                   db 0
  .bpp                      db 0
  .banks                    db 0
  .memory_model             db 0
  .bank_size                db 0
  .image_pages              db 0
  .reserved0                db 0
  .red_mask_num_bits        db 0
  .red_mask_start_bit       db 0
  .green_mask_num_bits      db 0
  .green_mask_start_bit     db 0
  .blue_mask_num_bits       db 0
  .blue_mask_start_bit      db 0
  .reserved_mask            db 0
  .reserved_position        db 0
  .direct_colour_attributes db 0
  .framebuffer              dd 0
  .off_screen_mem_off       dd 0
  .off_screen_mem_size      dw 0
  .reserved times(206)      db 0
