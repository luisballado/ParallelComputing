#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<pthread.h>

#define MAX_MSG 100

int num_threads, num_proc, proc_id, namelen;
char proc_name[ MPI_MAX_PROCESSOR_NAME ];

char **message;

////////////////////////////////////////////////////////////////////////////////
void* write_message( void* id ){
	long thread_id = (long)id;
	sprintf(message[thread_id], "Process %d - Theread %ld - %s\n", proc_id, thread_id, proc_name);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

	// Start MPI
	MPI_Init( &argc, &argv ); 

	// Get number of processes and ID
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc); 
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id); 
	MPI_Get_processor_name( proc_name, &namelen );

	// Create threads and do parallel work
	// Each MPI process creates its own group of threads
	sscanf(argv[1], "%d", &num_threads);
	message = (char**)malloc(sizeof(char*)*num_threads);
	for(int i=0; i<num_threads; i++)
		message[i] = (char*)malloc(sizeof(char)*MAX_MSG);

	pthread_t* threads;
	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

	for(long t=0; t<num_threads; t++)
		pthread_create(&threads[t], NULL, write_message, (void*)t);

	for(long t=0; t<num_threads; t++)
		pthread_join(threads[t], NULL);

	free(threads);

	// Communicate results between processes
	if( proc_id == 0 ){
		
		// Print messages of Process 0 threads
		for(int i=0; i<num_threads; i++)
			printf("%s\n", message[i]);

		// Receive and print messages from all other processes
		char msg_temp[MAX_MSG];
		for( int p=1; p<num_proc; p++ ){
			for(int i=0; i<num_threads; i++){
				MPI_Recv(msg_temp, MAX_MSG, MPI_CHAR, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				printf("%s\n", msg_temp);
			}
		}

	}else{

		// Send all messages to Process 0
		for(int i=0; i<num_threads; i++)
			MPI_Send(message[i], strlen(message[i])+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

	}

	// Free memory
	for(int i=0; i<num_threads; i++) free(message[i]);
	free(message);

	// End MPI
	MPI_Finalize(); 
	
	return 0;
}
