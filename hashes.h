#ifndef HASHES_H
#define HASHES_H

unsigned long hash1(const char *str);

unsigned long hash2(const char *str);

unsigned long hash3(const char *str);

unsigned long hash4(const char *str);

unsigned long hash5(const char* str);

typedef unsigned long(*fhash_t)(const char*);

#endif // HASHES_H

