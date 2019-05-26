/*
* Name        : hashmap.h
* Author      : Armando Ferrara
* Define a simple hashmap with function for isnert and search.
*
 */


#ifndef HASHMAP_H_
#define HASHMAP_H_
#define HASHSIZE 2

struct hashelement // entry table
{
	 struct haselement *next; // point to the next element in the chain
	 char *word; // define the word
	 int value; // number of occurrence for word
};

struct hashelement *hashtab[HASHSIZE]; // pointer table


/* display all the elements */
void displayhashelement();






/* insert: define the function for insert the word and the value in the hashtab */
void insertwithincrement(char *word);

/* freeHashmap: deallocate all the hashmap structure */
void freeHashmap();

#endif /* HASHMAP_H_ */
