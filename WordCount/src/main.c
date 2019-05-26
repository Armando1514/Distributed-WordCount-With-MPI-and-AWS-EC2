/*
 * main.c
 * Author: Armando Ferrara
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>

#include <workerhelper.h>

int main(int argc, char* argv[])
{
    int my_rank = 0;
    int num_proc = 1; /* total number of processes */
    int total_number_of_byte; /* the total number of byte for the file */
    long chunk_size; /* Usual byte chunk size for file : total_number_of_byte/num_proc */
    short bonus = 0; /* How many larger chunks, total_number_of_byte % num_proc */
    long start = 0; /* Start for one of the chunk */
    short i; /* loop variable*/

    /* Code for process 0 (master)*/
    if (my_rank == 0) {
        // get the total number of bytes by the sum of all the byte size from files
        total_number_of_byte = calculate_total_number_of_bytes();

        // make a division for the number of process
        chunk_size = total_number_of_byte / num_proc;
        // calculate the number of bonus byte that are not counted in chunk_size
        bonus = total_number_of_byte % num_proc; // i.e. remainder


        //calculate the word frequencies for the master
        calculate_word_frequencies(start, chunk_size);

        // from i=1 because i don't want to compute the work for the master
        for (i = 1, start = chunk_size; start < total_number_of_byte && i < num_proc; i++) {

        	if (bonus > 0) {
                int tmpChunkIncreased = chunk_size + 1;
                start = start + tmpChunkIncreased;
                bonus--;

            }
            else {
                start = start + chunk_size;

            }

        }

    }
    report(3);
    printf("\n ok");
}
