#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// compile: mpicc -fopenmp hybrid_sr.c -o hybrid_sr
// run: mpiexec -n 2 ./hybrid_sr 

int main(int argc, char** argv) {
  /* Initialize MPI environment */
  MPI_Init(NULL, NULL);
  // Discover world rank and size
// rank
  int rank_of_world;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_of_world);
// size
  int size_of_world;
  MPI_Comm_size(MPI_COMM_WORLD, &size_of_world);

/* OMP parallel section segment - each section in the parallel sections
section is excecuted in parallel */

#pragma omp parallel sections
{
 #pragma omp section
   {
  // If more than 2 processes for this task
  if (size_of_world < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
}

#pragma omp section
   {
int number;
int a = 1;
int b = 2;
  if (rank_of_world == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    number = a + b;
int c = 6; 
number = c * number;
int d = 8;
number = number / d;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (rank_of_world == 1) {
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n", number);
  }
}
#pragma omp section
   {
  MPI_Finalize();
clock_t start, end;
 double cpu_time_used;
 start = clock();
 /* Do the work. */
 end = clock();
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("Time taken was for calculation:  %f secs. ", cpu_time_used);
}
}
}
