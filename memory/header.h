#ifndef HEADER_H
#define HEADER_H

typedef struct {
	char used;
	unsigned int data_size;
} Header;

void init_pages(char* base, int size);


#endif
