#ifndef HEADER_H
#define HEADER_H
#include "../kernel/utils.h"

#define base 0x100000
#define len_curve 16

typedef struct {
	char used;
	unsigned int data_size;
	unsigned int addr;
} __attribute__((packed)) Header;

typedef struct {
	Header* headers;
	int len;
}__attribute__((packed)) Block;

void init_pages(unsigned int size);
char* malloc(unsigned int n_bytes);
char find_category(int n);


#endif
