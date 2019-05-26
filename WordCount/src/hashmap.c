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
#include <fileparser.h>



/* hash generator for string s*/
int hash (char *s)
{
	long hashval;
	for (hashval = 0; *s != '\0'; s++) // for each character in s
		hashval = *s + 31 * hashval; // calculate this number
	return(int)hashval % HASHSIZE;

}

/* display: display the hash elements */
void displayhashelement()
{
	struct hashelement *ptr;
	int i = 0;
	while (i < HASHSIZE)
		{
		   ptr = hashtab[i];
		   while(ptr != NULL)// scan the list
		  {

			printf("36. word: %s, occurrence/s: %d \n",ptr->word, ptr->value);
			ptr =(struct hashelement*) ptr->next;

		  }
		  i++;
		}
}




/* lookup: search s in the hashtab */
struct hashelement *lookup(char *s)
{
	struct hashelement *ptr = hashtab[hash(s)];

		while(ptr != NULL)
			{
			if( strcmp(s,ptr->word) == 0)
				return ptr;
			ptr =(struct hashelement*) ptr->next;

	       }


	return NULL; //the word is not found
}

/* freeHashmap: deallocate all the hashmap structure */
void freeHashmap()
{
	int i = 0;
	struct hashelement *ptr;
	struct hashelement *ptrtmp;

	while (i < HASHSIZE)
	{
	ptr = hashtab[i];
	   while(ptr != NULL)// scan the list
	  {

		 // remove the memory corresponding to np
  		if(ptr->next  != NULL)
  		ptrtmp = ptr->next;

  		free(ptr);
		free(ptr->word);

  		if(ptrtmp  != NULL)
  		ptr = ptrtmp->next;
  		else ptr= NULL;

	  }
	  i++;
	}
}


/* insert: define the function for insert the word and the value in the hashtab */
void insertwithincrement(char *word)
{
	struct hashelement *np;

	if((np = lookup(word)) == NULL)
	{
		/* not found */
		putinhashmap(word);

		// i want count the occurrences of a word.

	}
	else{
		// i want count the occurrences of a word.
		np->value = np->value + 1;

	}


}




void putinhashmap(char *data)
{

	//create a link
	struct hashelement *link = (struct hashelement*) malloc(sizeof(struct hashelement*));

    link->word = strdup(data);

    link->value = 1;

	//point it to old first node
	link->next =(struct hashelement *) hashtab[hash(data)];

	//point first to new first node
	hashtab[hash(data)] = link;


}
