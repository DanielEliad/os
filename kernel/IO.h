#ifndef IO_H
#define IO_H


unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
unsigned short port_word_in(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);
void port_sl_in(unsigned short port, void* buf, unsigned int nr);
void port_sl_out(unsigned short port, void* buf, unsigned int nr);



#endif