#include "utils.h"

void memory_copy(char* source, char* dest, int n_bytes) {
	for(int i = 0; i < n_bytes; i++) {
		*(dest + i) = *(source + i);
	}
}


void memory_set(char* dest, char val, int n_bytes) {
	for(int i = 0;i<n_bytes;i++) {
		*(dest + i) = val;
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

void strcopy(char* source, char* dst) {
    memory_copy(source, dst, strlen(source));
}

int strcmp(char* s1, char* s2) { //returns 1 if they are not equal and 0 if they are
    int len = strlen(s1);
    if(len != strlen(s2)) return 1;
    for(int i = 0; i < strlen(s1); i++) {
        if(s1[i] != s2[i]) return 1;
    }

    return 0;
}

int strcmpFrom(char* s1, char* s2, int index) {
    if(strlen(s1) < index || strlen(s2) < index) {
        return 1;
    }

    for(int i = 0; i < index; i++) {
        if(s1[i] != s2[i]) return 1;
    }

    return 0;
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


int findFirst(char ch, char* s) {
    return find(ch, s, 0);
}

int find(char ch, char* s, int start) {
    int len = strlen(s);
    for(int i = start + 1; i < len; i++) {
        if(s[i] == ch) return i;
    }

    return -1;
}


