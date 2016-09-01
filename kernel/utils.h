#ifndef UTILS_H
#define UTILS_H

void memory_copy(char* source, char* dest, int n_bytes);
void memory_set(char* source, char val, int n_bytes);
char* itoa(int num);
void reverse(char s[]);
int strlen(char s[]);

#endif