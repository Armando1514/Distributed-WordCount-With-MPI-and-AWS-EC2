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



/*
 * whichIsMyPortion: assign the correct number of file to analyze
 */
struct node * whichIsMyPortion(long start, long chunkSize)
{
	struct node *ptr = getfilenamelist();
	char * pathString = ptr->data;
	long tempSize = ptr->size;

	//the file to analyze is the first
    if( tempSize > start)
    {
    	insertFirst(pathString,2);
		ptr = ptr->next;

    }
    else
    {
    	//scroll until you don't reach the file to analyze
    	while(ptr != NULL && tempSize < start)
    	{
    		pathString = ptr->data;
    		tempSize = tempSize + ptr->size;
    		ptr = ptr->next;


    	}

    	insertFirst(pathString,2);
    }
    //remove the bytes yet analyzed
    tempSize = tempSize - start;

    //we can need more than one file
    while(ptr != NULL && tempSize < chunkSize)
        	{
        		tempSize = tempSize + ptr->size;
            	insertFirst(ptr->data,2);
        		ptr = ptr->next;

        	}
    //remove (free) the list of all files
    freeAllTheList(1);

    return getHeader();
    }

void buildfrequencieshash(FILE* fp) {

	int ch;
	char str[500];
	int i = 0;


	while((ch = fgetc(fp)) != EOF )
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
			{

			//insert the word and increment the value
			if (insertwithincrement(str, 1) == NULL)
			printf("ERRORE");
			}
			//reset the string buffer
		    memset(str, 0, i+1);
		    i = 0;

		}

	}

	// Insert the last word
	//insert the word and increment the value
	if(i > 1)
		{
			if (insertwithincrement(str, 1) == NULL)
					printf("ERRORE");
		}
		//reset the string buffer
		memset(str, 0, i+1);
		i = 0;
	 	   fclose(fp);

}


