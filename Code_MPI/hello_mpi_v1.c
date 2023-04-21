#include<stdio.h>
#include<mpi.h>

int main(int argc, char* argv[]){

	int num_proc, my_id;

	// Start MPI
	MPI_Init( &argc, &argv ); 

	// Get number of processes and ID
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc); 
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id); 

	// Some work by each process
	printf("Hello from Process %d!\n", my_id);
	printf("Hello  %d!\n", my_id);
	printf("Hello Process %d!\n", my_id);

	// End MPI
	MPI_Finalize(); 
	
	return 0;
}
