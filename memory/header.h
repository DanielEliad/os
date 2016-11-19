#ifndef HEADER_H
#define HEADER_H
#include "../kernel/utils.h"

#define base 0x40000000
#define len_curve 16

typedef struct {
	char used;
	unsigned int data_size;
	unsigned int addr;
} Header;

typedef struct {
	Header* headers;
	int len;
} Block;

void init_pages(int size);
char* malloc(int n_bytes);
char find_category(int n);


#endif
