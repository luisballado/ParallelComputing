#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//Complete the code of the parallel program for Matrix-Vector Multiplication
//The program will read the matrix A and vector x
//from input files

// I took the example at
// https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
// to make dynamically allocate 2D array

// Global variable (accessible to all threads)
int num_threads;
int m,n;

int* A; //matriz
int* x; //vector
int* y; //respuesta

void *mat_vect( void* id){

  long thread_id = (long)id;
  int local_m = m/num_threads;
  int my_first_row = thread_id * local_m;
  int my_last_row = (thread_id+1)*local_m;

  for(int i = my_first_row; i<=my_last_row; i++){
    y[i] = 0.0;
    for(int j=0;j<n;j++)
      y[i] += A[i*n+j] * x[j];
  }

  return NULL;
  
}


////////////////////////////////////////////////////////////////////////////////
void* hello( void* id ){
  long thread_id = (long)id;
  if(thread_id == -1) 
    printf("Hello from MAIN thread\n");
  else
    printf("Hello from thread %ld of %d\n", thread_id, num_threads);
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

  int i,j;
  int aux;
  //int arr[3][3];
  //int vect[3];
  FILE* file1;
  FILE* file2;
  
  printf("Dame el valor de m:\n");
  scanf("%d",&m);

  printf("Dame el valor de n:\n");
  scanf("%d",&n);
  
  A = malloc(m*n*sizeof(int));
  x = malloc(n*sizeof(int));
  y = malloc(m*sizeof(int));

  //abrir el archivo primer y segundo argumento
  file1 = fopen(argv[1],"r");
  file2 = fopen(argv[2],"r");

  // Determine number of threads
  //sscanf(argv[3], "%d", &num_threads);
  num_threads = 4;
  
  if (NULL == file1){
    printf("no puedo abrir el archivo1\n");
    exit(1);
  }

  if (NULL == file2){
    printf("no puedo abrir el archivo2\n");
    exit(1);
  }
  
  //Poblar la matriz local
  for (i=0;i<m;i++){
    for(j=0;j<n;j++){
      fscanf(file1,"%d",&aux);
      A[i*n+j] = aux;
    }
  }
  
  fclose(file1);

  //Imprimir matriz local
  
  printf("\nLa matriz es: \n");

  for(i=0;i<m;i++){
    printf("\n");
    for(j=0;j<n;j++)
      printf("%d\t",A[i*n+j]);
  }
  
  printf("\n\n");
  
  aux = 0;

  //VECTOR
  //Poblar el vector
  for (int vi=0;vi<n;vi++){
    fscanf(file2,"%d",&aux);
    x[vi] = aux;
  }
  
  fclose(file2);
  
  //Imprimir vector local
  
  printf("\nEl vector es: \n");
  for(int vi=0;vi<m;vi++){
    printf("%d\t",x[vi]);
    printf("\n");
  }
  
  printf("\n\n");
  
  // Allocating threads
  pthread_t* threads;
  threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
  
  // Creating the threads (branching, forking)
  for(long t=0; t<num_threads; t++)
    pthread_create(&threads[t], NULL, mat_vect, (void*)t);
  
  // Do some work with main thread
  //hello((void*)-1);
  
  // Terminate threads (join)
  for(long t=0; t<num_threads; t++)
    pthread_join(threads[t], NULL);

  
  printf("El resultado es:\n");
  for (int i = 0; i < m; i++){
    printf("%d\t", y[i]);
    printf("\n");
  }
  printf("\n");

  // Deallocate memory
  free(A);
  free(x);
  free(y);
  free(threads);
  
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
