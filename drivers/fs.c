#include "fs.h"


void setb(void *s, unsigned int i) {
	unsigned char *v = s;
    v += i>>3; // adds the offset divided by 8 (offset in bytes instead of bits)

    // i%8 refers to which bit in the byte specified by v it refers to
    // 7 - (i&8) iis because it is a little endian architecture
    *v |= 1<<(7-(i%8)); // lights the specified bit up
}


void clrb(void *s, unsigned int i) {
    unsigned char *v = s;
    v += i>>3; // adds the offset divided by 8 (offset in bytes instead of bits)

    // i%8 refers to which bit in the byte specified by v it refers to
    // 7 - (i&8) iis because it is a little endian architecture
    *v &= ~(1<<(7-(i%8))); // turns the specified bit off
}

int testb(void *s, unsigned int i) {
    unsigned char *v = s;
    v += i>>3; // adds the offset divided by 8 (offset in bytes instead of bits)

    // i%8 refers to which bit in the byte specified by v it refers to
    // 7 - (i&8) iis because it is a little endian architecture
    return (*v&(1<<(7-(i%8)))) !=0; // returns if the specified bit is on or off
}



void verify_fs(void) {
	
	extern unsigned int* hd0;

	unsigned int *q = hd0;


	unsigned char sect[512];
    struct SUPER_BLOCK sb;

	unsigned int i = 0, j = 0, m = 0, n = 0;

	/* get the fs sb */
    sb.sb_start = q[0];
	HD_RW(ABS_SUPER_BLK(sb), HD_READ, 1, &sb);

	/* the first partition must be FST_FS type */
    if (sb.sb_magic != FST_FS) {
    	print("Partition 1 does not have a valid file system\n");
    	print("Creating file system\n");
    	sb.sb_magic = FST_FS;
    	sb.sb_start = q[0];
		sb.sb_blocks = q[1];
		sb.sb_dmap_blks = (sb.sb_blocks+0xfff)>>12;
		sb.sb_imap_blks = INODE_BIT_BLKS;
		sb.sb_inode_blks = INODE_BLKS;
		HD_RW(ABS_SUPER_BLK(sb), HD_WRITE, 1, &sb);


		/* initial data bitmap */
		n = ABS_DMAP_BLK(sb);
		j = sb.sb_dmap_blks+sb.sb_imap_blks+sb.sb_inode_blks+2;
		memory_set(sect, 0xff, sizeof sect/sizeof sect[0]);
		for (i=j/(512*8); i>0; --i) {
			HD_RW(n++, HD_WRITE, 1, sect);
			m += 4096;
		}

		m += 4096;
		for (i=j%(512*8); i<512*8; ++i) {
			clrb(sect, i);
			--m;
		}
		HD_RW(n++, HD_WRITE, 1, sect);

		memory_set(sect, 0, sizeof sect/sizeof sect[0]);
		for (i=sb.sb_imap_blks-(n-ABS_DMAP_BLK(sb)); i>0; --i)
			HD_RW(n++, HD_WRITE, 1, sect);


		/* initail inode bitmap */
		for (i=sb.sb_imap_blks; i>0; --i)
			HD_RW(ABS_IMAP_BLK(sb)+i-1, HD_WRITE, 1, sect);

		/* initial inode blocks */
		for (i=sb.sb_inode_blks; i>0; --i)
			HD_RW(ABS_INODE_BLK(sb)+i-1, HD_WRITE, 1, sect);
		print("DONE\n");

    }

      

    char tmp[200];

    print("0: Type: FST_FS ");

    print("start at: ");
    itoa(sb.sb_start, tmp);
    print(tmp);
    print(", ");

    print("blocks: ");
    itoa(sb.sb_blocks, tmp);
    print(tmp);
    print(", ");

    print("dmap: ");
    itoa(sb.sb_dmap_blks, tmp);
    print(tmp);
    print(", ");

    print("imap: ");
    itoa(sb.sb_imap_blks, tmp);
    print(tmp);
    print(", ");

	print("inodes: ");
    itoa(sb.sb_inode_blks, tmp);
    print(tmp);


    print("\n");







    /* initialize swap partition */
    q += 2;

	sb.sb_start = q[0];
	HD_RW(ABS_SUPER_BLK(sb), HD_READ, 1, &sb);
	if (sb.sb_magic != FST_SW) {
		print("Partition 2 does not have a valid file system\n");
		print("Creating file system\n");
		sb.sb_magic = FST_SW;
		sb.sb_start = q[0];
		sb.sb_blocks = q[1];
		sb.sb_dmap_blks = (sb.sb_blocks)>>15;	/* 1 bits == 4K page */
		HD_RW(ABS_SUPER_BLK(sb), HD_WRITE, 1, &sb);
		print("DONE\n")	;
	}

	/* initial data bitmap */
	n = ABS_DMAP_BLK(sb);
	j = sb.sb_dmap_blks+2;
	memory_set(sect, 0xff, sizeof sect/sizeof sect[0]);
	for (i=j/(512*8); i>0; --i) {
		HD_RW(n++, HD_WRITE, 1, sect);
		m += 4096;
	}
	m += 4096;
	for (i=j%(512*8); i<512*8; ++i) {
		clrb(sect, i);
		--m;
	}
	HD_RW(n++, HD_WRITE, 1, sect);


	print("1: Type: FST_SW ");

    print("start at: ");
    itoa(sb.sb_start, tmp);
    print(tmp);
    print(", ");

    print("blocks: ");
    itoa(sb.sb_blocks, tmp);
    print(tmp);
    print(", ");

    print("dmap: ");
    itoa(sb.sb_dmap_blks, tmp);
    print(tmp);
    print(", ");

    print("presents: ");
    itoa(sb.sb_blocks>>3, tmp);
    print(tmp);
    print(" 4k-pages");

    print("\n");
}






