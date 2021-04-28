/*
	This program gets an integer input (N) from the user and generates a file from 1 to N
    of the prime values. 

	Author: Reema Amhaz
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int p;
    int my_rank;
    int range;
    double timer;
    int index;

    //initialize  mpi
    MPI_Init(&argc, &argv);
    // get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    // get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Barrier(MPI_COMM_WORLD);
    // if there aren't enough arguments given terminate the program
    if(argc != 2) 
    {
        if(my_rank == 0)
            printf("usage: %s <arg1>\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    //get the input
    range = atoi(argv[1]);
    // check if it's in range if not exit
    if (range > 1000000)
    {
        exit(1);
    }
    // track time
    timer = -MPI_Wtime();

    // start and end of the process
    floor = 2 + (my_rank)*(range-1/p)
    ceil = 1 + (my_rank + 1)*(range - 1)/p;
    // check between 
    s = ceil - floor + 1

    sieve = (char*) calloc(s, sizeof(char));

    if (sieve == NULL)
    {
        MPI_Finalize();
        exit(1);
    }

    if (my_rank == 0)
    {

    }
    
    MPI_Finalize();
}

