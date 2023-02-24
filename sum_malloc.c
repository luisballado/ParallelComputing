#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// The sum computed by the background thread


//Thread function to generate sum of 0 to N
void* sum_runner(void* arg){

  long long *limit_ptr = (long long*) arg;
  long long limit = *limit_ptr;
  free(arg);

  long long sum = 0;
  
  for(long long i = 0; i<= limit; i++){
    sum += i;
  }

  //pass back data in dynamically allocated memory
  long long *answer = malloc(sizeof(*answer));

  *answer = sum;
  pthread_exit(answer);
  
}

int main(int argc, char **argv){

  if(argc < 2){
    printf("Usage: %s <num> \n",argv[0]);
    exit(-1);
  }

  long long *limit = malloc(sizeof(*limit));
  *limit = atoll(argv[1]);
  //long long limit = atoll(argv[1]);

  // Thread ID
  pthread_t tid;

  // Create attributes
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  
  //No se le ponen () ya que se pasa la funcion pointer
  pthread_create(&tid,&attr,sum_runner,limit);

  long long *result;
  //Wait until thread is done its work
  pthread_join(tid,(void**)&result);
  
  printf("Sum is %lld\n",*result);
  free(result);
}
