
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char ** argv) {
//All declarations
  int i, index, size, prime, count, g, first;
  int n = atoi(argv[1]);
  if(n > 1000000){
  	exit(1);
  }
  int high;
   int low;
  int rank;
  int comm_size;
  char * hit;
  double t;
  //Initializing MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Barrier(MPI_COMM_WORLD);
  //Time taken
  t = -MPI_Wtime();
  //Start of each process
  low  = 2+(rank)*(n-1)/comm_size;
  //End of each process 
  high = 1 +(rank + 1)*(n - 1)/comm_size;
  //Jump size between process
  size = high - low + 1;
  //Necessary to use calloc here to take advantage of data being initialized to 0 
  //Only necessary for input sizes of 0
  hit = (char *) calloc(size, sizeof(char));
  if (hit == NULL) {
   MPI_Finalize();
   exit(1);
  }
  //Start the counter off 
  if (rank == 0){
  	index = 0;
  } 
  //Start at the first prime which is 2
  prime = 2;
  //Necessary to use do-while loop here because 
  do {
  	//Check to grab the first element 
    if (prime*prime > low) {
      first = prime*prime-low;
    } 
    else {
    	//Make sure that first element is not prime
      if ((low%prime) == 0){
      	first = 0;
      } 
      else{
      	first = prime - (low % prime);
      } 
    }
    //Load it up
    for (i = first; i < size; i += prime){
    	hit[i] = 1;
    }
    if (rank == 0) {
    	//Advance the first process
      while (hit[++index]);
      prime = index + 2;
    }
    if(comm_size > 1){
    	//send the primes to the rest of the processes 
    	MPI_Bcast(&prime,  1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    // go until the end of the problem size
  } while (prime*prime<=n);
  //get how many were found
  count = 0;
  for (i = 0; i < size; i++){
  	//print
  	if (hit[i] == 0){
  		count++;
  		printf("%d ", i+low);
  	} 

  }
  if (comm_size > 1) {
  	//get the global sum 
    MPI_Reduce(&count, &g, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  } 
  else {
    g = count;
  }
  
  t += MPI_Wtime();
  //print time 
  if (rank == 0) {
    printf("%d primes in %f secs.\n",g,t);
  }
  
  MPI_Finalize();
  return 0;
}