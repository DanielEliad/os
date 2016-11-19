#include "../drivers/screen.h"
#include "isrs.h"
#include "idt.h"
#include "irq.h"
#include "timer.h"
#include "keyboard.h"
#include "../memory/header.h"

void main () {
	clear_screen();
	idt_install();
	isrs_install();
	irq_install();
	__asm__ __volatile__ ("sti");
	timer_install();
	keyboard_install();
	//timer_wait(5);
	init_pages(0xe0000);
	char* str = malloc(6);
	str[0] = 'H';
	str[1] = 'E';
	str[2] = 'L';
	str[3] = 'L';
	str[4] = 'O';
	str[5] = '\0';
	print(str);
	print("\n");
	char* buff = malloc(300);
	itoa(0xe0000, buff);
	print(buff);
	print("I am the Tal OS...\nT.A.L.O.S\n    Tal\n    Awesome\n    Lovely\n    Overwatch\n    System");
	for(;;);
}