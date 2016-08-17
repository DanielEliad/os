# Automatically generate lists of sources using wildcards .
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
# TODO : Make sources dep on all header files .
# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o}
# Defaul build target
all: os_image

run: all
	qemu-system-i386 -fda os_image

os_image: boot/boot_sect.bin kernel.bin
	cat $^ > os_image

kernel.bin: kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

%.o : %.c ${HEADERS}
	gcc -m32 -std=c99  -ffreestanding -c $< -o $@

%.o : %.asm
	nasm $< -f elf32 -o $@
%.bin : %.asm
	nasm $< -f bin -I boot/ -o $@

clean:
	rm -rf *.bin *.dis *.o os_image
	rm -rf kernel/*.o boot/*.bin drivers/*.o