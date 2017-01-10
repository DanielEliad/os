#include "shell.h"

void shell_install() {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	shellBuffer->currentDir = malloc(MAX_DIR_LEN);
	shellBuffer->len = 0;
	// Init current directory to "/"
	shellBuffer->currentDir[0] = '/';
	shellBuffer->currentDir[1] = 0;
	shellBuffer->len = 1;

}

void runCommand(char* command) {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;

	if(command[0] == 0) {	// Empty command
		return;
	}

	int index = findFirst(' ', command);
	if(index == -1) {	// Doesn't exist
		ls(shellBuffer->currentDir);
		return;
	}

	if(strcmpFrom(command, "ls", index) == 0) {
		int oldIndex = index;
		int newIndex = find(' ', command, oldIndex);
		char multipleFiles = 0;
		while(newIndex != -1) {
			multipleFiles = 1;

			// newIndex - 1: skip the space
			// oldIndex + 1: skip the space
			// +1: to make it length and not index from oldIndex
			int len = (newIndex - 1) - (oldIndex + 1) + 1;
			if(command[oldIndex + 1] == '/') {
				char tmp[len];
				memory_copy(command + oldIndex + 1, tmp, len);
				printch('\n'); print(tmp); print(":");
				ls(tmp);
			} else {
				char tmp[len + shellBuffer->len];
				memory_copy(shellBuffer->currentDir, tmp, shellBuffer->len);
				// -1 for the command: skip the space
				memory_copy(command + oldIndex + 1, tmp + shellBuffer->len, len);
				printch('\n'); print(tmp); print(":");
				ls(tmp);
			}
			// char indexBuffer[20];
			// itoa(newIndex, indexBuffer); print('\n------------'); print(indexBuffer); printch('------------\n');
			// itoa(oldIndex, indexBuffer); print('\n------------'); print(indexBuffer); printch('------------\n');
			oldIndex = newIndex;
			newIndex = find(' ', command, oldIndex);
		}

		int len = strlen(command) - oldIndex;
		if(command[oldIndex + 1] == '/') {
			char tmp[len + 1];
			memory_copy(command + oldIndex + 1, tmp, len);
			
			if(multipleFiles) {
				printch('\n'); print(tmp); print(":");
			}

			ls(tmp);
		} else {
			char tmp[len + 1 + shellBuffer->len];
			memory_copy(shellBuffer->currentDir, tmp, shellBuffer->len);
			// +1 for the command: skip the space
			memory_copy(command + oldIndex + 1, tmp + shellBuffer->len, len);
			if(multipleFiles) {
				printch('\n'); print(tmp); print(":");
			}
			ls(tmp);			
		}
	}

	// print(command);
	// printch('\n');
}



void ls(char* path) {
	print(path);
	struct INODE_NUM file = findFile(path);
	if(file.inode_num == -1) {	// Illegal file or file not found
		print("\nIllegal file or file not found\n");
		return;
	}
	// char t[20]; itoa(file.inode_num, t); printch('\n'); print(t); printch('\n');
	unsigned int i = 0;
    char sect[512] = {0};
    struct DIR_ENTRY *de;

    printch('\n');
    switch (file.inode.i_mode) {
    case FT_NML:
    	if(path[strlen(path) - 1] == '/') path[strlen(path) - 1] = 0;
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



