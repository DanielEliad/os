#ifndef SHELL_H
#define SHELL_H

#include "utils.h"
#include "../drivers/fs.h"
#include "../memory/header.h"
#include "../drivers/screen.h"

#define shell_base (screen_base + sizeof(struct ScreenBuffer))

#define MAX_DIR_LEN 215
#define MAX_PARAM_LEN 10
struct ShellBuffer {
	char* currentDir;
	char len;			// Make sure to increment me to always equal the len of currentDir :)
};

struct Args {
	char** argv;
	int argc;
};


void freeArgs(struct Args args);

struct Args getArgs(char* command);

void shell_install();

void runCommand(char* command);
void printCurrentDir();

// Handlers to Commands
void handle_ls(struct Args args);
void handle_cd(struct Args args);
void handle_rm(struct Args args);
void handle_touch(struct Args args);
void handle_mkdir(struct Args args);

// Commands
void ls(char* path);
void cd(char* path);
void rm(char* path);
void rmdir(char* path);
void touch(char* path);
void mkdir(char* path);


#endif