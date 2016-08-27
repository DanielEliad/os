# Automatically generate lists of sources using wildcards .
CPP_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
#$(info  ${CPP_SOURCES})
# TODO : Make sources dep on all header files .
# Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${CPP_SOURCES:.c=.o}
# Defaul build target
all: os_image

run: all
	qemu-system-i386 -fda os_image

os_image: kernel.bin #boot/boot_sect.bin
	cat $^ > os_image

kernel.bin: boot/boot_sect.o kernel_entry.o ${OBJ} 
#	nasm -f aout -I boot/ -o start.o boot/boot_sect.asm
	ld -m32 -m elf_i386 -o $@ -Ttext 0x1000 $^   --oformat binary

%.o: %.c ${HEADERS}
	g++ -std=c11 -m32 -ffreestanding -c $< -o $@ -march=i386

%.o: %.asm
	nasm $< -f elf32 -I boot/ -o $@

%.bin: %.asm
	nasm $< -f bin -I boot/ -o $@

clean:
	rm -rf *.bin *.dis *.o os_image
	rm -rf kernel/*.o boot/*.bin drivers/*.o
