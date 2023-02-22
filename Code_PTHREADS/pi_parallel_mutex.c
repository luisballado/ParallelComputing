#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

int num_threads;
double pi = 0.0;
long long n;
pthread_mutex_t mutex;

void* parallel_pi_mutex( void *id ){
	long thread_id = (long)id;
	double factor = 1.0, my_pi = 0.0;
	long long my_n = n / num_threads;
	long long my_first = my_n * thread_id;
	long long my_last = my_first + my_n;

	if (my_first % 2 != 0) factor = -1.0;

	for(long long i = my_first; i < my_last; i++, factor = -factor){
		my_pi += factor/(2.0*i+1.0);  
	}

	pthread_mutex_lock(&mutex);
	pi += my_pi;
	pthread_mutex_unlock(&mutex);

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

	// Initialize mutex
	pthread_mutex_init(&mutex, NULL);

	// Creating the threads (branching, forking)
	for(long t=0; t<num_threads; t++){
		pthread_create(&threads[t], NULL, parallel_pi_mutex, (void*)t);
	}	

	// Terminate threads (join)
	for(long t=0; t<num_threads; t++){
		pthread_join(threads[t], NULL);
	}

	// Additional work in main thread
	pi *= 4.0;
	printf("                   PI = %.10f\n", 4.0*atan(1.0));
	printf("Estimated value of PI = %.10f\n", pi);

	// Destroy mutex
	pthread_mutex_destroy(&mutex);

	// Deallocate memory
	free(threads);

	return 0;
}
