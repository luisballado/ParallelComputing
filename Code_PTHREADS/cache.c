#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define n 1000000000

// Global variables (accessible to all threads)
int num_threads;
double result = 0;
double* individual_result;

long flag = 0;

////////////////////////////////////////////////////////////////////////////////
void* sum( void* id ){
	long thread_id = (long)id;

	// Version 1
	//for(long long i = 1; i<=n; i++){
	//  result += i;
        //}

	// // Version 2
	// individual_result[ thread_id ] = 0;
	// for(long long i = 1; i<=n; i++){
	// 	individual_result[ thread_id ] += i;
	// }

	// Version 3
	double myresult = 0;
	for(long long i = 1; i<=n; i++){
	  myresult += i;
	}
	
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

	// Determine number of threads
	sscanf(argv[1], "%d", &num_threads);

	// Allocating memory to store results
	individual_result = (double*)malloc(sizeof(double)*num_threads);

	// Allocating threads
	pthread_t* threads;
	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

	// Creating the threads (branching, forking)
	for(long t=0; t<num_threads; t++)
		pthread_create(&threads[t], NULL, sum, (void*)t);

	// Terminate threads (join)
	for(long t=0; t<num_threads; t++)
		pthread_join(threads[t], NULL);

	// Deallocate memory
	free(threads);
	free(individual_result);
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
