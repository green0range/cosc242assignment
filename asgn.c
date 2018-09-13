#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"
#include "tree.h"

#define DEFAULT_TABLE_SIZE 113

/* A struct to represent the command line flags
 * given by the user.
 */
struct flags {
    int tree;
    char *check_file;
    hashing_t hashing_method;
    int entire_contents_printed;
    int output_dot;
    int print_stats;
    int red_black;
    int snapshot_count;
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
    tree b = NULL;
    char word[256];
    int num_entries = 0;
    /* Process command line options */
    f.tree = 0;
    f.hashing_method = LINEAR_P;
    f.entire_contents_printed = 0;
    f.output_dot = 0;
    f.print_stats = 0;
    f.red_black = 0;
    f.snapshot_count = 0;
    f.table_size = 0;
    while((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'T':
                f.tree = 1;
                break;
            case 'c':
                /* f.check_file = emalloc(strlen(optarg) * sizeof f.check_file[0]); */
                break;
            case 'd':
                f.hashing_method = DOUBLE_H;
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
                break;
            case 's':
                f.snapshot_count = atoi(optarg);
                break;
            case 't':
                f.table_size = atoi(optarg);
                break;
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;
        }
    }
    
    /* Setup the data structure (hash or tree/rbt) */
    if (f.tree == 0){
        if (f.table_size > 0){
            h = htable_new(find_greater_prime(f.table_size), f.hashing_method);
        } else {
            h = htable_new(DEFAULT_TABLE_SIZE, f.hashing_method);
        }
    } /* we do not need to setup the tree as this is done automatically when
         tree_insert is called, if it is passed a NULL pointer. */
    
    /* get words from stdin */
    while (getword(word, sizeof word, stdin) != EOF){
        if (f.tree == 0){
            htable_insert(h, word);
        }else{
            if (f.red_black == 1){
                b = tree_insert(b, word, RBT);
            }else{
                b = tree_insert(b, word, BST);
            }
        }
        num_entries++;
    }        
    if (f.tree == 0){
        if (f.entire_contents_printed == 1){
            htable_print_entire_table(h);
        }
        if (f.print_stats == 0){
            htable_print(h, stdout);
        }else{
            if (f.snapshot_count == 0){
                htable_print_stats(h, stdout, num_entries);
            }
            else{
                htable_print_stats(h, stdout, f.snapshot_count);
            }
        }
    }else{
        tree_preorder(b, tree_print_key);
        if (f.output_dot == 1){
            FILE *fptr;
            fptr = fopen("tree-view.dot", "w");
            tree_output_dot(b, fptr);
        }
    }
    if (f.tree == 0){
        htable_free(h);
    }else{
        tree_free(b);
    }
    
    return EXIT_SUCCESS;
}
