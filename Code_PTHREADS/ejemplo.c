#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/time.h>

#define MAX_PROBLEM_NAME 100

// Global variable (accessible to all threads)
int num_threads;
char problem[ MAX_PROBLEM_NAME ];

////////////////////////////////////////////////////////////////////////////////
void* funcion( void* id ){
	long thread_id = (long)id;
	
	printf("Thread %ld of %d\n", thread_id, num_threads);
	usleep(2000000); // 2 segundos

	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){
  
  // Determine 
  if(argc != 3){
    printf("ERROR: Input parameters!!!\n");
    exit(1);
  }else{
    sscanf(argv[1], "%s", problem); // Number of threads
    sscanf(argv[2], "%d", &num_threads); // Number of threads
  }
  
  printf("PROBLEM: %s\n", problem);
  printf("NUMBER OF THREADS: %d\n", num_threads);
  
  // Allocating threads
  pthread_t* threads;
  threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
  
  /***************************************************/
  // Measure start time
  struct timeval t;
  gettimeofday(&t, NULL);
  double start = t.tv_sec * 1000 + t.tv_usec / 1000;
  /***************************************************/
  
  // Creating the threads (branching, forking)
  for(long t=0; t<num_threads; t++)
    pthread_create(&threads[t], NULL, funcion, (void*)t);
  
  // Do some work with main thread
  //hello((void*)-1);
  
  // Terminate threads (join)
  for(long t=0; t<num_threads; t++)
    pthread_join(threads[t], NULL);
  
  /***************************************************/
  // Measure end time
  gettimeofday(&t, NULL);
  double end = t.tv_sec * 1000 + t.tv_usec / 1000;
  /***************************************************/
  
  // Elapsed time
  printf("TIME: %.2lf\n", (end - start));
  
  // Deallocate memory
  free(threads);
  
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
