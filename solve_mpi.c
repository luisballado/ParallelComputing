#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>

#define MAX_MSG 100

int main(int argc, char **argv){
	int my_id, num_proc, run_number;
	char problem[ 100 ], proc_name[ MPI_MAX_PROCESSOR_NAME ];
	int namelen;
	char message[MAX_MSG];

	if( argc == 3 ){
		sscanf(argv[1], "%s", problem);
		sscanf(argv[2], "%d", &run_number);
	}else{
		printf("ERROR - Run program as follows:\n\n");
		printf("\t./solve_mpi PROBLEM RUN\n\n");
		exit(1);
	}

	// Start MPI
	MPI_Init( &argc, &argv );

	// Get some info about MPI
	MPI_Comm_rank( MPI_COMM_WORLD, &my_id );
	MPI_Comm_size( MPI_COMM_WORLD, &num_proc );
	MPI_Get_processor_name( proc_name, &namelen );

	// Parallel work
	if( my_id == 0 ){
		printf("PROBLEM: %s\n", problem);
		printf("RUN NUMBER: %d\n", run_number);
		printf("This is process %d of %d, running on %s\n", my_id, num_proc, proc_name);
		for( int p=1; p<num_proc; p++ ){
			MPI_Recv(message, MAX_MSG, MPI_CHAR, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%s\n", message);
		}
	}else{
		sprintf(message, "This is process %d of %d, running on %s", my_id, num_proc, proc_name);
		MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	
	// End MPI
	MPI_Finalize();

	return 0;
}
