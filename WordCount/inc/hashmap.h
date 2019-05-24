/*
* Name        : hashmap.h
* Author      : Armando Ferrara
* Define a simple hashmap with function for isnert and search.
*
 */


#ifndef HASHMAP_H_
#define HASHMAP_H_
#define HASHSIZE 1

/* display all the elements */
void hashmapwordreport();





/* insert: define the function for insert the word and the value in the hashtab */
struct hashelement *insertwithincrement(char *word, int value);

/* freeHashmap: deallocate all the hashmap structure */
void freeHashmap();

#endif /* HASHMAP_H_ */
