#include <stdio.h>
#include <string.h>
#include "mpi.h"
/*Excercise 2
The master send a number, the slave receive and increment it, after resend to a master the number incremented, the process is repeated until
the variable doesn't receive the value 5.
*/

int main(int argc, char* argv[])
{
    //Inizialize the MPI Environment
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Status error;
    short ball = 0;
    //Assign the rank of each node in the computation, usually 0 is for the master node
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //If the master node (rank 0)
    while (ball <= 5) {
        if (rank == 0) {

            //increment the number
            ball = ball + 1;

            //send the variable incremented
            MPI_Send(&ball, 1, MPI_SHORT, 1, 10, MPI_COMM_WORLD);
            //wait the value of ball incremented from the slave
            MPI_Recv(&ball, 1, MPI_SHORT, 1, 10, MPI_COMM_WORLD, &error);
            printf("The value is: %hd (process 0)\n", ball);
        }
        //The rank is 1, because i assumed  to have only 2 nodes
        else {

            //Recv the variable from the master and increment it
            MPI_Recv(&ball, 1, MPI_SHORT, 0, 10, MPI_COMM_WORLD, &error);
            printf("The value is: %hd (process 1)\n", ball);
            ball = ball + 1;
            //send the variable incremented to the master
            MPI_Send(&ball, 1, MPI_SHORT, 0, 10, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
