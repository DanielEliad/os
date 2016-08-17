MAKEFLAGS += --silent

# Default make target.
# Run qemu to simulate booting of our code.
all: build
	qemu-system-i386 -fda build/os_image

build: os_image
# Build the kernel binary

os_image: boot kernel.bin
	cd build && \
	cat $^ > $@ && \
	cd ..
boot: boot.asm build_dir
	nasm $< -f bin -I ’16bit/’ -o build/$@

kernel.bin: kernel_entry.o kernel.o
	cd build && \
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^  --oformat binary && \
	cd ..
# Build the kernel object file
kernel.o: kernel.c build_dir
	gcc -m32 -ffreestanding -c $< -o build/$@
# Build the kernel entry object file .
kernel_entry.o: kernel_entry.asm build_dir
	nasm $< -f elf -o build/$@
clean:
	rm -rf build/*
kernel.dis: kernel.bin
	ndisasm -b 32 build/$< > build/$@
build_dir:
	mkdir -p build
