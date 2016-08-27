#include "../drivers/screen.h"
#include "isrs.h"
#include "idt.h"
void main () {
	idt_install();
	isrs_install();
	clear_screen();
	print("I am the Tal OS...\nT.A.L.O.S\n    Tal\n    Awesome\n    Lovely\n    Overwatch\n    System");
}