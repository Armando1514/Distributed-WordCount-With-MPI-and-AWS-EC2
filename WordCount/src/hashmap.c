/*
 * hashmap.c
 * Implementation of a simple hashmap with function for isnert and search.
 * Author: Armando Ferrara
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <hashmap.h>


struct hashelement // entry table
{
	 struct haselement *next; // point to the next element in the chain
	 char *word; // define the word
	 int value; // number of occurrence for word
};

static struct hashelement *hashtab[HASHSIZE]; // pointer table


/* hash generator for string s*/
unsigned hash (char *s)
{
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++) // for each character in s
		hashval = *s + 31 * hashval; // calculate this number
	return hashval % HASHSIZE;

}

/* display all the elements */
void hashmapwordreport()
{
	int i = 0;
	struct hashelement * np;
    long numberOfWords = 0;
	while( i < HASHSIZE)
	{
		np = hashtab[i];
		   while(np != NULL)// scan the list
		  {
			  printf("word: %s , value: %d \n", np->word,np->value);
			  numberOfWords = numberOfWords + np->value;
			  np =(struct hashelement*) np->next;

		  }
		  i++;
	}
	printf("Number of word of length > 2 : %ld\n", numberOfWords);
}

/* lookup: search s in the hashtab */
struct hashelement *lookup(char *s)
{
	struct hashelement *np;
	for (np = hashtab[hash(s)]; np != NULL; np = (struct hashelement*)np->next) // scan the list
		if(strcmp(s, np->word) == 0) // compare the two strings
			return np; // the word is found
	return NULL; //the word is not found
}

/* freeHashmap: deallocate all the hashmap structure */
void freeHashmap()
{
	int i = 0;
	struct hashelement *np;
	struct hashelement *nptmp;

	while (i < HASHSIZE)
	{
	np = hashtab[i];
	   while(np != NULL)// scan the list
	  {
		  nptmp = (struct hashelement*) np->next;

		 // remove the memory corresponding to np
		  free(&np->value);
		  free(np->word);
		  free(np);

		  np = nptmp;
	  }
	  i++;
	}
}

/* insert: define the function for insert the word and the value in the hashtab */
struct hashelement *insertwithincrement(char *word, int value)
{
	struct hashelement *np;
	unsigned hashval;


	if((np = lookup(word)) == NULL)
	{ /* not found */
		np = (struct hashelement *) malloc (sizeof (*np));
        np->word = malloc (strlen(word) + 1);

		if(np==NULL || (np->word = strdup(word)) == NULL)
			return NULL;

		printf("sapients: %s \n", np->word);
		hashval = hash(word);

		// i want count the occurrences of a word.
		np->value = 0 + value;

		np->next = (struct haselement *)hashtab[hashval];

		hashtab[hashval] = np;
	}
	else
		// i want count the occurrences of a word.
		np->value = np->value + value;


	return np;
}
