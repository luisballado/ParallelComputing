#include<stdio.h>
#include<stdlib.h>

#ifdef _OPENMP
	#include<omp.h>
#endif

void hello(){
	#ifdef _OPENMP
		int thread_id = omp_get_thread_num();
		int n_threads = omp_get_num_threads();
	#else
		int thread_id = 0;
		int n_threads = 1;
	#endif

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
