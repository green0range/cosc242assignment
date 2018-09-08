#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"

#define DEFAULT_TABLE_SIZE 113

/* A struct to represent the commandline flags
 * given by the user.
 */
struct flags {
	int tree;
	char *check_file;
	int double_hash;
	int entire_contents_printed;
	int output_dot;
	int print_stats;
	int red_black;
	char *s;
	int table_size;
};

/* Main method.
 *
 * @param argc total number of cmd arguments.
 * @param argv array of cmd arguments.
 */
int main(int argc, char **argv){
	const char *optstring = "Tc:deoprs:t:h";
	char option;
	struct flags f;
        htable h;
        char word[256];
	/* Process command line options */
	while((option = getopt(argc, argv, optstring)) != EOF) {
		switch (option) {
			case 'T':
				f.tree = 1;
				break;
                        case 'c':
                                /* f.check_file = emelloc(strlen(optarg) * sizeof f.check_file[0]); */
                                break;
                        case 'd':
                                f.double_hash = 1;
                                break;
                        case 'e':
                                f.entire_contents_printed = 1;
                                break;
                        case 'o':
                                f.output_dot = 1;
                                break;
                        case 'p':
                                f.print_stats = 1;
                                break;
                        case 'r':
                                f.red_black = 1;
                        case 's':
                                /* Not sure what this means */
                                break;
                        case 't':
				f.table_size = atoi(optarg);
                                break;
                        case 'h':
                                print_help();
                                return EXIT_SUCCESS;
                }
        }

        /* Setup the data structure (hash or bst/rbt) */
        if (f.tree == 0){
                h = htable_new(DEFAULT_TABLE_SIZE);
                if (f.double_hash == 1){
                        /* h->method = DOUBLE_H; */
                }
        }else{
                /* create and setup tree */
        }

        /* get words from stdin */
	while (getword(word, sizeof word, stdin) != EOF){
                if (f.tree == 0){
		        htable_insert(h, word);
                }else{
                        /* insert into tree */
                }
	}


	return EXIT_SUCCESS;
}
