#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include <semaphore.h>
#include <fcntl.h>

int num_threads;
double pi = 0.0;
long long n;
sem_t* sem;
char sem_name[10];

void* parallel_pi_semaphore( void *id ){
	long thread_id = (long)id;
	double factor = 1.0, my_pi = 0.0;
	long long my_n = n / num_threads;
	long long my_first = my_n * thread_id;
	long long my_last = my_first + my_n;

	if (my_first % 2 != 0) factor = -1.0;

	for(long long i = my_first; i < my_last; i++, factor = -factor){
		my_pi += factor/(2.0*i+1.0);  
	}

	sem_wait(sem);
	pi += my_pi;
	sem_post(sem);

	return NULL;
}

int main(int argc, char* argv[]){

	//  Number of terms?
	sscanf(argv[1], "%lld", &n);

	//  Number of threads?
	sscanf(argv[2], "%d", &num_threads);

	// Allocating threads
	pthread_t* threads;
	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

	// Allocate semaphore and set name
	sem = (sem_t*)malloc(sizeof(sem_t));
	sprintf(sem_name, "/mysem");

	// Initialize semaphore
	sem = sem_open(sem_name, O_CREAT, 0777, 1);

	// Creating the threads (branching, forking)
	for(long t=0; t<num_threads; t++){
		pthread_create(&threads[t], NULL, parallel_pi_semaphore, (void*)t);
	}	

	// Terminate threads (join)
	for(long t=0; t<num_threads; t++){
		pthread_join(threads[t], NULL);
	}

	// Additional work in main thread
	pi *= 4.0;
	printf("                   PI = %.10f\n", 4.0*atan(1.0));
	printf("Estimated value of PI = %.10f\n", pi);

	// Destroy semaphore
	sem_unlink(sem_name);
    sem_close(sem);

	// Deallocate memory
	free(threads);

	return 0;
}
