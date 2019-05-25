/*
 * main.c
 * Author: Armando Ferrara
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
#include <fileparser.h>
#include <hashmap.h>
#include <workerhelper.h>

int main (int argc, char *argv[])
{
    int my_rank ;
    int num_proc ; /* total number of processes */
    int total_number_of_byte; /* the total number of byte for the file */
    long chunk_size; /* Usual byte chunk size for file : total_number_of_byte/num_proc */
    short bonus; /* How many larger chunks, total_number_of_byte % num_proc */
    long start; /* Start for one of the chunk*/
    struct node * myPortion; /* my portion file to read */
    FILE *fp; /* reference to the file */
      my_rank = 0;
      num_proc = 1;
      start = 0;
    /* Code for process 0 (master)*/
    if(my_rank == 0)
    {

       // get the total number of bytes by the sum of all the byte size from files
       total_number_of_byte = getTotalFileSizes();
       // make a division for the number of process
       chunk_size =  total_number_of_byte / num_proc;
       // calculate the number of bonus byte that are not counted in chunk_size
       bonus = total_number_of_byte % num_proc; // i.e. remainder

       // from i=1 because i don't want to compute the work for the master
       for (short i = 1, start = chunk_size ; start < total_number_of_byte && i < num_proc ; i++)
       {
    	   if(bonus > 0)
    	   {
    		   int tmpChunkIncreased = chunk_size + 1;
    		   start = start + tmpChunkIncreased;
    		   bonus --;
    	   }
    	   else
    	   {
    		   start = start + chunk_size;
    	   }
       }

    	   // return a list of file that i need to analyze
    	   myPortion = whichIsMyPortion(0, 16);
          int i = 0;
          while(myPortion != NULL && myPortion->data != NULL )
          {

        	  printf("scusa:%s\n",myPortion->data);
        	  fp = fopen(myPortion->data,"r");
        	   if (fp == NULL)
        	   		      {
        	   		    	  perror("Error while opening the file.\n");
        	   		      }
	    	  buildfrequencieshash(fp);


        	       if(i == 0){
        	       fseek(fp, start, SEEK_SET);
        	       i++;
        	       }
        	       fclose(fp);

        	myPortion = myPortion->next;
          }
    	   // if there are other files to read
    		          // rationale, if the last worker stop to read in the middle of a word, it ends to read the word.
    		    	 // the next worker does not read the same word.


    }//the slaves



    /* display all the elements */
    /* lookup: search s in the hashtab */

    	struct hashelement *ptr = hashtab[0];

    	while(ptr != NULL)
    	{

        if(ptr != NULL && ptr->word!=NULL){
        printf("%s\n",ptr->word);
        ptr =(struct hashelement*) ptr->next;
        }
    	}


       	   /* display all the elements */
       	   printf("\n ok");
}
