#include "header.h"


void init_pages(int size) {
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


	Block blocks[len_curve]; // = len_curve * sizeof(Header*)
	int offset_free_space_after_blocks = sizeof(blocks);
	int amounts = 0;
	for (int i = 0; i < len_curve; i++) {
		float data_size = 1 << (i+5); // 2^(i+5)
		float frames = (size/data_size);
		int amount = curve[i]*frames;
		amounts += amount;
	}
	
	int current = sizeof(blocks) + amounts*sizeof(Header);//skip all the headers
	for (int i = 0; i < len_curve; ++i) {
		float data_size = 1 << (i+5); // 2^(i+5)
		float frames = (size/data_size);
		int amount = curve[i]*frames;

		
		Header headers[amount];

		for(int j = 0; j < amount; j++) {
			Header pageStart;
			pageStart.addr = base+current;
			pageStart.used = 0;
			pageStart.data_size = data_size;
			headers[j] = pageStart;
			current += data_size;
		}

		

		char* new_current_block_address = base + offset_free_space_after_blocks;
		memory_copy(&headers, new_current_block_address, sizeof(headers));
		
		Block current_block;
		current_block.headers = new_current_block_address;
		current_block.len = amount;
		
		offset_free_space_after_blocks += sizeof(headers);		
		
		blocks[i] = current_block;
	}

	memory_copy(&blocks, base, sizeof(blocks));
}

char* malloc(int n_bytes) {
	Block* blocks = (Block* *) base;
	char block = find_category(n_bytes);
	if(block == -1) {
		//error handling
		print("CATEGORY NOT FOUND\n");
		return 0;
	}

	Block correct_block = blocks[block];
	// print("GOT HERE\n");
	// char str[20];
	// itoa(correct_block.len, str);
	// print(str);
	for (int i = 0; i < correct_block.len; ++i)	{

		Header h = correct_block.headers[i];
		if(h.used == 0) {	//if h is not being used at the moment
			h.used = 1;	//mark as used
			return h.addr;	//return the space allocated for your use.
		}
	}

	print("Something went wrong returned null\n");
	return 0;
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
