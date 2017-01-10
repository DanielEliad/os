#ifndef SHELL_H
#define SHELL_H

#include "utils.h"
#include "../drivers/fs.h"
#include "../memory/header.h"

#define shell_base (0x100000)

#define MAX_DIR_LEN 215
struct ShellBuffer {
	char* currentDir;	// Make sure I have a trailing slash always :)
	char len;			// Make sure to increment me to always equal the len of currentDir :)
};

void shell_install();

void runCommand(char* command);

// Commands
void ls(char* path);


#endif