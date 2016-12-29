#ifndef FS_H
#define FS_H

#include "hd.h"
#include "../kernel/utils.h"

#define FT_NML  1
#define FT_DIR  2

struct INODE {
        unsigned int i_mode;            /* file mode */
        unsigned int i_size;            /* size in bytes */
        unsigned int i_block[8];
};


#define MAX_NAME_LEN 11
 
struct DIR_ENTRY {
        char de_name[MAX_NAME_LEN]; // <?>
        int  de_inode;
};


struct SUPER_BLOCK {
		/* Type of Partition 0x08 */
        unsigned char sb_magic;
        /* Partition Sector Number 0x0c */
        unsigned int sb_start;
        /* Partition Capacity */
        unsigned int sb_blocks;
        /* How Many Blocks to Store the Bitmap of Blocks */
        unsigned int sb_dmap_blks;
        /* How Many Blocks to Store the Bitmap of INodes */
        unsigned int sb_imap_blks;
        /* How Many Blocks to Store the Actual INode Array */
        unsigned int sb_inode_blks;
};


// Macros for Convenience

#define ABS_BOOT_BLK(sb)        ((sb).sb_start)
#define ABS_SUPER_BLK(sb)       ((ABS_BOOT_BLK(sb))+1)
#define ABS_DMAP_BLK(sb)        ((ABS_SUPER_BLK(sb))+1)
#define ABS_IMAP_BLK(sb)        ((ABS_DMAP_BLK(sb))+(sb).sb_dmap_blks)
#define ABS_INODE_BLK(sb)       ((ABS_IMAP_BLK(sb))+(sb).sb_imap_blks)
#define ABS_DATA_BLK(sb)        ((ABS_INODE_BLK(sb))+INODE_BLKS)
 
/* 512*8 = 4096 inodes */
#define INODE_BIT_BLKS          1       
#define INODES_PER_BLK          (512/sizeof(struct INODE))
#define INODE_BLKS              ((INODE_BIT_BLKS*512*8+INODES_PER_BLK-1)/(INODES_PER_BLK))


void setb(void *s, unsigned int i);
void clrb(void *s, unsigned int i);
int testb(void *s, unsigned int i);


void verify_fs(void);



#endif