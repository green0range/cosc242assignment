#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char *s, int limit, FILE *stream);
extern void print_help();
extern int find_greater_prime(int n);
extern int table_size(int s);

#endif
