#include <mpi.h>
#include <iostream>

void round_robin(int rank, int procs){
  long int rand_mine, rand_prev;
  int rank_next = (rank + 1) % procs;
  int rank_prev = rank == 0 ? procs - 1: rank -1;
  MPI_Status status;

  srandom(time(NULL) + rank);
  rand_mine = random() / (RAND_MAX / 100);
  printf("%d: random is %ld\n", rank, rand_mine);
}

int main(int argc, char**argv){

  int num_procs;
  int rank;

  //si lo hace el num de nodos
  //printf("vamo a ver\n");
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("%d: hello (p=%d)\n", rank, num_procs);

  round_robin(rank,num_procs);

  printf("%d: goodbye\n", rank);

  MPI_Finalize();

  //si lo hace el num de nodos
  //printf("termine todo el peo\n");
  
  return 0;

}
