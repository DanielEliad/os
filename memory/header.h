#ifndef HEADER_H
#define HEADER_H
#include "../kernel/utils.h"

typedef struct {
	char used;
	unsigned int data_size;
	char* addr;
} Header;

void init_pages(char* base, int size);


#endif
