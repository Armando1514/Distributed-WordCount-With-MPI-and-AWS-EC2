/*
 * mpihelper.c
 * Author: Armando Ferrara
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"

#include "../include/workerhelper.h"
#include "../include/hashmap.h"
#include "../include/mpihelper.h"




struct send_structure // entry table
{

    int value[WORDONTHENETWORK]; // define the word
    char word[WORDONTHENETWORK][MAXWORDLENGTH]; // define the word

};


MPI_Datatype*  __define_mpi_type()
{

    /*DEFINE A DERIVED MPI TYPE */

    int blocklengths[2] = {WORDONTHENETWORK , WORDONTHENETWORK * MAXWORDLENGTH  };
    MPI_Datatype types[2]={MPI_INT,MPI_CHAR};
    MPI_Aint displacements[2];
    MPI_Datatype  restype;
    MPI_Aint lb,intex;



    MPI_Type_get_extent(MPI_INT,&lb,&intex);


    displacements[0] = (MPI_Aint) 0;
    displacements[1] = (MPI_Aint) WORDONTHENETWORK;


    MPI_Type_create_struct(2,blocklengths,displacements,types,&restype);
    MPI_Type_commit(&restype);


return restype;

}


void master_receiver(int num_proc)
{



/* END DEFINITION  DERIVED STRUCTURE*/

/* TEST MY ALGORITHM  */


    MPI_Status status;


    MPI_Datatype restype;
    int i=0;
    int recv = 0;
    int sinc = 0;
    int occurrence = 0;
    // DELETE THE MASTER THAT RECEIVE
    num_proc--;
    struct send_structure * x  = malloc(sizeof(struct send_structure));


    restype = __define_mpi_type();

    while(num_proc) {

        //recv from anyone
        MPI_Recv(x, 1, restype, MPI_ANY_SOURCE, 50, MPI_COMM_WORLD, &status);


        if(x->word[0][0] == '\0'){

            num_proc --;
        }
        else {

            sinc = WORDONTHENETWORK - 1;
            while (sinc >= 0) {



                    insert_or_increment(x->word[sinc], x->value[sinc]);
                sinc = sinc - 1;
            }



        }
        //reset the array
        i = WORDONTHENETWORK;
        int l = 0;
        i--;
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



    printf("TIMEEEEEEEEEE: %lf , RECVVVVVVV : %d  OCCURRENCE : %d\n",recv,occurrence);


}



void worker_sender()
{

    struct send_structure * x= malloc(sizeof(struct send_structure));
    /*DEFINE A DERIVED MPI TYPE */
    MPI_Datatype restype;

    restype = __define_mpi_type();


    struct hash_element* ptr;
    int i = 0;
    int total_word = 0;
    int j = 0;
    long sended = 0;
    long sended2 = 0;
    while (i < HASHSIZE) {
        ptr = hash_tab[i];
        while (ptr != NULL) // scan the list
        {
            if(j < WORDONTHENETWORK ) {


                strcpy(x->word[j], ptr->word);

                x->value[j] = ptr->value;

                sended = sended + ptr->value;
                j++;

                ptr = (struct hash_element *) ptr->next;

            }

            //send
            if(j == (WORDONTHENETWORK ) ) {



                MPI_Ssend(x, 1, restype, 0, 50, MPI_COMM_WORLD);

                int l = 0;
                 j--;
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
    if((j != (WORDONTHENETWORK ) ) && j > 0) {

        MPI_Send(x, 1, restype, 0, 50, MPI_COMM_WORLD);

    }

    x->word[0][0] = '\0';

    MPI_Ssend(x, 1, restype, 0, 50, MPI_COMM_WORLD);




  free(x);

}





