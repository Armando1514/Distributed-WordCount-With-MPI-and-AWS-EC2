/*
 * workerhelper.c
 *      Author: Armando Ferrara
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <workerhelper.h>
#include <fileparser.h>
#include <hashmap.h>



/* Function to delete the entire linked list */
void deletealltheList(struct node* head_ref)
{
   /* deref head_ref to get the real head */
   struct node* current = head_ref;
   struct node* next;

   while (current != NULL)
   {
       next = current->next;
       free(current->data);
       free(current);
       current = next;
   }


}

/*
 * whichIsMyPortion: assign the correct number of file to analyze
 */
struct node * whichIsMyPortion(long start, long chunkSize)
{
	struct node *current = getfilenamelist();
	struct node* next;
	char * pathString ;
	long tempSize = current->size ;

        // the file to analyse is the first
        if(tempSize > start)
        {

        	next = current->next;
        	pathString = current->data;
        	current = next;
        	printf("1: %d\n",tempSize);
        }
        else{
        	tempSize = 0;
    	//scroll until you don't reach the file to analyze
    	while(current != NULL)
    	{
    		 next = current->next;

    		 pathString = current->data;
    		 tempSize = tempSize + current->size;
    		 // i don't need to analyze this file, i'm able to delete it.
    		 if(tempSize < start)
    		 {

    		 free(current->data);
    		 free(current);
    		 setHeader(next);
    		 }

             current = next;
         	printf("2: %d\n",tempSize);

    	}

        }
    //remove the bytes yet analyzed
    tempSize = tempSize - start;
	printf("tempsize 1: %d\n",tempSize);
	printf("start 1: %d\n",start);
	printf("chunk 1: %d\n",chunkSize);

    //we can need more than one file, if we need of more bytes.
    while(current != NULL && tempSize < chunkSize)
        {
	         tempSize = tempSize + current->size;
    	     next = current->next;
    	     current = next;
         	printf("3: %d\n",tempSize);

        }

    //remove (free) the other elements in the list after the current node
    while(current != NULL)
        	{


    	           next = current->next;
    	           free(current->data);
    	    	   free(current);

    	    	   current = next;

        	}

 	printf("4: %d\n",tempSize);



    return getfilenamelist();

    }


void buildfrequencieshash(FILE* fp) {


	int ch;
	char str[500];
	memset(str, 0, 500);

	int i = 0;


	while((ch = fgetc(fp)) != EOF)
	{

		// if is not a space and is not a punctuation and is alphanumeric
		if(isalnum(ch))
		{
            // convert the words to uppercase
			if (ch > 96 && ch < 123)
				ch -= 32;

			str[i] = ch;


			i++;
		}
		else
		{

			// a word is composed min by two characters
			if(i > 1)
			//insert the word and increment the value
			insertwithincrement(str, 1);

			//reset the string buffer
		    memset(str, 0, i+1);
		    i = 0;

		}


	}
	// Insert the last word
		//insert the word and increment the value
		if(i > 1)
			{
				insertwithincrement(str, 1);

			}
			//reset the string buffer
			memset(str, 0, i+1);
			i = 0;

}


