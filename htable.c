#include <stdio.h>
#include <stdlib.h>
#include "htable.h"
#include <string.h>
#include "mylib.h"

struct htablerec {
	int capacity;
	int num_keys;
	int* freqs;
	char** keys;
        int* stats;
        hashing_t method;
};

/* Frees the entire hash table from memory
 *
 * @params h the htable to free
 */
void htable_free(htable h){
	int i;
	for (i=0; i<h->capacity; i++){
		if (h->freqs[i] >= 1){
			free(h->keys[i]);
		}
	}
	free(h->keys);
	free(h->freqs);
	free(h);
}

/* Convert as a word to a integer.
 * This lets it be used a a htable key
 *
 * @params word pointer to an array of chars to convert
 */
static unsigned int htable_word_to_int(char *word){
	unsigned int result = 0;
	while (*word != '\0'){
		result = (*word++ + 31 * result);
	}
	return result;
}

/* Enters data into the hash table using linear
 * probing. Uses the formula:
 * H(k, i) = (h(k) + i) % m
 * Where h(k) = k % m
 *
 * @params ht the htable to use
 * @params str the char array to enter.
 */
int linear_probing(htable ht, char *str){
        unsigned int h, k, fhash;
        int i = 0;
        for(;;){
                k = htable_word_to_int(str);
                h = k % ht->capacity;
                fhash = (h + i) % ht->capacity;
                if (ht->freqs[h] == 0){
                        break; /* empty slot */
                } else if (strcmp(ht->keys[h], str) == 0){
                        break; /* duplicate */
                } else {
                        i++;
                }
                if (i > ht->capacity){
                        return 0;
                }
        }
	ht->freqs[fhash]++;
	ht->keys[fhash] = emalloc((strlen(str)+1) * sizeof ht->keys[0]);
	strcpy(ht->keys[fhash], str);
	if (ht->freqs[fhash] == 1){ /* only increment num_keys if new item, not duplicate */
		ht->num_keys++;
	}
        return 1;
}

/* Enters data into the hash table using double 
 * hashing. This method uses the formula:
 * H(k,i) = (h(k) + i * g(k)) % m
 * where h(k) = k % m
 * and g(k) = 1 + k % (m - 1)
 *
 * @params ht htable to use
 * @params str string to enter
 */
int double_hash(htable ht, char *str){
	unsigned int fhash, h, g, k;
	int i=0, j;
	for (;;){
		/* Convert to a number */
		k = htable_word_to_int(str);
		/* now hash! */
		h = k % ht->capacity;
		g = 1 + k % (ht->capacity -1);
		fhash = (h + (i * g)) % ht->capacity;
		if (ht->freqs[fhash] == 0){
			break; /* found an empty slot */
		}else if (strcmp(ht->keys[fhash], str) == 0){ 
			break; /* already contains this data */	
		}else{
			i++;
		}
		if (i > ht->capacity){
			fprintf(stderr, "Error: hash table full, cannot add data.");
			return 0;
		}
	}
	ht->freqs[fhash]++;
	ht->keys[fhash] = emalloc((strlen(str)+1) * sizeof ht->keys[0]);
	strcpy(ht->keys[fhash], str);
	if (ht->freqs[fhash] == 1){ /* only increment num_keys if new item, not duplicate */
		ht->num_keys++;
	}
        return 1;
}

/* Inserts a new value into the hash table
 * This passes the insertion role to either
 * linear_probing or double_hash depending
 * on the set options
 *
 * @param ht The hash table to insert into
 * @param str The string to insert
 */
int htable_insert(htable ht, char *str){
        if (ht->method == LINEAR_P){
                return linear_probing(ht, str);
        }else{
                return double_hash(ht, str);
        }
}

/* Creates and return a new htable
 *
 * @params capacity maximum size of the hash table.
 */
htable htable_new(int capacity){
	htable result = emalloc(capacity * sizeof result);
	result->capacity = capacity;
	result->num_keys = 0;
        result->method = LINEAR_P;
	result->keys = emalloc(result->capacity * sizeof result->keys[0]);
	result->freqs = emalloc(result->capacity * sizeof result->freqs[0]);
	return result;
}

void htable_set_double_hashing(htable h){
        h->method = DOUBLE_H;
}

void htable_print(htable h, FILE *stream){
	int i;
	for (i=0; i<h->capacity; i++){
		if (h->freqs[i] > 0){
			fprintf(stream, "%d    %s\n", h->freqs[i], h->keys[i]);
		}
	}
}

/* Searches for a particular word in the
 * hash table. Returns 1 if found, 0 if not.
 *
 * @params ht htable to search in
 * @params str string to search for
 */
int htable_search(htable ht, char *str){
	unsigned int fhash, h, g, k;
	int i=0, j;
	for (;;){
		/* Convert to a number */
		k = htable_word_to_int(str);
		/* now hash! */
		h = k % ht->capacity;
		g = 1 + k % (ht->capacity -1);
		fhash = (h + (i * g)) % ht->capacity;
		if (ht->freqs[fhash] == 0){
			break; /* found an empty slot */
		}else if (strcmp(ht->keys[fhash], str) == 0){ /* weird function that returns 0 if true */
			break; /* already contains this data, reinsert and increment freqs */	
		}else{
			i++;
		}
	}

	return 0;
}

/* Prints the entire hash table.
 * Each entry is printed on a new line.
 * 
 * @params h the hash table to print.
 */
void htable_print_entire_table(htable h){
        int i = 0;
        while (i < h->capacity && i < h->num_keys){
                if (h->keys[i] != NULL){
                        printf("%s\n", h->keys[i]);
                }
                i++;
        }
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
   int current_entries = h->capacity * percent_full / 100;
   double average_collisions = 0.0;
   int at_home = 0;
   int max_collisions = 0;
   int i = 0;

   if (current_entries > 0 && current_entries <= h->num_keys) {
      for (i = 0; i < current_entries; i++) {
         if (h->stats[i] == 0) {
            at_home++;
         } 
         if (h->stats[i] > max_collisions) {
            max_collisions = h->stats[i];
         }
         average_collisions += h->stats[i];
      }
    
      fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full, 
              current_entries, at_home * 100.0 / current_entries,
              average_collisions / current_entries, max_collisions);
   }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
   int i;

   fprintf(stream, "\n%s\n\n", 
           h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
   fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
   fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
   fprintf(stream, "-----------------------------------------------------\n");
   for (i = 1; i <= num_stats; i++) {
      print_stats_line(h, stream, 100 * i / num_stats);
   }
   fprintf(stream, "-----------------------------------------------------\n\n");
}
