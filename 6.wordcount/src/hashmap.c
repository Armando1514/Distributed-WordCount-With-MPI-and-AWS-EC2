/*
 * hashmap.c
 * Implementation of a simple hashmap with function for insert and search.
 * Author: Armando Ferrara
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/hashmap.h"

/* __hash (char *s)
 * hash generator for string s*/
int __hash(char* s)
{

    long hash_val;
    int i = 0;

    for (hash_val = 0; s[i] != '\0'; s++) // for each character in s
    {
        hash_val = s[i] + 31 * hash_val; // calculate this number
        if((int)hash_val < 0)
           hash_val = hash_val * -1;
        i++;

    }

    return (int)hash_val % HASHSIZE;
}

/* report_hash_elements():
 * display the hash elements and
 * the computation timing */
void report_hash_elements(long test)
{
    struct hash_element* ptr;
    int i = 0;
    int total_word = 0;

    while (i < HASHSIZE) {
        ptr = hash_tab[i];
        while (ptr != NULL) // scan the list
        {
            total_word = total_word + ptr->value;
            ptr = (struct hash_element*)ptr->next;
        }
        i++;
    }

    printf("Total number of words analyzed: %d TEST ---- %ld \n",total_word,test);
}



/* struct hash_element * __lookup(char *s):
 * search s in the hashtab */
struct hash_element* __lookup(char* s)
{


    struct hash_element* ptr = hash_tab[__hash(s)];

    while (ptr != NULL ) {

        if (strcmp(s, ptr->word) == 0)
            return ptr;


        ptr = (struct hash_element*)ptr->next;
    }

    return NULL; //the word is not found
}

/*  free_hashmap():
 *  deallocate all the hashmap structure */
void free_hashmap()
{
    int i = 0;
    struct hash_element* ptr;
    struct hash_element* ptr_tmp;

    while (i < HASHSIZE) {
        ptr = hash_tab[i];
        while (ptr != NULL) // scan the list
        {
            // remove the memory corresponding to np
            ptr_tmp = ptr->next;

            free(ptr);

            ptr = ptr_tmp;
        }
        i++;
    }
}


/* __put_in_hashmap(char * data)
 * insert the element in the right position
 * into the hashmap
 */
void __put_in_hashmap(char* s, int value)
{


    //create a link
    struct hash_element* link = malloc(sizeof(struct hash_element));

    strcpy(link->word,s);

    link->value = value;

    //point it to old first node
    link->next = (struct hash_element*)hash_tab[__hash(s)];

    //point first to new first node
    hash_tab[__hash(s)] = link;
}

struct hash_element* get_hash_head(){
    return hash_tab;
}

/* insert_or_increment(char *s):
 * if there is yet the node, increments the value,
 * if not, allocates a new node with value to 1. */
void insert_or_increment(char* s, int value)
{
    struct hash_element* np;

    if(s[0] != '\0')
    {
        // search the element
        if ((np = __lookup(s)) == NULL) {
            /* not found */
            __put_in_hashmap(s, value);

            // i want count the occurrences of a word.
        } else {
            // i want count the occurrences of a word.
            np->value = np->value + value;
        }
    }
}


