#include "../drivers/screen.h"
#include "isrs.h"
#include "idt.h"
#include "irq.h"
#include "../drivers/timer.h"
#include "keyboard.h"
#include "../memory/header.h"
#include "../drivers/hd.h"
#include "../drivers/fs.h"

void main () {
	init_pages(pages_size);
	screen_install();
	// clear_screen();
	

	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	timer_install();
	
	//timer_wait(5);
	
	verify_DPT();
	verify_fs();
	verify_dir();
	shell_install();
	keyboard_install();
	// clear_screen();
	// deleteFile("/worldFile");
	// char* a = malloc(300);
	// char* str = "Hello World!";
	// memory_copy(str, a, strlen(str) + 1);
	// print(a);
	// free(a);

	// print("\n--------------------------------------------\n");
	// struct INODE_NUM file = makeFile("worldFile", "/");
	// struct INODE_NUM folder = makeFolder("worldDir", "/");
	// struct INODE_NUM nestedFile = makeFile("nested", "/worldDir");
	// struct INODE_NUM nestedFile2 = makeFile("nested2", "/worldDir");
	// struct INODE_NUM worldDir = findFile("/worldDir");
	// stat(&worldDir.inode);
	
	// deleteFile("/worldDir");
	// struct INODE_NUM root = findFile("/worldDir");
	// stat(&root.inode);
	// struct INODE_NUM worldDir = findFile("/worldDir/nested");
	// stat(&worldDir.inode);
	// char* str = "Hello World!\n";
	// writeToFile("/worldDir/nested", str, strlen(str));
	// char* str2 = "Hello World2!\n";
	// writeToFile("/worldFile", str2, strlen(str2));
	// char* tmp = readFromFile("/worldDir/nested");
	
	struct INODE_NUM root = findFile("/");
	stat(&root.inode);
	// stat(&folder.inode);
	//stat(findFile("/helloWorld.txt"));
	// print("\n--------------------------------------------\n");

	// print(tmp);
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
	printCurrentDir();
	for(;;);
}