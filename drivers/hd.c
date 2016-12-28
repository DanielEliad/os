#include "hd.h"

CHS LBA2CHS(unsigned int LBA) {
	unsigned int cyl = lba/(HD0.head * HD0.sect);
    unsigned int head = (lba%(HD0.head*HD0.sect))/HD0.sect;
    unsigned int sect = (lba%(HD0.head*HD0.sect))%HD0.sect+1;

	CHS chs = {
		cyl,
		head,
		sect
	}

	return chs;
}


void HW_RW(unsigned int LBA, unsigned int command, unsigned int sects_to_access, void* buf) {
	
	CHS chs = LBA2CHS(LBA);


	//Wait for the hard driver state till it's ready
	
	//Structure of the `Status Register Output`
	// Bit 0	previous command ended in an error
	// Bit 1	index - set to 1 each revolution
	// Bit 2	disk data read corrected
	// Bit 3	sector buffer requires servicing
	// Bit 4	seek completed
	// Bit 5	write error
	// Bit 6	drive is ready
	// Bit 7	controller is busy

	//0xc0 = 1100 0000 -> only looks at the `drive is ready` and `controller is busy` flags
	//0x40 = 0100 0000 -> desired state, only the `drive is ready` flag is lit up
	while ((port_byte_in(HD_PORT_STATUS)&0xc0)!=0x40); 
	
	//The drive is ready!

	port_byte_out(sects_to_access, HD_PORT_SECT_COUNT);
	port_byte_out(chs.sect, HD_PORT_SECT_NUM);
	port_byte_out(chs.cyl, HD_PORT_CYL_LOW);
	port_byte_out(chs.cyl >> 8, HD_PORT_CYL_HIGH);

	//Structure of the `HD_PORT_DRV_HEAD` input
	// Bits 7-5	must be 101b
	// Bit 4	HD0(=0), HD1(=1)
	// Bits 3-0	head number

	//0xa0 = 1010 0000 -> light up the `101` flags at the end of the byte
	port_byte_out(0xa0 | chs.head, HD_PORT_DRV_HEAD);


	//Send the command to the command port
	port_byte_out(command, HD_PORT_COMMAND);


	while (! (inb(HD_PORT_STATUS)&0x8)); //waits for ?
	if (command == HD_READ) {
		insl(HD_PORT_DATA, buf, sects_to_access<<7);
	} else if(command == HD_WRITE) {
		outsl(buf, sects_to_access<<7, HD_PORT_DATA);
	}




}