/*
	This program is the Cellular Automaton written in OpenMP using parallelism. It takes input from 
    the user and generates a new matrix for each generation requested based on each cell's status 
    and their neighbors in the previous generation. 

	Author: Reema Amhaz
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

/*
    This function checks the surrounding neighbors to determine the status 
    of the cell in the follwing generation.
    Parameters: the current matrix, the row position, column position, and the dimension of the matrix
    Return value: an int representingthe status of the cell in the next generation
*/
int check_neighbors(int *current, int a, int b, int dimension)
{
    // calculate how many alive neighbors there are
    	int alive = 0;
    	int cell = current[a * dimension + b];
	int left = current[a * dimension +((b-1+dimension)%dimension)]; //left cell
	int right = current[a * dimension +((b+1+dimension)%dimension)]; //right cell
	int top = current[(((a - 1 + dimension)%dimension)* dimension)+ b]; // top of cell
	int bottom = current[((a + 1+ dimension)% dimension)* dimension + b]; // bottom of cell
	int tl =current[(((a - 1+ dimension)%dimension)*dimension)+((b-1+ dimension)% dimension)]; //top left cell
	int tr = current[(((a - 1+ dimension)%dimension)*dimension)+((b+1 + dimension)% dimension)]; // top right cell
	int bl = current[((a +1 +dimension)%dimension)*dimension+((b-1 + dimension)% dimension)]; //bottom left cell
	int br = current[((a + 1+ dimension)%dimension)* dimension+((b+1 + dimension)% dimension)]; // bottom right cell
	alive = left + right + top +bottom + tl + tr + bl + br;

	if(alive < 2 || alive > 3) 
    {
        return 0;
    }
 	else if(cell == 1 && (alive == 2 || alive == 3))
    {
        return 1;
    }
	else if(cell == 0 && alive == 3)
    {
        return 1;
    }
	else return 0;
}

/*
    This function just updates the matrix to the new generation.
    Parameters: the previous matrix, the new matrix, and the dimension of the matrix
    Return value: none
*/
void update_matrix(int *curr, int *next, int dimension)
{
    for(int i = 0; i < dimension*dimension; i++)
    {
        curr[i] = next[i];
    }
}

/*
    This main function is responsible for taking in the command line arguments and calling the neighbor function 
    and update matrix function. It harnesses OpenMP's parallelism to maximize runtime.
    Parameters: commandline arguments
    Return value: integer which varies if there is erroneous input
*/
int main(int argc, char* argv[]) 
{
    double start = 0.0;
    double end = 0.0;
    //check if they have all the input needed 
    if (argc != 5)
    {
        printf("This program requires number of generations, matrix dimension, # of threads, and input file as input.\n");
        return -1;
    }
    // get the number of generations
    int generations = 0;
    generations = atoi(argv[1]);

    // get the dimension
    int dimension = 0;
    dimension = atoi(argv[2]);

    // get the number of threads from the user 
    int threads = 0;
    threads = strtol(argv[3], NULL, 10);
    if (threads < 1)
    {
        threads = 1;
    }

    // get the intial input file and create outputfile
    FILE *input;

    input = fopen(argv[4], "r");
    if (input == NULL)
    {
        printf("Error: Cannot read the file.\n");
        exit(0);
    }

    // create the file name to write to
    char ext[5] = ".out";
    char *name = strcat(argv[4],ext);
    FILE *output;
    output = fopen(name, "w");

    int size = dimension*dimension;
    int current_matrix[size];
    fprintf(output, "Generation: 0 \n");
    
    int current = 0;
    int iterations = 0;
    int begin = 0;
    int finish = 0;
    int new_matrix[size];

    //read the first board into a matrix
    int k = 0;
    int elements = 0;
    while(fscanf(input, "%d", &elements) == 1)
    {
        current_matrix[k] = elements;
        k++;
    }
    fclose(input);

    for(int x = 0; x < dimension; ++x)
    {
        fprintf(output, "\n");
        for(int y = 0; y < dimension; ++y)
        {
            fprintf(output, "%d ", current_matrix[x*dimension+y]);
        }
    }
    

    // sets the number of threads
    omp_set_num_threads(threads);
    start = omp_get_wtime(); 
    
    for (int i = 0; i < generations; i++)
    {
        # pragma omp parallel num_threads(threads)
        {
            current = omp_get_thread_num();
            // see how many iterations each thread will have to do
            iterations = dimension/threads;
            if (iterations < 1)
            {
                //too many threads, then just set it to 1 for each dimension
                threads = dimension;
                iterations = dimension/threads;
            }
            //mark the beginning and end of each thread's work 
            begin = current * iterations;
            if (current == threads - 1)
            { 
                finish = dimension;
            }
            else
            {
                finish = begin + iterations;
            }
            
            for (int i = begin; i < finish; i++)
            {
                for (int j = 0; j < dimension; j++)
                {
                    new_matrix[i * dimension + j] = check_neighbors(current_matrix, i, j, dimension);
                }
            }
        }
        update_matrix(current_matrix, new_matrix, dimension);
        end = omp_get_wtime();
        // print the output
        fprintf(output, "\n");
        fprintf(output, "\nGeneration: %d \n", i+1);
        for(int x = 0; x < dimension; ++x)
        {
            fprintf(output, "\n");
            for(int y = 0; y < dimension; ++y)
            {
                fprintf(output, "%d ", current_matrix[x*dimension+y]);
            }
        }
    }
     

    printf("\nRuntime: %f seconds\n", end - start);

    fclose(output);

    return 0;
}
