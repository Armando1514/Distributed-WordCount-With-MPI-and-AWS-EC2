/*
* Name        : hashmap.h
* hashmap: it implements a hash map structure with the related functionality :
*- insert_or_increments(), inserts the read word if is not present, or increments if present.
*- report_hash_elements(), report all the words into the hash map in a file into the folder "results", with the name of the process that analyzed its.
*- free_hashmap(): de-allocate all the memory related to the hash map structure.
* Author      : Armando Ferrara
*
*
*/

#ifndef HASHMAP_H_
#define HASHMAP_H_

//the size of the hashmap
#define HASHSIZE 500

struct hash_element // entry table
{
    struct hash_element* next; // point to the next element in the chain
    char word[500]; // define the word
    int value; // number of occurrence for word
};

struct hash_element* hash_tab[HASHSIZE]; // pointer table

/* report_hash_elements():
 * display the hash elements and
 * the computation timing */
void report_hash_elements(float execution_time, short my_rank);

/* insert_or_increment(char *s, int value):
 * if there is yet the node, increments the value,
 * if not, allocates a new node with value to 1. */

void insert_or_increment(char* s, int value);

/* free_hashmap():
 *  deallocate all the hashmap structure */
void free_hashmap();

#endif /* HASHMAP_H_ */