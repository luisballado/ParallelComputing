#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define MSGMAX 100

int num_threads;
char **messages;

void* msg_bw( void* id ){
	long thread_id = (long)id;
	long to = (thread_id + 1) % num_threads;
	long from = (thread_id + num_threads - 1) % num_threads;
	char* my_msg = (char*)malloc(sizeof(char)*MSGMAX);

	// Send message
	sprintf(my_msg, "Message from %ld to %ld", thread_id, to);
	messages[to] = my_msg;

	// Print message
	while( messages[thread_id] == NULL );
	printf("Thread %ld: %s\n", thread_id, messages[thread_id]);

	return NULL;
}

int main(int argc, char* argv[]){

	pthread_t* threads;

	// Determine number of threads
	sscanf(argv[1], "%d", &num_threads);

	// Allocating threads
	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

	// Allocating / initializing messages
	messages = (char**)malloc(sizeof(char*)*num_threads);
	for(long t=0; t<num_threads; t++) messages[t] = NULL;

	// Creating the threads (branching, forking)
	for(long t=0; t<num_threads; t++){
		pthread_create(&threads[t], NULL, msg_bw, (void*)t);
	}

	// Terminate threads (join)
	for(long t=0; t<num_threads; t++){
		pthread_join(threads[t], NULL);
	}

	// Deallocate memory
	free(threads);
	for(long t=0; t<num_threads; t++) free(messages[t]);
	free(messages);
	
	return 0;
}
