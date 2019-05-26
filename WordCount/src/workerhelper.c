/*
 * workerhelper.c
 *      Author: Armando Ferrara
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <workerhelper.h>
#include <fileparser.h>
#include <hashmap.h>
#define MAXWORDLENGTH 500


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

        // the file to analyze is the first
        if(tempSize > start)
        {
	           printf("a4:%s\n",current->data);

        	next = current->next;
        	pathString = current->data;

        	current = next;
        }
        else{
        	tempSize = 0;
    	//scroll until you don't reach the file to analyze
    	while(current != NULL && tempSize < start)
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

    	}

        }
    //remove the bytes yet analyzed
    tempSize = tempSize - start;

    //we can need more than one file, if we need of more bytes.
    while(current != NULL && tempSize < chunkSize)
        {
    	printf("a1:%s\n",current->data);
	         tempSize = tempSize + current->size;
    	     next = current->next;
    	     current = next;


        }

    //remove the other elements in the list after the current node
    if(current != NULL)
        	{

    	           next = current->next;

    	           current->data = NULL;
    	    	   current = next;

        	}


    return getfilenamelist();

    }


long buildfrequencieshash(FILE* fp, long wordToCount, long wordCounted) {


	    int ch;
		char str[MAXWORDLENGTH];
		memset(str, 0, MAXWORDLENGTH);
		int i = 0;


		//if the last slave read your word, ignore the word until the next non alphanumeric value
		if(ftell(fp) != 0)
		{

			if(isalnum(ch = fgetc(fp)) )
			{

				wordCounted ++;


				// read until there are character to read
				while(isalnum(ch = fgetc(fp)))
				{

					wordCounted ++;

				}

			}
		}



	while(  wordToCount > wordCounted && (ch = fgetc(fp)) != EOF)
	{

		// if is not a space and is not a punctuation and is alphanumeric
		if(isalnum(ch))
		{
            // convert the words to uppercase
			if (ch > 96 && ch < 123)
				ch -= 32;

			str[i] = ch;

			i++;

			if(i > MAXWORDLENGTH)
						{
							memset(str, 0, 500);
							i = 0;

						//ignore the word
						while(isalnum(ch = fgetc(fp)))
							{

							 wordCounted ++;

							}
						}
		}
		else
		{

			// a word is composed min by two characters
			if(i > 1)
			//insert the word and increment the value
			insertwithincrement(str);

			//reset the string buffer
		    memset(str, 0, i+1);
		    i = 0;

		}
		wordCounted ++;

	}

//insert the last word
if(ch == EOF && i > 1)
{
	insertwithincrement(str);

}
		//if the reading is stopped during the scan of a word, finish to read it
			if(i > 0 && isalnum(ch))
			{
				// read until there are character to read
				while(isalnum(ch = fgetc(fp)))
				{
					if (ch > 96 && ch < 123)
									ch -= 32;
				 str[i] = ch;
				 if(i > MAXWORDLENGTH)
				 {
				 				memset(str, 0, 500);
				 				i = 0;

				 			//ignore the word
				 			while(isalnum(ch = fgetc(fp)))
				 				{

				 				 wordCounted ++;

				 				}
				 }
				 i++;
				 // for statistic use
		         wordCounted ++;
				}

				insertwithincrement(str);
			}

			return wordCounted;
}


void calculatewordfrequencies(long start, long chunk_size)
{
    FILE *fp; /* reference to the file */

    struct node * myPortion = whichIsMyPortion(start, chunk_size); /* my portion file to read */

    int i = 0;

    long wordCounted = 0;

	while(myPortion != NULL && myPortion->data != NULL )
	{
		if(wordCounted > chunk_size)
		{
			perror("likely an error at line 242\n");
		}
		fp = fopen(myPortion->data,"r");

		  if(i == 0)
		  {
		  	fseek(fp, start, SEEK_SET);
		  	i++;
		  }

          //start to count the word in the file
		  wordCounted = wordCounted + buildfrequencieshash(fp, chunk_size, wordCounted);


	    fclose(fp);

		myPortion = myPortion->next;
	}


}



