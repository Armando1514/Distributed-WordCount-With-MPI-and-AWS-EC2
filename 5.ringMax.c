#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX_LEN 1000000 /* MAX ARRAY SIZE */
/* EXCERCISE 5
Given an array of n randomly generated integers, when n is given from terminal,
compute the maximum element using p processes. Each of them should work on
n/p + (1 optional)  elements.
The master computes its local max and sends it to the first slave. The 
slave computes its local max and compares it to the previously number received
from the master, compare it with its max, and send to the next slave the 
most higher value from the comparison. The last slave must send back the 
final max to the master. 
Estimate the whole running time . (look, the overhead is higher with more processors, you need a big matrix for obtain a good trade of between overhead and speed (SPEED UP)).
Note: if you use this program, you need to specify 1 (does it mean sequential comparison) and length of array, that must be less than MAX_LEN. E.G.
mpirun -np 4 --hostfile machinefile a.out 1 10000  */
/* search the maximum */
int find_max(int a[ ], int len
{
    int i;
    int max; /* current max */
    max = a[0];

    for (i=1; i<len; i++)
    {   

         if(a[i] > max)
           {
              max=a[i];
            }
    }
    return max;
}

/* Function to generate random ints */
void generate_data (int a[ ], int len)
{
    int i;
    struct timeval time;
    /* Use time of day to get a seed */
    gettimeofday(&time, (struct timezone *) 0 );
    srand((int) time.tv_sec);

    for (i=0;i<len;i++)
    {
        a[i]=rand();
    }
}

int main (int argc, char *argv[])
{
    double t1, t2; /* two variable for the time */
    int my_rank;
    int num_proc; /* total number of processes */
    int array_size; /* Length of the main array */
    int chunk_size; /* Usual subarray size: array_len/num_proc */
    int bonus; /* How many larger subarrays, array_len % num_proc */
    int start; /* Start of one of the subarray */
    int search_array[MAX_LEN]; /* array to search */
    int my_max; /*  max for my  subarray */
    int global_max; /* Max for the main array */
    int local_max; /* Local max from one process */
    int sequential; /* if is sequential or not the algorithm for the comparison */
    MPI_Status status; /* status for receive */
    /* Startup task */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
   /* Sequential 1 - Parallel !=1 */
    sequential = atoi(argv[1]);
    array_size = atoi(argv[2]);
    /* generate array with your input */
    generate_data(search_array,array_size);

    /* Code for process 0 */
    if(my_rank == 0)
    {
    t1=MPI_Wtime();


    /* calculate the result in sequential way for comparison */
    if (sequential)
    {

         printf("The sequential search gives %d\n", find_max(search_array,array_size));

    }


    chunk_size = array_size / num_proc;

    bonus = array_size % num_proc;  // i.e. remainder
    start = chunk_size;

    MPI_Send(&chunk_size, 1, MPI_INT, 1, 10, MPI_COMM_WORLD);
    MPI_Send(&(search_array[start]), chunk_size, MPI_INT, 1, 11, MPI_COMM_WORLD);
 /* send also an indication for the bonus number */
    MPI_Send(&bonus, 1, MPI_INT, 1, 12, MPI_COMM_WORLD);
    

    /* the master has to process the bonus element in array */
    if((array_size % num_proc) > 0) 
    {
    global_max = find_max(search_array, chunk_size + 1);
    }
    else
    {
    global_max = find_max(search_array, chunk_size);
    }
    
    MPI_Send(&global_max, 1, MPI_INT, 1, 13, MPI_COMM_WORLD);
    /* wait the global max from the last slave */
    MPI_Recv(&global_max, 1, MPI_INT, num_proc-1, 14, MPI_COMM_WORLD, &status);

    t2 = MPI_Wtime();
    printf("The Elapsed time for the execution is %f\n", t2 - t1 ); 
    printf("The parallel search, gives as result the number: %d\n",global_max);
    }
    /* The slaves receive the part of array and process it */
    else
    {

    MPI_Recv(&chunk_size, 1, MPI_INT, my_rank-1, 10, MPI_COMM_WORLD, &status);
    MPI_Recv(search_array, chunk_size, MPI_INT, my_rank-1, 11, MPI_COMM_WORLD, &status);
    MPI_Recv(&bonus, 1, MPI_INT, my_rank-1, 12, MPI_COMM_WORLD, &status);
    MPI_Recv(&global_max, 1, MPI_INT, my_rank-1, 13, MPI_COMM_WORLD, &status);
    /*find the local max in the subarray */
    

    if((bonus - my_rank) > 0)
	{
    	start=(chunk_size*(my_rank+1))+my_rank;
	local_max = find_max(search_array, chunk_size+1);


        }
    else 
	{     
   	 start=(chunk_size*(my_rank+1))+bonus;
	 local_max = find_max(search_array, chunk_size);
	}

    if(local_max>global_max)
	global_max=local_max;

    array_size = (num_proc*chunk_size) + bonus;
 
    if(start < array_size)
    {
         
        MPI_Send(&chunk_size, 1, MPI_INT, my_rank+1, 10, MPI_COMM_WORLD);
        MPI_Send(&(search_array[start]), chunk_size, MPI_INT, my_rank+1, 11, MPI_COMM_WORLD);
        MPI_Send(&bonus, 1, MPI_INT, my_rank+1, 12, MPI_COMM_WORLD);
   	MPI_Send(&global_max, 1, MPI_INT, my_rank+1, 13, MPI_COMM_WORLD);
	
    }
    else /*is the last slave, have to sends the max to the master */
    {
    MPI_Send(&global_max, 1, MPI_INT, 0, 14, MPI_COMM_WORLD);
    }
  }
    
    MPI_Finalize();
    return 0;
}
