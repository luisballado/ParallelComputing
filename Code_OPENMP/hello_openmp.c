#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void hello(){
	int thread_id = omp_get_thread_num();
	int n_threads = omp_get_num_threads();
	printf("Hello from thread %d of %d\n", thread_id, n_threads);
}

int main(int argc, char* argv[]){

	// Determine number of threads
	int n_threads;
	sscanf(argv[1], "%d", &n_threads);

	// Execute some parallel code
#	pragma omp parallel num_threads(n_threads)
	hello();
	
	return 0;
}
