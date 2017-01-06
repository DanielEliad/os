#include "header.h"


void write_header(Header h, unsigned int addr) {
	memory_copy((char*)&h, (char*)addr, (int) sizeof(Header));
}

void write_block(Block b, unsigned int addr) {
	memory_copy((char*)&b, (char*)addr, (int) sizeof(Block));	
}

void init_pages(unsigned int size) {
	float curve[len_curve] = {
		0.0019, // 2^5 = 32
		0.0056, // 2^6 = 64
		0.0146, // 2^7 = 128
		0.032 , // 2^8 = 256
		0.0603, // 2^9 = 512
		0.0968, // 2^10 = 1024
		0.1327, // 2^11 = 2048
		0.1554, // 2^12 = 4096
		0.1554, // 2^13 = 8192
		0.1327, // 2^14 = 16384
		0.0968, // 2^15 = 32768
		0.0603, // 2^16 = 65536
		0.032,  // 2^17 = 131072
		0.0146, // 2^18 = 262144
		0.0056, // 2^19 = 524288
		0.0019, // 2^20 = 1048576
	};

	int amounts = 0;
	float smallest_data_size = 1 << (5); // 2^5
	for (int i = 0; i < len_curve; i++) {
		float data_size = 1 << (i+5); // 2^(i+5)
		float frames = (size/smallest_data_size);
		int amount = (curve[i]*frames)/data_size;
		amounts += amount;
	}
	unsigned int data_addr = base + len_curve*(sizeof(Block)) + amounts*(sizeof(Header));
	unsigned int header_addr = base + len_curve*(sizeof(Block));
	unsigned int block_addr = base;
	for (int i = 0; i < len_curve; ++i) {	//for each block
		float data_size = 1 << (i+5); // 2^(i+5)
		float frames = (size/smallest_data_size); //(size/data_size);
		int amount = (curve[i]*frames)/data_size;
		unsigned int first_header_of_block_addr = header_addr;
		for (int j = 0; j < amount; ++j) {
			Header pageStart;
			pageStart.addr = data_addr;
			pageStart.data_size = data_size;
			pageStart.used = 0;
			data_addr += data_size;
			write_header(pageStart, header_addr);
			header_addr += sizeof(Header);
		}

		Block b;
		b.headers = (Header*)first_header_of_block_addr;
		b.len = amount;
		// print("Set len of block to: ");
		// char str[200];
		// itoa(amount, str);
		// print(str);
		// print("\n");
		write_block(b, block_addr);
		block_addr += sizeof(Block);
	}
}

char* malloc(unsigned int n_bytes) {
	Block* blocks = (Block* ) base;
	char block = find_category(n_bytes);
	if(block == -1) {
		//error handling
		print("CATEGORY NOT FOUND\n");
		return 0;
	}
	Block correct_block = blocks[block];
	for (int i = 0; i < correct_block.len; ++i)	{
		Header h = correct_block.headers[i];
		if(h.used == 0) {	//if h is not being used at the moment
			h.used = 1;	//mark as used
			// print("\nALLOCATED HEADER WITH ADDR: ");
			// char tmp[20]; itoa(h.addr, tmp); print(tmp); print("\n");
			return h.addr;	//return the space allocated for your use.
		}
	}

	print("Something went wrong returned null\n");
	return 0;
}

void free(unsigned int addr) {
	Block* blocks = (Block* ) base;
	for(unsigned int i = 0; i < len_curve; i++) {
		Block tested_block = blocks[i];
		for(unsigned int j = 0; j < tested_block.len; j++) {
			Header h = tested_block.headers[j];
			if(h.addr == addr) {
				h.used = 0;
				// print("\nCLEARED HEADER WITH ADDR: ");
				// char tmp[20]; itoa(h.addr, tmp); print(tmp); print("\n");
				return;
			}
		}


	}

}

char find_category(int n) {
	//binary search
	//assumes sizes go from 2^5 up to and including 2^20
	//per data section
	int powers_of_two[16] = {
		1<<5,
		1<<6,
		1<<7,
		1<<8,
		1<<9,
		1<<10,
		1<<11,
		1<<12,
		1<<13,
		1<<14,
		1<<15,
		1<<16,
		1<<17,
		1<<18,
		1<<19,
		1<<20
	};

	// char first = 0;
	// char last = 16 - 1;
	// char middle = (first + last)/2;

	// while(first <= last) {
	// 	if(n < powers_of_two[middle] && powers_of_two[middle-1] <= n) {
	// 		return middle;
	// 	} else if(powers_of_two[middle] <= n) {
	// 		first = middle + 1;
	// 	} else {
	// 		last = middle - 1;
	// 	}

	// 	middle = (first + last)/2;
	// }


	for (int i = 0; i < 16; ++i) {
		if(n < powers_of_two[i]) return i;
	}

	return -1;
}
