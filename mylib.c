#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include <ctype.h>
#include <assert.h>

int getword(char *s, int limit, FILE *stream){
	int c;
	char *w = s;
	assert(limit > 0 && s != NULL && stream != NULL);

	while (!isalnum(c = getc(stream)) && EOF != c);
	
	if (EOF == c){
		return EOF;
	}else if (--limit > 0) {
		*w++ = tolower(c);
	}
	while (--limit > 0){
		if (isalnum(c = getc(stream))){
			*w++ = tolower(c);
		}else if ('\'' == c){
			limit++;
		}else{
			break;
		}
	}
	*w = '\0';
	return w-s;
}

void print_help(){
        printf("Options:\n");
        printf("    -T Use tree data structure (hash table is default)\n");
        printf("    -d Use double hashing (linear probing is default)\n");
        printf("    -e Display entire contents of the hash table on stderr\n");
        printf("    -o Output the tree as a dot file\n");
        printf("    -p Print statistics about data\n");
        printf("    -r Use a Red Black tree (default has no rebalancing)\n");
        printf("    -h Prints this help message\n");
}

void *emalloc(size_t s){
	void *result = malloc(s);
	if (NULL == result){
		fprintf(stderr, "memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	return result;
}

void *erealloc(void *p, size_t s){
	void *result = realloc(p, s);
	if (NULL == result){
		fprintf(stderr, "memory reallocation failed.\n");
		exit(EXIT_FAILURE);
	}
	return result;
}
