/*
* Name        : hashmap.h
* Author      : Armando Ferrara
* Define a simple hashmap with function for insert and search.
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

struct hash_element* get_hash_head();

/* report_hash_elements():
 * display the hash elements and
 * the computation timing */
void report_hash_elements();

/* insert_or_increment(char *s):
 * if there is yet the node, increments the value,
 * if not, allocates a new node with value to 1. */

void insert_or_increment(char* s);

/* free_hashmap():
 *  deallocate all the hashmap structure */
void free_hashmap();

#endif /* HASHMAP_H_ */
