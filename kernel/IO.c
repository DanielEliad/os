#include "IO.h"

unsigned char port_byte_in(unsigned short port) {
	// A handy C wrapper function that reads a byte from the specified port
	// "=a" ( result ) means : put AL register in variable RESULT when finished
	// "d" ( port ) means : load EDX with port
	unsigned char result ;
	__asm__ ("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result ;
}

void port_byte_out(unsigned short port, unsigned char data) {
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
	unsigned char result ;
	__asm__ ("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result ;
}

void port_word_out(unsigned short port, unsigned short data) {
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

void port_sl_in(unsigned short port, void* buf, unsigned int nr) {
	__asm__ ("cld;rep;insl\n\t" :: "d"(port), "D"(buf), "c"(nr));
}

void port_sl_out(unsigned short port, void* buf, unsigned int nr) {
	__asm__ ("cld;rep;outsl\n\t" :: "d"(port), "S" (buf), "c" (nr));
}
