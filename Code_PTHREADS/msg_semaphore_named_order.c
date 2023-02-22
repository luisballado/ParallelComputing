#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <fcntl.h>

#define MSGMAX 100

int num_threads;
sem_t** sem;
char** sem_name;

void* msg_semaphore( void* id ){
	long thread_id = (long)id;
	long to = (thread_id + 1) % num_threads;
	long from = (thread_id + num_threads - 1) % num_threads;
	char* my_msg = (char*)malloc(sizeof(char)*MSGMAX);

	// Send message
	sprintf(my_msg, "Message from %ld", thread_id);

	// Print message
	sem_wait(sem[thread_id]);
	printf("%s\n", my_msg);
	sem_post(sem[to]);

	free(my_msg);

	return NULL;
}

int main(int argc, char* argv[]){

	pthread_t* threads;

	// Determine number of threads
	sscanf(argv[1], "%d", &num_threads);

	// Allocating threads
	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

	// Allocating / initializing messages
	// messages = (char**)malloc(sizeof(char*)*num_threads);
	// for(long t=0; t<num_threads; t++) messages[t] = NULL;

	// Allocating / initializing semaphores
	sem = (sem_t**)malloc(sizeof(sem_t*)*num_threads);
	sem_name = (char**)malloc(sizeof(char*)*num_threads);
	for(long t=0; t<num_threads; t++){ 
		sem[t] = (sem_t*)malloc(sizeof(sem_t));
		sem_name[t] = (char*)malloc(sizeof(char)*10);
		sprintf(sem_name[t], "/sem%ld", t);
		sem[t] = sem_open(sem_name[t], O_CREAT, 0777, 0);
	}

	// Creating the threads (branching, forking)
	for(long t=0; t<num_threads; t++){
		pthread_create(&threads[t], NULL, msg_semaphore, (void*)t);
	}

	sem_post(sem[0]);

	// Terminate threads (join)
	for(long t=0; t<num_threads; t++){
		pthread_join(threads[t], NULL);
	}

	// Destroy semaphpres
	for(long t=0; t<num_threads; t++){ 
		sem_unlink(sem_name[t]);
		sem_close(sem[t]);
	}

	// Deallocate memory
	free(threads);
	for(long t=0; t<num_threads; t++){
		// free(messages[t]);
		free(sem_name[t]);
	}
	// free(messages);
	free(sem_name);
	free(sem);
	
	return 0;
}
