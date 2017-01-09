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