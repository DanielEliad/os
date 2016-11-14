#ifndef UTILS_H
#define UTILS_H

#include "../drivers/screen.h"
void memory_copy(char* source, char* dest, int n_bytes);
void memory_set(char* source, char val, int n_bytes);
void itoa(int num, char* str);
void reverse(char s[]);
int strlen(char s[]);
int get_num_digits(int num);
char* malloc(int n_bytes);

#endif