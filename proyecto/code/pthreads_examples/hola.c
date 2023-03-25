#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS 5

void *print_hola(void *threadid){

  long tid;
  tid = (long)threadid;
  printf("Hola mundo, hilo #%ld!\n",tid);
  pthread_exit(NULL);
  
}

int main(int argc, char *argv[]){

  pthread_t threads[NUM_THREADS];

  int rc;
  long t;

  for(t=0;t<NUM_THREADS;t++){

    printf("In main: creando el hilo %ld\n",t);

    rc = pthread_create(&threads[t],NULL,print_hola,(void *)t);
    
    if(rc){
      printf("ERROR; return code from pthread_create() is %d\n",rc);
      exit(-1);
    }
  }

  //Last thing that main() should do
  pthread_exit(NULL);

}
