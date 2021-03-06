#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"
#include "tree.h"

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
    rbt_colour red_black;
    int snapshot_count;
    int table_size;
};

/* Main method.
 *
 * @param argc total number of cmd arguments
 * @param argv array of cmd arguments
 */
int main(int argc, char **argv){
    const char *optstring = "Tc:deoprs:t:h";
    char option;
    struct flags f;
    htable h;
    tree b = NULL;
    char word[256];
    FILE *fptr;
    int unknown_word_count = 0;
    double fill_time = 0.0;
    double search_time = 0.0;
    clock_t start, end;
    /* process command line options */
    f.tree = 0;
    f.hashing_method = LINEAR_P;
    f.check_file = NULL;
    f.entire_contents_printed = 0;
    f.output_dot = 0;
    f.print_stats = 0;
    f.red_black = BST;
    f.snapshot_count = 0;
    f.table_size = 0;
    while((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'T':
                f.tree = 1;
                break;
            case 'c':
                f.check_file = emalloc((strlen(optarg)+1) *
                                       sizeof f.check_file[0]);
                strcpy(f.check_file, optarg);
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
                f.red_black = RBT;
                break;
            case 's':
                f.snapshot_count = atoi(optarg);
                break;
            case 't':
                f.table_size = atoi(optarg);
                break;
            case 'h':
            default:
                print_help(argv[0]);
                return EXIT_SUCCESS;
        }
    }

    /* setup the data structure (hash or tree/rbt) */
    if (f.tree == 0){
        h = htable_new(table_size(f.table_size), f.hashing_method);
    } /* we do not need to setup the tree as this is done automatically when
         tree_insert is called, if it is passed a NULL pointer. */
    
    /* get words from stdin */
    while (getword(word, sizeof word, stdin) != EOF){
        if (f.tree == 0){
            start = clock();
            htable_insert(h, word);
            end = clock();
        }else{
            start = clock();
            b = tree_insert(b, word, f.red_black);
            b = tree_make_black(b);
            end = clock();
        }
        fill_time += (end - start)/(double)CLOCKS_PER_SEC;
    } 
    if (f.check_file != NULL){
        /* read file into another function then search and match words */
        if (NULL == (fptr = fopen(f.check_file, "r"))){
            fprintf(stderr, "Can't open file '%s' using mode r.\n", f.check_file);
            return EXIT_FAILURE;
        }
        while (getword(word, sizeof word, fptr) != EOF){
            if (f.tree == 0){
                start = clock();
                if (htable_search(h, word) == 0){
                    end = clock();
                    fprintf(stderr, "%s\n", word);
                    unknown_word_count++;
                }else{
                    end = clock();
                }
            }else{
                start = clock();
                if (tree_search(b, word) == 0){
                    end = clock();
                    fprintf(stderr, "%s\n", word);
                    unknown_word_count++;
                }else{
                    end = clock();
                }
            }
            search_time += (end - start)/(double)CLOCKS_PER_SEC;
        } 
        printf("Fill time     : %f\n", fill_time);
        printf("Search time   : %f\n", search_time);
        printf("Unknown words = %d\n", unknown_word_count);

    }else{
        if (f.tree == 0){
            if (f.entire_contents_printed == 1){
                htable_print_entire_table(h);
            }
            if (f.print_stats == 0){
                htable_print(h, stdout);
            }else{
                if (f.snapshot_count == 0){
                    htable_print_stats(h, stdout, 10);
                }
                else{
                    htable_print_stats(h, stdout, f.snapshot_count);
                }
            }
        }else{
            if (f.output_dot == 1){
                fptr = fopen("tree-view.dot", "w");
                tree_output_dot(b, fptr);
            }
            tree_preorder(b, tree_print_key);
        }
    }
    if (f.tree == 0){
        htable_free(h);
    }else{
        tree_free(b);
    }
    
    return EXIT_SUCCESS;
}
