#include<stdio.h>
#include<string.h>
#include<mpi.h>

#define MAX_MSG 100

int main(int argc, char* argv[]){
	char message[MAX_MSG];
	int num_proc, my_id;

	// Start MPI
	MPI_Init( &argc, &argv ); 

	// Get number of processes and ID
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc); 
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id); 

	// Parallel work
	if( my_id == 0 ){
		printf("Hello from Process %d!\n", my_id);
		for( int p=1; p<num_proc; p++ ){
			MPI_Status stat;
			MPI_Recv(message, MAX_MSG, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
			printf("Message from process %d with tag %d: %s\n", stat.MPI_SOURCE, stat.MPI_TAG, message);

		}
	}else{
		sprintf(message, "Hello!");
		MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}

	// End MPI
	MPI_Finalize(); 
	
	return 0;
}
