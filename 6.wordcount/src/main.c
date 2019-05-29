/*
 * main.c
 * Author: Armando Ferrara
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "mpi.h"

#include "../include/workerhelper.h"
#include "../include/hashmap.h"

#define WORDONTHENETWORK 10000








int main(int argc, char* argv[])
{
    struct send_structure // entry table
    {

        int value[WORDONTHENETWORK]; // define the word
        char word[WORDONTHENETWORK][MAXWORDLENGTH]; // define the word

    };


    /* variable used for mpi */
    int my_rank ;
    int num_proc ; /* total number of processes */
    double t1, t2; /* two variable for the time */
    MPI_Status status; /* status for receive */
    /*variable used for mpi end */

    long total_number_of_byte; /* the total number of byte for the file */
    long chunk_size; /* Usual byte chunk size for file : total_number_of_byte/num_proc */
    short bonus = 0; /* How many larger chunks, total_number_of_byte % num_proc */
    long start = 0; /* Start for one of the chunk */
    short i=2; /* loop variable*/

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    /*DEFINE A DERIVED MPI TYPE */

    int blocklengths[2] = {WORDONTHENETWORK   , WORDONTHENETWORK*MAXWORDLENGTH  };
    MPI_Datatype types[2]={MPI_INT,MPI_CHAR};
    MPI_Aint displacements[2];
    MPI_Datatype  restype;
    MPI_Aint lb,intex;



    MPI_Type_get_extent(MPI_INT,&lb,&intex);


    displacements[0] = (MPI_Aint) 0;
    displacements[1] = (MPI_Aint) WORDONTHENETWORK;


    MPI_Type_create_struct(2,blocklengths,displacements,types,&restype);
    MPI_Type_commit(&restype);


/* END DEFINITION  DERIVED STRUCTURE*/




    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);





    int flags = 1;



    /* example */
    long table[num_proc][2];
    long row[2];
    /* end example */
    /* Startup task */

    // get the total number of bytes by the sum of all the byte size from files
    total_number_of_byte = calculate_total_number_of_bytes();
i=0;
    /* Code for process 0 (master)*/
    struct send_structure * x= malloc(sizeof(struct send_structure));


    if (my_rank == 0) {


        t1 = MPI_Wtime();


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

            } else {
                start = start + chunk_size;
                table[i][1] = chunk_size;
            }

            table[i + 1][0] = start;

        }

        //sends different start positions and chunk_size to each process
        MPI_Scatter(&table[0][0], 2, MPI_LONG,
                    &row[0] , 2, MPI_LONG, 0, MPI_COMM_WORLD);
        //calculate the word frequencies for the master
        calculate_word_frequencies(0, row[1]);

/* TEST MY ALGORITHM  */
 int flag;
 int nproc = num_proc;
 int j = 0;
 short is_the_last = 1;

 j= 0;


        MPI_Request request;
        i=0;
flags=1;
        int recv = 0;
int sinc = 0;
        int occurrence = 0;
        // DELETE THE MASTER THAT RECEIVE
        num_proc--;
        report(4);

        while(num_proc) {

                 //recv from anyone
                 MPI_Recv(x, 1, restype, MPI_ANY_SOURCE, 50, MPI_COMM_WORLD, &status);


            if(x->word[0][0] == '\0'){

                num_proc --;
            }
            else {


                sinc = WORDONTHENETWORK - 1;
                while (sinc >= 0) {


                    if (x->word[sinc][0] != '\0') {

                        occurrence = occurrence + x->value[sinc];

                    }
                    sinc = sinc - 1;
                }



            }
            //reset the array
            i = WORDONTHENETWORK - 1;
            int l = 0;

            while (i >= 0 &&  l < MAXWORDLENGTH) {

                l = 0;

                while (x->word[i][l] != '\0' &&  i < WORDONTHENETWORK && l < MAXWORDLENGTH) {
                    x->word[i][l] = '\0';

                    l++;
                }
                x->value[i] = 0;
                i--;

            }
         }





        t2 = MPI_Wtime();

        printf("TIMEEEEEEEEEE: %lf , RECVVVVVVV : %d  OCCURRENCE : %d\n",t2-t1,recv,occurrence);

    }//the slaves
    else {

        MPI_Scatter(&table[0][0], 2, MPI_LONG,
                    &row[0], 2, MPI_LONG, 0, MPI_COMM_WORLD);

        calculate_word_frequencies(row[0], row[1]);
        report(3);
        struct hash_element* ptr;
        int i = 0;
        int total_word = 0;
        int j = 0;
       long sended = 0;

        while (i < HASHSIZE) {
            ptr = hash_tab[i];
            while (ptr != NULL) // scan the list
            {
                if(j < WORDONTHENETWORK  ) {


                        strcpy(x->word[j], ptr->word);

                        x->value[j] = ptr->value;

                        sended = sended + ptr->value;
                        j++;

                    ptr = (struct hash_element *) ptr->next;

                }
                //send
                if(j == (WORDONTHENETWORK ) ) {



                        MPI_Send(x, 1, restype, 0, 50, MPI_COMM_WORLD);

                        int l = 0;

                        while (j >= 0  ) {
                            l = 0;
                            while (x->word[j][l] != '\0' &&  j < WORDONTHENETWORK && l < MAXWORDLENGTH) {

                                x->word[j][l] = '\0';

                                l++;
                            }
                            x->value[j] = 0;
                            j--;
                    }
                j = 0;
                }

            }

            i++;

        }

        //NOT SENT
        if((j != (WORDONTHENETWORK - 1) ) && j > 0) {

            MPI_Send(x, 1, restype, 0, 50, MPI_COMM_WORLD);

        }

       x->word[0][0] = '\0';

        MPI_Ssend(x, 1, restype, 0, 50, MPI_COMM_WORLD);



    }
free(x);





    MPI_Finalize();
    return 0;

}

