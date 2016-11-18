# Automatically generate lists of sources using wildcards .
CPP_SOURCES = $(wildcard kernel/*.c drivers/*.c memory/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h memory/*.h)
#$(info  ${CPP_SOURCES})
# TODO : Make sources dep on all header files .
# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${CPP_SOURCES:.c=.o}
# Defaul build target
all: os_image

debug: all
	ld -m32 -m elf_i386 -i -o  $@-symbols ${OBJ} kernel_entry.o boot/idt.o boot/ISRs.o boot/IRQs.o
	qemu-system-i386 -s -S -fda os_image

run: all
	qemu-system-i386 -fda os_image

os_image: boot/boot_sect.bin kernel.bin
	cat $^ > os_image

kernel.bin: kernel_entry.o ${OBJ} boot/idt.o boot/ISRs.o boot/IRQs.o
	ld -m32 -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	gcc -fno-leading-underscore -std=c11 -m32 -ffreestanding -c $< -o $@ -march=i386 -g

%.o: %.asm
	nasm $< -f elf32 -I boot/ -o $@

%.bin: %.asm
	nasm $< -f bin -I boot/ -o $@

clean:
	rm -rf *.bin *.dis *.o os_image
	rm -rf kernel/*.o boot/*.bin boot/*.o drivers/*.o
