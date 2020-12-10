
/*
 * main: the master sends the  portion of bytes to read, after computes its workload communicating with "workerhelper" ,
 * finally it waiting the results from the slaves(communicating with "mpihelper") for combine them.
 * The slaves after that has been received the portion to analyze,
 * they compute and send  to the master, the words/occurrences (communicatin with "mpihelper") through a defined struct (using "mpihelping").
 * Author: Armando Ferrara
 */

#include <stdio.h>
#include <string.h>

#include "mpi.h"

#include "../include/workerhelper.h"
#include "../include/mpihelper.h"

int main(int argc, char* argv[])
{

    /* variable used for mpi */
    int my_rank;
    int num_proc; /* total number of processes */
    double t1, t2; /* two variable for the time */
    /*variable used for mpi end */

    long total_number_of_byte; /* the total number of byte for the file */
    long chunk_size; /* Usual byte chunk size for file : total_number_of_byte/num_proc */
    short bonus = 0; /* How many larger chunks, total_number_of_byte % num_proc */
    long start = 0; /* Start for one of the chunk */
    short i; /* loop variable*/

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    /* position 0 the start, position 1 the byte size to analyze */
    long table[num_proc][2];
    long row[2];

    // get the total number of bytes by the sum of all the byte size from files
    total_number_of_byte = calculate_total_number_of_bytes();

    t1 = MPI_Wtime();

    if (my_rank == 0) {

        printf("number of processes: %d \n", num_proc);

        // make a division for the number of process
        chunk_size = total_number_of_byte / num_proc;

        // calculate the number of bonus byte that are not counted in chunk_size
        bonus = total_number_of_byte % num_proc; // i.e. remainder

        //bonus for the master

        for (i = 0, start = 0; start < total_number_of_byte && i < num_proc; i++) {

            if (bonus > 0) {

                start = start + chunk_size + 1;
                table[i][1] = chunk_size + 1;
                bonus--;
            }
            else {
                start = start + chunk_size;
                table[i][1] = chunk_size;
            }

            table[i + 1][0] = start;
        }

        //sends different start positions and chunk_size to each process
        MPI_Scatter(&table[0][0], 2, MPI_LONG,
                    &row[0], 2, MPI_LONG, 0, MPI_COMM_WORLD);

        //calculate the word frequencies for the master
        calculate_word_frequencies(0, row[1]);

        master_receiver(num_proc);

    } //the slaves
    else {

        MPI_Scatter(&table[0][0], 2, MPI_LONG,
                    &row[0], 2, MPI_LONG, 0, MPI_COMM_WORLD);

        calculate_word_frequencies(row[0], row[1]);

        worker_sender();
    }

    t2 = MPI_Wtime();

    //report the time and the words in a file
    report(t2 - t1, my_rank);

    //deallocate the hashmap and the list used for the filenames
    deallocate_the_lists();

    MPI_Finalize();

    return 0;
}
