#include "utils.h"

void memory_copy(char* source, char* dest, int n_bytes) {
	for(int i = 0; i < n_bytes; i++) {
		*(dest + i) = *(source + i);
	}
}
