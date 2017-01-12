#include "shell.h"


void freeArgs(struct Args args) {
	for(int i = 0; i < args.argc; i++) {
		free(args.argv[i]);
	}

	free(args.argv);
}

struct Args getArgs(char* command) {
	char** args = malloc(MAX_PARAM_LEN*sizeof(char*) + 1);
	int oldIndex = 0;
	int newIndex = findFirst(' ', command);
	int len_command = strlen(command);
	int i = 0;
	while(i < MAX_PARAM_LEN && newIndex != -1) {
		args[i] = malloc(newIndex - oldIndex + 1);	// +1 for the null terminator
		memory_copy(command + oldIndex, args[i], newIndex - oldIndex);
		oldIndex = newIndex + 1;
		newIndex = find(' ', command, newIndex);
		i++;
	}

	args[i] = malloc(len_command - oldIndex + 1);
	memory_copy(command + oldIndex, args[i], len_command - oldIndex);
	args[i][len_command - oldIndex + 1] = 0;
	struct Args a;
	a.argv = args;
	a.argc = i + 1;
	return a;
}


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

	struct Args args = getArgs(command);

	if(strcmp(args.argv[0], "ls") == 0) {
		handle_ls(args);
	} else if(strcmp(args.argv[0], "cd") == 0) {
		handle_cd(args);
	} else if(strcmp(args.argv[0], "rm") == 0) {
		handle_rm(args);
	} else if(strcmp(args.argv[0], "touch") == 0) {
		handle_touch(args);
	} else if(strcmp(args.argv[0], "mkdir") == 0) {
		handle_mkdir(args);
	} else if(strcmp(args.argv[0], "cat") == 0) {
		handle_cat(args);
	} else if(strcmp(args.argv[0], "write") == 0) {
		handle_write(args, command);
	}


	freeArgs(args);
	
}

void printCurrentDir() {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	printColor(shellBuffer->currentDir, GREEN_ON_BLACK); printColor(" ", GREEN_ON_BLACK);
}

void handle_ls(struct Args args) {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	
	if(args.argc == 1) {
		ls(shellBuffer->currentDir);
	}

	
	char multipleFiles = args.argc > 2;
	for(int i = 1; i < args.argc; i++) {	

		if(args.argv[i][0] == '/') {
			if(multipleFiles) {
				printch('\n'); print(args.argv[i]); print(":\n");
			}
			ls(args.argv[i]);
		} else {
			char* completePath = concat(shellBuffer->currentDir, args.argv[i]);
			if(multipleFiles) {
				printch('\n'); print(completePath); print(":\n");
			}
			ls(completePath);
			free(completePath);
		}
	}
}

void handle_cd(struct Args args) {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	if(args.argc > 2) {
		printColor("Too many arguments. Usage:\n\tcd <dir>\n", RED_ON_BLACK);
		return;
	}


	if(args.argc == 1) {	// cd into the root directory
		cd("/");
		return;
	}

	if(args.argv[1][0] == '/') {
		cd(args.argv[1]);
		return;
	}

	char* completePath = concat(shellBuffer->currentDir, args.argv[1]);
	cd(completePath);
	free(completePath);
}

void handle_rm(struct Args args) {
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;

	if(args.argc == 1) {
		printColor("Provide a file to remove\n", RED_ON_BLACK);
		return;
	}

	int i = 1;
	char dir = 0;
	if(strcmp("--r", args.argv[1]) == 0) {
		if(args.argc == 2) {
			printColor("Provide a directory to remove\n", RED_ON_BLACK);
			return;
		}

		i = 2;
		dir = 1;
	}

	for(; i < args.argc; i++) {
		if(args.argv[i][0] == '/') {
			if(dir) {
				rmdir(args.argv[i]);
			} else {
				rm(args.argv[i]);
			}
		} else {
			char* completePath = concat(shellBuffer->currentDir, args.argv[i]);
			if(dir) {
				rmdir(completePath);
			} else {
				rm(completePath);
			}
			free(completePath);
		}
	}
}

void handle_touch(struct Args args) {
	
	if(args.argc == 1) {
		printColor("Provide a file to create\n", RED_ON_BLACK);
		return;
	}

	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	
	for(int i = 1; i < args.argc; i++) {
		if(args.argv[i][0] == '/') {
			touch(args.argv[i]);
		} else {
			char* completePath = concat(shellBuffer->currentDir, args.argv[i]);
			touch(completePath);
			free(completePath);
		}
	}
}

void handle_mkdir(struct Args args) {
	if(args.argc == 1) {
		printColor("Provide a directory to create\n", RED_ON_BLACK);
		return;
	}

	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	
	for(int i = 1; i < args.argc; i++) {
		if(args.argv[i][0] == '/') {
			mkdir(args.argv[i]);
		} else {
			char* completePath = concat(shellBuffer->currentDir, args.argv[i]);
			mkdir(completePath);
			free(completePath);
		}
	}
}

void handle_cat(struct Args args) {
	if(args.argc == 1) {
		printColor("Provide a file to cat\n", RED_ON_BLACK);
		return;
	}

	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	
	for(int i = 1; i < args.argc; i++) {
		if(args.argv[i][0] == '/') {
			cat(args.argv[i]);
		} else {
			char* completePath = concat(shellBuffer->currentDir, args.argv[i]);
			cat(completePath);
			free(completePath);
		}
	}
}

