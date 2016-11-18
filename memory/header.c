#include "header.h"


void init_pages(char* base, int size) {
	float curve[] = {
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

	int len_curve = (int)sizeof(curve)/sizeof(curve[0]);
	int amounts = 0;
	for (int i = 0; i < len_curve; ++i)
	{
		float data_size = 1 << (i+5); // 2^(i+5)
		float frames = (size/data_size);
		int amount = curve[i]*frames;
		amounts += amount;
	}

	Header* blocks[len_curve][amounts];
	int current = 0;//skip all the headers
	for (int i = 0; i < len_curve; ++i) {
		float data_size = 1 << (i+5); // 2^(i+5)
		float frames = (size/data_size);
		int amount = curve[i]*frames;
		Header* current_block[amount];

		for(int j = 0; j < amount; j++) {
			Header* pageStart = base+current;
			(*pageStart).used = 0;
			(*pageStart).data_size = data_size - sizeof(Header);
			current_block[j] = &pageStart;
			current += data_size;
		}

		blocks[i] = current_block;
	}

}
