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
		// print("\nNew Arg: "); char t[20]; itoa(i, t); print(t); printch('\n');
		args[i] = malloc(newIndex - oldIndex + 1);	// +1 for the null terminator
		// print(command + oldIndex);
		// char l[20]; itoa(args[i], l); print("args[i]: "); print(l); printch('\n');
		memory_copy(command + oldIndex, args[i], newIndex - oldIndex);
		oldIndex = newIndex + 1;
		newIndex = find(' ', command, newIndex);
		i++;
	}
	// for(i = 0; i < MAX_PARAM_LEN && newIndex != -1 ; i++) {
	// 	print("\nNew Loop\n");
	// 	args[i] = malloc(newIndex - oldIndex + 1);	// +1 for the null terminator
	// 	memory_copy(command + oldIndex, args[i], newIndex - oldIndex);
	// 	oldIndex = newIndex;
	// 	newIndex = find(' ', command, newIndex);
	// }

	// print("\nNew Arg: "); char t[20]; itoa(i, t); print(t); printch('\n');
	args[i] = malloc(len_command - oldIndex + 1);
	// print(command + oldIndex);
	// char l[20]; itoa(args[i], l); print("args[i]: "); print(l); printch('\n');
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
	} // else if(strcnp(args.argv[0], "rm"))


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

void ls(char* path) {
	struct INODE_NUM file = findFile(path);

	if(file.inode_num == -1) {	// Illegal file or file not found
		print("Illegal file or file not found\n");
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


void cd(char* path) {
	
	struct ShellBuffer* shellBuffer = (struct ShellBuffer* ) shell_base;


	struct INODE_NUM folder = findFile(path);

	if(folder.inode_num == -1) {	// Illegal file or file not found
		print("Illegal file or file not found\n");
		return;
	}

	char* pathToFolder = simplify(path);
	unsigned int len_path = strlen(pathToFolder);
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
        print("UNKNOWN FILE TYPE!!\n");
        for(;;); // halt();
    }

	free(pathToFolder);
}


