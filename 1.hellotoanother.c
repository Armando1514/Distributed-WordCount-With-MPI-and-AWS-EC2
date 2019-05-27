#include <stdio.h>
#include <string.h>
#include "mpi.h"
/*Excercise 1
The master send a message to one node, and that node print a hello + message
*/
int main(int argc, char* argv[])
{
    //Inizialize the MPI Environment
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Status error;
    char message[8];
    //Assign the rank of each node in the computation, usually 0 is for the master node
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //If the master node (rank 0)
    if (rank == 0) { //Send the message "Armando"(length 7) to the  node with rank 1
        MPI_Send("Armando", 7, MPI_CHAR, 1, 10, MPI_COMM_WORLD);
    }
    //The rank is 1, because i assumed  to have only 2 nodes
    else {
        //Recive the message from master, if i'm the slave
        MPI_Recv(message, 7, MPI_CHAR, 0, 10, MPI_COMM_WORLD, &error);
        printf("Hello %s", message);
    }
    MPI_Finalize();
    return 0;
}
