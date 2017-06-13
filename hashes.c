#include "hashes.h"
#include <stdlib.h>
#include <string.h>

unsigned long hash1(const char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

unsigned long hash2(const char *str) {
	unsigned long hash = 0;
	while (*str != '\0') {
		hash = (hash << 4) + *(str++);
		unsigned long g = hash & 0xF0000000L;
		if (g != 0) hash ^= g >> 24;
		hash &= ~g;
	}
	return hash;
}

unsigned long hash3(const char *str){
	unsigned long hash = 0;
	int c;

	while ((c = *str++))
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

unsigned long hash4(const char *str){
    unsigned long hash, i, len = strlen(str);
    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

unsigned long hash5(const char* str){
	unsigned long hash = 0x811c9dc5;
	while (*str) {
		hash ^= (unsigned char) *str++;
		hash *= 0x01000193;
	}
	return hash;
}
