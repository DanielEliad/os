#ifndef IO_H
#define IO_H


unsigned char port_byte_in(unsigned short port);
unsigned char port_byte_out(unsigned short port, unsigned char data);
unsigned short port_word_in(unsigned short port);
unsigned char port_word_out(unsigned short port, unsigned short data);

#endif