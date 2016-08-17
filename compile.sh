nasm -f bin boot.asm
nasm kernel_entry.asm -f elf -o kernel_entry.o
gcc -m32 -ffreestanding -c kernel/kernel.c -o kernel/kernel.o
ld -m elf_i386 -o kernel/kernel.bin -Ttext 0x1000 kernel_entry.o kernel/kernel.o --oformat binary
cat boot kernel/kernel.bin > os_image
qemu-system-i386 -fda os_image