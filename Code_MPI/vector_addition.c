#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

////////////////////////////////////////////////////////////////////////////////
void read_n( int* n, int* local_n, int my_id, int num_proc ){

   // Process 0 reads from user input
   if (my_id == 0) {
      printf("What's the size of the vectors?\n");
      scanf("%d", n);
   }

   // Share n with all other processes
   MPI_Bcast( n, 1, MPI_INT, 0, MPI_COMM_WORLD );

   // Compute local_n
   *local_n = *n/num_proc;

}
////////////////////////////////////////////////////////////////////////////////
void read_vector( double local_a[], int local_n, int n, char vec_name[], int my_id ) {
   double* a = NULL;

   // Only Process 0 reads values from user
   if( my_id == 0 ){

      a = malloc(n*sizeof(double));

      printf("Enter vector %s\n", vec_name);
      for (int i = 0; i < n; i++)
         scanf("%lf", &a[i]);

      MPI_Scatter( a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD );
      
      free(a);

   }else{

      MPI_Scatter( a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD );

   }
}
////////////////////////////////////////////////////////////////////////////////
void print_vector( double local_b[], int local_n, int n, char title[], int my_id ){
   double* b = NULL;

   if( my_id == 0 ){

      b = malloc(n*sizeof(double));

      MPI_Gather( local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD );
      
      printf("%s\n", title);
      for (int i = 0; i < n; i++)
         printf("%f ", b[i]);
      printf("\n");

      free(b);

   }else{

      MPI_Gather( local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD );

   }
} 
////////////////////////////////////////////////////////////////////////////////
void parallel_vector_sum( double x[], double y[], double z[], int n ){

   for( int i = 0; i < n; i++ ) z[i] = x[i] + y[i];

} 
////////////////////////////////////////////////////////////////////////////////
int main(){
   int n, local_n, num_proc, my_id;
   double *local_x, *local_y, *local_z;

   // Start MPI and get proc info
   MPI_Init( NULL, NULL );
   MPI_Comm_size( MPI_COMM_WORLD, &num_proc );
   MPI_Comm_rank( MPI_COMM_WORLD, &my_id );

   // Read size of vector from user
   read_n( &n, &local_n, my_id, num_proc );

   // Allocate memory
   local_x = malloc( local_n * sizeof(double) );
   local_y = malloc( local_n * sizeof(double) );
   local_z = malloc( local_n * sizeof(double) );

   // Read vectors
   read_vector( local_x, local_n, n, "x", my_id );
   read_vector( local_y, local_n, n, "y", my_id );

   // Compute sum of vectors
   parallel_vector_sum( local_x, local_y, local_z, local_n );

   // Print results
   print_vector( local_x, local_n, n, "Vector x is: ", my_id );
   print_vector( local_y, local_n, n, "Vector y is: ", my_id );
   print_vector( local_z, local_n, n, "The sum z is: ", my_id );

   // Deallocate memory
   free(local_x);
   free(local_y);
   free(local_z);

   // End MPI
   MPI_Finalize();

   return 0;
}  
