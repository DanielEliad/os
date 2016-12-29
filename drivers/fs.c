#include "fs.h"

struct INODE iroot = {FT_DIR, 2*sizeof(struct DIR_ENTRY), {0,}};

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


unsigned int alloc_blk(struct SUPER_BLOCK *sb) {
	unsigned int i = 0, j = 0, n = 0, m = 0;
	unsigned char sect[512] = {0};
	n = ABS_DMAP_BLK(*sb);

	for (; i<sb->sb_dmap_blks; ++i) {
		HD_RW(n, HD_READ, 1, sect);
        for (j=0; j<512*8; ++j) {
            
            if (testb(sect, j)) {
                ++m;
            } else {            /* gotcha */
                setb(sect, j);
                if (m >= sb->sb_blocks)
                    return 0;
                else {
                    HD_RW(n, HD_WRITE, 1, sect);
                    return ABS_BOOT_BLK(*sb) + m;
                }
            }

        }

        ++n;
	}

	return 0;
}

void free_blk(struct SUPER_BLOCK *sb, unsigned int n) {
	unsigned char sect[512] = {0};
    unsigned int t = (n-ABS_BOOT_BLK(*sb))/(512*8)+ABS_DMAP_BLK(*sb);
    HD_RW(t, HD_READ, 1, sect);
    clrb(sect, (n-ABS_BOOT_BLK(*sb))%(512*8));
    HD_RW(t, HD_WRITE, 1, sect);
}

int alloc_inode(struct SUPER_BLOCK *sb) {
	unsigned char sect[512] = {0};
    int i = 0;
    HD_RW(ABS_IMAP_BLK(*sb), HD_READ, 1, sect);
    for (; i<512; ++i) {
        if (! testb(sect, i)) {
            setb(sect, i);
            HD_RW(ABS_IMAP_BLK(*sb), HD_WRITE, 1, sect);
            break;
        }
    }
    return (i==512)?-1:i;
}

void free_inode(struct SUPER_BLOCK *sb, int n) {
	unsigned char sect[512] = {0};
	HD_RW(ABS_IMAP_BLK(*sb), HD_READ, 1, sect);
	clrb(sect, n);
	HD_RW(ABS_IMAP_BLK(*sb), HD_WRITE, 1, sect);
}


struct INODE *
iget(struct SUPER_BLOCK *sb, struct INODE *inode, int n) {
    unsigned char sect[512] = {0};
    int i = n/INODES_PER_BLK;
    int j = n%INODES_PER_BLK;
 
    HD_RW(ABS_INODE_BLK(*sb)+i, HD_READ, 1, sect);
    memory_copy(sect+j*sizeof(struct INODE), inode, sizeof(struct INODE));

    return inode;
}

void iput(struct SUPER_BLOCK *sb, struct INODE *inode, int n) {
    unsigned char sect[512] = {0};
    int i = n/INODES_PER_BLK;
    int j = n%INODES_PER_BLK;
    HD_RW(ABS_INODE_BLK(*sb)+i, HD_READ, 1, sect);
    memory_copy(inode, sect+j*sizeof(struct INODE), sizeof(struct INODE));
    HD_RW(ABS_INODE_BLK(*sb)+i, HD_WRITE, 1, sect);
}


void check_root(void) {
	extern unsigned int* hd0;

	struct SUPER_BLOCK sb;
    unsigned char sect[512] = {0};
    struct DIR_ENTRY *de = 0;
 
    sb.sb_start = hd0;
    HD_RW(ABS_SUPER_BLK(sb), HD_READ, 1, sect);
    memory_copy(sect, &sb, sizeof(struct SUPER_BLOCK));
    HD_RW(ABS_IMAP_BLK(sb), HD_READ, 1, sect);
    if (! testb(sect, 0)) {
        print("/ has not been created, creating....");
        if (alloc_inode(&sb) != 0) {
            print("\n/ must be inode 0!!!\n");
            for(;;); // halt();
        }

    	iroot.i_block[0] = alloc_blk(&sb);
        iput(&sb, &iroot, 0);
        de = (struct DIR_ENTRY *)sect;
        strcopy(".", de->de_name);
        de->de_inode = 0;
        ++de;
        strcopy("..", de->de_name);
        de->de_inode = -1;
        HD_RW(iroot.i_block[0], HD_WRITE, 1, sect);

        print("DONE\n");
	}

	iget(&sb, &iroot, 0);
	HD_RW(iroot.i_block[0], HD_READ, 1, sect);
    de = (struct DIR_ENTRY *)sect;
    if ((strcmp(de[0].de_name, ".")) || (de[0].de_inode) ||
        (strcmp(de[1].de_name, "..")) || (de[1].de_inode) != -1) {
    	print("File system is corrupted!!!\n");
    	for(;;); // halt();
    }

}


void stat(struct INODE *inode) {
	unsigned int i = 0;
    char sect[512] = {0};
    struct DIR_ENTRY *de;
    print("======== stat / ========\n");
    switch (inode->i_mode) {
    case FT_NML:
        print("File, ");
        break;
    case FT_DIR:
        print("Dir,  ");
        break;
    default:
        print("UNKNOWN FILE TYPE!!");
        for(;;); // halt();
    }

	print("Size: ");
	char size[get_num_digits(inode->i_size) + 1];
	itoa(inode->i_size, size);
	print(size);
	print(", ");

	print("Blocks: ");
	for (; i<8; ++i) {
		char block[get_num_digits(inode->i_block[i]) + 1];
		itoa(inode->i_block[i], block);
		print(block);
	}

	HD_RW(inode->i_block[0], HD_READ, 1, sect);
	switch (inode->i_mode) {
		case FT_DIR:
			print("\nName    INode\n");
			de = (struct DIR_ENTRY *)sect;
			for (i=0; i< inode->i_size/sizeof(struct DIR_ENTRY); ++i) {
				print(de[i].de_name);
				print("    ");
				char inode[get_num_digits(de[i].de_inode) + 1];
				itoa(de[i].de_inode, inode);
				print(inode);
				print("\n");
        	}
        	break;
    	default:
    		break;
	}

}



void verify_dir(void) {

	extern unsigned int* hd0;

	unsigned char sect[512] = {0};
	unsigned int *q = hd0;

	struct INODE inode;
    struct SUPER_BLOCK sb;

    sb.sb_start = q[0];

    HD_RW(ABS_SUPER_BLK(sb), HD_READ, 1, sect);
    check_root();

    memory_copy(sect, &sb, sizeof(struct SUPER_BLOCK));
    stat(iget(&sb, &inode, 0));
}






