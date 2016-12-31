#include "../drivers/screen.h"
#include "isrs.h"
#include "idt.h"
#include "irq.h"
#include "timer.h"
#include "keyboard.h"
#include "../memory/header.h"
#include "../drivers/hd.h"
#include "../drivers/fs.h"

void main () {
	clear_screen();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	timer_install();
	keyboard_install();
	//timer_wait(5);
	init_pages(1<<30);
	verify_DPT();
	verify_fs();
	verify_dir();
	print("\n--------------------------------------------\n");
	// struct INODE_NUM file = makeFile("worldFile", "/.");
	// struct INODE_NUM folder = makeFolder("worldDir", "/.");
	// struct INODE_NUM nestedFile = makeFile("nested", "/worldDir");
	// struct INODE_NUM worldDir = findFile("/worldDir");
	// stat(&worldDir.inode);
	// struct INODE_NUM worldDir2 = findFile("/worldDir/");
	// stat(&worldDir2.inode);
	struct INODE_NUM root = findFile("/");
	stat(&root.inode);
	// stat(&folder.inode);
	//stat(findFile("/helloWorld.txt"));
	print("\n--------------------------------------------");
	//setup_DPT();
	// char* str = malloc(6);
	// str[0] = 'H';
	// str[1] = 'E';
	// str[2] = 'L';
	// str[3] = 'L';
	// str[4] = 'O';
	// str[5] = '\0';
	// print(str);
	// print("\n");
	// char* buff = malloc(300);
	// itoa(0x40000000, buff);
	// print(buff);
	//print("I am the Tal OS...\nT.A.L.O.S\n    Tal\n    Awesome\n    Lovely\n    Overwatch\n    System");
	for(;;);
}