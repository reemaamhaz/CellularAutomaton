/*
	This program gets an integer input (N) from the user and generates a file from 1 to N
    of the prime values. 

	Author: Reema Amhaz
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <mpi.h>


int main(int argc, char* argv[]) {
    int p;
    int my_rank;
    int range;
    double timer;
    int index;
    int next;

    //initialize  mpi
    MPI_Init(&argc, &argv);
    // get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    // get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Barrier(MPI_COMM_WORLD);
    //if there aren't enough arguments given terminate the program
    if(argc != 2) 
    {
        if(my_rank == 0)
            printf("usage: %s <arg1>\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    FILE *output;
    char ext[5] = ".txt";
    char *name = strcat(argv[1],ext);
    printf("%s", name);
    output = fopen(name, "w");

    //get the input
    range = atoi(argv[1]);
    // check if it's in range if not exit
    if (range > 10000000)
    {
        printf("That value is out of range.\n");
        exit(1);
    }

    // track time
    timer = -MPI_Wtime();
    
    if (p > 0)
    {
        // section off the work depending on the amount of processes
        int block = 0;
        int first = 0;
        int last = 0;
        block = floor((range+1)/p);
        if((range+1) % p > 0)
        {
            block += 1;
        }

        first = my_rank * block;
        last = first + block;
        
        for (int i = first; i < last + 1; i++)
        {
            if (i < (range+1))
            {
                for (int j = first; j < (last + 2)/2; j++)
                {
                    if (i != j && i % j == 0)
                    {
                        break;
                    }
                    else
                    {
                        fprintf(output, "%d ",i);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 2; i < range + 1; i++)
        {
            if (i < (range+1))s
            {
                for (int j = 2; j < (range + 2)/2; j++)
                {
                    if (i != j && i % j == 0)
                    {
                        break;
                    }
                    else
                    {
                        fprintf(output, "%d ",i);
                        break;
                    }
                }
            }
        }
    }
    
    fclose(output);
    MPI_Finalize();
    print(timer);
    return 0;
}