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
	init_pages(0x40000000,0xe0000);
	print("I am the Tal OS...\nT.A.L.O.S\n    Tal\n    Awesome\n    Lovely\n    Overwatch\n    System");
	for(;;);
}