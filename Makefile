# Makefile for kernel

# Useful make variables:
#
# $^ = target dependencies
# $< = first dependency
# $@ = target

CPP_SOURCE = $(wildcard kernel/*.cpp drivers/*.cpp)
CPP_HEADERS = $(wildcard kernel/*.hpp drivers/*.hpp)

ASM_SOURCE = $(wildcard kernel/*.asm)

# Build list of object files from C++ source files by replacing .cpp with .o
OBJ_FILES = ${CPP_SOURCE:.cpp=.o} ${ASM_SOURCE:.asm=.o}

BOOT_ASM_SOURCE = $(wildcard boot/*.asm)

# Memory address where boot stage 1 execution starts
BOOT1_START = 0x1000

# Memory address where kernel execution starts
KERNEL_START = 0x4000

all: os-image

run: os-image
	qemu-system-x86_64 -smp cpus=2 -drive format=raw,file=$<

os-image: boot/boot0.bin boot/boot1.bin kernel/kernel.bin
	sh load_sectors_check.sh
	cat $^ > $@

boot/boot0.bin: boot/boot0.asm ${BOOT_ASM_SOURCE}
	nasm $< -f bin -o $@ -I ./boot/

boot/boot1.o: boot/boot1.asm ${BOOT_ASM_SOURCE}
	nasm $< -f elf -o $@ -I ./boot/

boot/boot1.bin: boot/boot1.o
	ld -o $@ -Ttext ${BOOT1_START} --entry ${BOOT1_START} --oformat binary -m elf_i386 $^

kernel/kernel.bin: kernel/kernel_entry.o ${OBJ_FILES}
	ld -o $@ -Ttext ${KERNEL_START} --entry ${KERNEL_START} --oformat binary -m elf_i386 $^

%.o: %.cpp ${CPP_HEADERS}
	g++ -Wall -Werror -Wfatal-errors -m32 -fno-pic -ffreestanding -c $< -o $@ -I .

%.o: %.asm
	nasm $< -f elf -o $@

clean:
	find . -name "*.o" -o -name "*.bin" | xargs rm -f
	rm -f os-image
