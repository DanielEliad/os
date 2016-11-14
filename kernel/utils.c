#include "utils.h"

void memory_copy(char* source, char* dest, int n_bytes) {
	for(int i = 0; i < n_bytes; i++) {
		*(dest + i) = *(source + i);
	}
}


void memory_set(char* source, char val, int n_bytes) {
	for(int i = 0;i<n_bytes;i++) {
		*(source + i) = val;
	}
}


void itoa(int num, char* str) {
	int i = 0;
    char isNegative = 0;
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num/10;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str);
 
    return;
}


void reverse(char s[]) {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

int strlen(char s[]) {
	int i = 0;
	int len = 0;
	while(s[i++] != 0) {
		len++;
	}

	return len;
}


int get_num_digits(int num) {
	if(num == 0) return 1;
    int i = 0;
    if(num < 0) i++;
	while(num != 0) {
		i++;
		num = num/10;
	}

	return i;
}


char* malloc(int n_bytes) {
    
}
