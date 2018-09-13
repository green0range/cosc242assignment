#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include <ctype.h>
#include <assert.h>

#define DEFAULT_TABLE_SIZE 113

/* Function for getting words from file
 *
 * @param s
 * @param limit
 * @param stream
 * @return
 */
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

/* Determines if a number is a prime.
 *
 * @param p the integer to be checked
 */
int is_prime(int p){
    int i;
    for (i=2; i< p; i++){
        if (p % i == 0){
            return 0;
        }
    }
    return 1;
}

/* Returns a prime number greater than n.
 *
 * @param n the number the prime must be >= to
 */
int find_greater_prime(int n){
    int i = n;
    while (is_prime(i) == 0){
        i++;
    }
    fprintf(stdout, "using prime %d\n", i);
    return i;
}

/* Returns the table size to use */
int table_size(int s){
	if (s == 0){
		return DEFAULT_TABLE_SIZE;
	}else{
		return find_greater_prime(s);
	}
}

/* Function prints user options for command line arguments.
 *
 * @param prog_name pointer to charthe name of our program
 */
void print_help(char *prog_name){
    printf("Usage: %s [OPTIONS]... <STDIN>\n", prog_name);
    printf("\n");
    printf("Perform tasks using a hash table or binary tree.  By default, ");
    printf("words\n");
    printf("read from stdin are added to the data structure before printing\n");
    printf("them, along with their frequencies, to stdout.\n");
    printf("\n");
    printf("-T           Use tree data structure (default is hash table)\n");
    printf("-c FILENAME  Check spelling of words in FILENAME using words\n");
    printf("             from stdin as dictionary. Print unknown words to\n");
    printf("             stdout, timing info etc to stderr (ignore -o & -p)\n"); 
    printf("-d           Use double hashing (linear probing is default)\n");
    printf("-e           Display entire contents of hash table on stderr\n");
    printf("-o           Output the tree in DOT form to file 'tree-view.dot'\n");
    printf("-p           Print hash table stats instead of frequencies & words");
    printf("\n");
    printf("-r           Make the tree an RBT (the default is a BST)\n");
    printf("-s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n");
    printf("-t TABLESIZE Use the first prime >= TABLESIZE as htable size\n");
    printf("\n");
    printf("-h           Display this message\n");
    printf("\n");
}

/* Standard *emalloc function.
 *
 * @param s
 */
void *emalloc(size_t s){
    void *result = malloc(s);
    if (NULL == result){
        fprintf(stderr, "memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/* Standard *erealloc function.
 *
 * @param p
 * @param s
 */
void *erealloc(void *p, size_t s){
    void *result = realloc(p, s);
    if (NULL == result){
        fprintf(stderr, "memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}
