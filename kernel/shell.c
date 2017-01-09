#include "shell.h"

void shell_install() {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	shellBuffer->currentDir = malloc(MAX_DIR_LEN);
	
	// Init current directory to "/"
	shellBuffer->currentDir[0] = '/';
	shellBuffer->currentDir[1] = 0;
}

void runCommand(char* command) {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;

	if(command[0] == 0) {	// Empty command
		return;
	}
	int index = findFirst(' ', command);
	if(index == -1) {	// Doesn't exist
		index = strlen(command);
	}
	if(strcmpFrom(command, "ls", index) == 0) {
		ls(shellBuffer->currentDir);
	}

	// print(command);
	// printch('\n');
}



void ls(char* path) {
	struct INODE_NUM file = findFile(path);

	unsigned int i = 0;
    char sect[512] = {0};
    struct DIR_ENTRY *de;

    printch('\n');
    switch (file.inode.i_mode) {
    case FT_NML:
        print(path); print(" - File"); printch('\n');
        break;
    case FT_DIR:
        HD_RW(file.inode.i_block[0], HD_READ, 1, sect);
        de = (struct DIR_ENTRY *)sect;
		for (i = 0; i < file.inode.i_size/sizeof(struct DIR_ENTRY); ++i) {
			unsigned int fileType = getFileType(de[i].de_inode);
			if(fileType == FT_NML) {
				printColor(de[i].de_name, WHITE_ON_BLACK);
			} else if(fileType == FT_DIR){
				printColor(de[i].de_name, BLUE_ON_BLACK);
			} else {
				print("UNKNOWN FILE TYPE!!\n");
        		for(;;); // halt();
			}
			printch('\t');
    	}
        printch('\n');
        break;
    default:
        print("UNKNOWN FILE TYPE!!\n");
        for(;;); // halt();
    }
}



