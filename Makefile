# Makefile for kernel

# Useful make variables:
#
# $^ = target dependencies
# $< = first dependency
# $@ = target

C_SOURCE = $(wildcard kernel/*.c drivers/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h)

# Build list of object files from C source files by replacing .c with .o
OBJ_FILES = ${C_SOURCE:.c=.o}

all: os-image

run: os-image
	qemu-system-i386 -drive format=raw,file=$<

os-image: boot/boot_kernel.bin kernel/kernel.bin
	cat $^ > $@

kernel/kernel_entry.o: kernel/kernel_entry.asm
	nasm $< -f elf -o $@

kernel/kernel.bin: kernel/kernel_entry.o ${OBJ_FILES}
	ld -o $@ -Ttext 0x1000 $^ --oformat binary -m elf_i386

%.o: %.c ${C_HEADERS}
	gcc -m32 -fno-pic -ffreestanding -c $< -o $@ -I .

%.bin: %.asm
	nasm $< -f bin -o $@ -I "./boot/"

clean:
	find . -name "*.o" -o -name "*.bin" | xargs rm -f
	rm -f os-image
