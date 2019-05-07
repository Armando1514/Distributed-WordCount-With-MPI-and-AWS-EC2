#include <stdio.h>
#include <string.h>
#include "mpi.h"
/*Excercise 3
The master send a number, the slave receive and increment it, after it sends the number incremented to another slave that repeat the process (slave i, send a number incremented to the slave i+1). The last slave, send the number to the master for close the circle.
*/

int main(int argc, char *argv[])
{
    //Inizialize the MPI Environment
    MPI_Init(&argc,&argv);
    int rank;
    MPI_Status error;
    short  telephone=0;
    //Assign the rank of each node in the computation, usually 0 is for the master node
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    //If the master node (rank 0)
        if(rank==0)
        {
                            
                //send the variable to the slave
                MPI_Send(&telephone,1,MPI_SHORT,1,10,MPI_COMM_WORLD);
                //wait the value of ball incremented from the slave
                MPI_Recv(&telephone,1,MPI_SHORT,3,10,MPI_COMM_WORLD,&error);
                printf("I'm the master, the last value is: %hd (i received it from slave with rank 3)\n",telephone); 
              
        
         }
        //The slave with rank 1
        else if(rank==1)
         {
                
                //Recv the variable from the master and increment it
                MPI_Recv(&telephone,1,MPI_SHORT,0,10,MPI_COMM_WORLD,&error);
                printf("I'm the process with rank 1, i received: %hd (from process 0)\n",telephone);
                telephone=telephone+1;
                //send the variable incremented to the master
                MPI_Send(&telephone,1,MPI_SHORT,2,10,MPI_COMM_WORLD);
         }      
        else if(rank==2)
         {
                
                //Recv the variable from the master and increment it
                MPI_Recv(&telephone,1,MPI_SHORT,1,10,MPI_COMM_WORLD,&error);
                printf("I'm the process with rank 2, i received: %hd (from process 1)\n",telephone);
                telephone=telephone+1;
                //send the variable incremented to the master
                MPI_Send(&telephone,1,MPI_SHORT,3,10,MPI_COMM_WORLD);
         }      
        else 
         {
                
                //Recv the variable from the master and increment it
                MPI_Recv(&telephone,1,MPI_SHORT,2,10,MPI_COMM_WORLD,&error);
                printf("I'm the process with rank 3, i received: %hd (from process 2)\n",telephone);
                telephone=telephone+1;
                //send the variable incremented to the master
                MPI_Send(&telephone,1,MPI_SHORT,0,10,MPI_COMM_WORLD);
         }
    MPI_Finalize();
    return 0;
}