void handle_write(struct Args args, char* command) {
	if(args.argc == 1) {
		printColor("Provide a file to write into\n", RED_ON_BLACK);
		return;
	}

	if(args.argc == 2) {
		printColor("Provide data to write into the file\n", RED_ON_BLACK);
		return;
	}

	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;
	
	int indexSecondArg = findFirst(' ', command) + 1;
	int indexThirdArg = find(' ', command, indexSecondArg) + 1;
	int indexEndThirdArg = strlen(command) - 1;
	if(command[indexThirdArg] != '"') {
		printColor("Data to the `write` command should start with \"\n", RED_ON_BLACK);
		return;
	}
	if(command[indexEndThirdArg] != '"') {
		printColor("Data to the `write` command should start end with \"\n", RED_ON_BLACK);
		return;
	}

	indexEndThirdArg -= 1;	// Skip the "
	indexThirdArg += 1;	// Skip the "

	unsigned int len_data = indexEndThirdArg - indexThirdArg + 1;	// Difference of the indexes + 1 to equal the size
	char* data = malloc(len_data);
	memory_copy(command + indexThirdArg, data, len_data);

	if(args.argv[1][0] == '/') {
		write(args.argv[1], data, len_data);
	} else {
		char* completePath = concat(shellBuffer->currentDir, args.argv[1]);
		write(completePath, data, len_data);
		free(completePath);
	}

	free(data);
}



void ls(char* path) {

	char* pathToFolder = simplify(path);
	
	if(strcmp(pathToFolder, "/..") == 0) {
		printColor("Illegal file or file not found\n", RED_ON_BLACK);
		return;
	}

	struct INODE_NUM file = findFile(path);

	if(file.inode_num == -1) {	// Illegal file or file not found
		printColor("Illegal file or file not found\n", RED_ON_BLACK);
		return;
	}

	unsigned int i = 0;
    char sect[512] = {0};
    struct DIR_ENTRY *de;

    switch (file.inode.i_mode) {
    case FT_NML:
    	if(path[strlen(path) - 1] == '/') path[strlen(path) - 1] = 0;
        printColor(path, WHITE_ON_BLACK); printColor(" - File", WHITE_ON_BLACK); printch('\n');
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
				printColor("UNKNOWN FILE TYPE!!\n", RED_ON_BLACK);
        		for(;;); // halt();
			}
			printch('\t');
    	}
        printch('\n');
        break;
    default:
        printColor("UNKNOWN FILE TYPE!!\n", RED_ON_BLACK);
        for(;;); // halt();
    }
}

void cd(char* path) {
	char* pathToFolder = simplify(path);
	unsigned int len_path = strlen(pathToFolder);
	
	if(strcmp(pathToFolder, "/..") == 0) {
		printColor("Illegal file or file not found\n", RED_ON_BLACK);
		return;
	}
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;


	struct INODE_NUM folder = findFile(path);

	if(folder.inode_num == -1) {	// Illegal file or file not found
		printColor("Illegal file or file not found\n", RED_ON_BLACK);
		return;
	}

	
    switch (folder.inode.i_mode) {
    case FT_NML:
    	printColor("Can't change directory into a file\n", RED_ON_BLACK);
        break;
    case FT_DIR:
    	memory_copy(pathToFolder, shellBuffer->currentDir, len_path);
    	if(pathToFolder[len_path - 1] != '/') {
    		shellBuffer->currentDir[len_path] = '/';
    		shellBuffer->currentDir[len_path + 1] = 0;
    		shellBuffer->len = len_path + 1;
    	} else {
	    	shellBuffer->currentDir[len_path] = 0;
	    	shellBuffer->len = len_path;
    	}        
        break;
    default:
        printColor("UNKNOWN FILE TYPE!!\n", RED_ON_BLACK);
        for(;;); // halt();
    }

	free(pathToFolder);
}

void rm(char* path) {
	struct INODE_NUM file = findFile(path);

	if(file.inode_num == -1) {	// Illegal file or file not found
		printColor("Illegal file or file not found\n", RED_ON_BLACK);
		return;
	}

	switch (file.inode.i_mode) {
    case FT_NML:
    	deleteFile(path);
        break;
    case FT_DIR:
        printColor("Use --r to recursively delete directories and files\n", RED_ON_BLACK);
        break;
    default:
        printColor("UNKNOWN FILE TYPE!!\n", RED_ON_BLACK);
        for(;;); // halt();
    }
}

void rmdir(char* path) {
	struct INODE_NUM file = findFile(path);
	
	if(file.inode_num == -1) {	// Illegal file or file not found
		printColor("Illegal file or file not found\n", RED_ON_BLACK);
		return;
	}

	switch (file.inode.i_mode) {
    case FT_NML:
    	deleteFile(path);
        break;
    case FT_DIR:
        deleteFile(path);
        break;
    default:
        printColor("UNKNOWN FILE TYPE!!\n", RED_ON_BLACK);
        for(;;); // halt();
    }
}

void touch(char* path) {
	char* simplifiedPath = simplify(path);
	struct FOLDER f = getNameOfContainingFolder(simplifiedPath);
	makeFile(f.dirName, f.pathToDir);
	free(simplifiedPath);
	free(f.pathToDir);
	free(f.dirName);
}

void mkdir(char* path) {
	char* simplifiedPath = simplify(path);
	struct FOLDER f = getNameOfContainingFolder(simplifiedPath);
	makeFolder(f.dirName, f.pathToDir);
	free(simplifiedPath);
	free(f.pathToDir);
	free(f.dirName);	
}

void cat(char* path) {
	char* simplifiedPath = simplify(path);
	char* content = readFromFile(simplifiedPath);
	if(content == 0) return;
	print(content);
	printch('\n');
	free(simplifiedPath);
	free(content);
}

void write(char* path, char* data, unsigned int len_data) {
	char* simplifiedPath = simplify(path);
	writeToFile(simplifiedPath, data, len_data);
}

