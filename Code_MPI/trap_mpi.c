#include <stdio.h>
#include <mpi.h>

///////////////////////////////////////////////////////////
// Function to be integrated
double f(double x){
   return x*x;
}
///////////////////////////////////////////////////////////
// Trapezoidal rule
double trap_rule(double a, double b, int n, double h){

   double integral = ( f(a) + f(b) )/2.0;

   for (int k = 1; k <= n-1; k++)
     integral += f( a + k * h );

   return integral * h;
} 
///////////////////////////////////////////////////////////
int main(int argc, char* argv[]){
   int my_id, num_proc, n = 1024000000, local_n;   
   double a = 0.0, b = 3.0, h, total_int;
   double local_a, local_b, local_int;

   // Start MPI
   MPI_Init( &argc, &argv );

   // Get number of processes and ID
   MPI_Comm_size(MPI_COMM_WORLD, &num_proc); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_id); 

   // Base of all trapezoids
   h = (b-a) / n;

   // Subinterval information
   // NOTE: this code assumes num_proc divides n
   local_n = n / num_proc;
   local_a = a + my_id * local_n * h;
   local_b = local_a + local_n * h;

   // Compute area of subinterval
   local_int = trap_rule( local_a, local_b, local_n, h );

   // Add up the integrals calculated by each process 
   if( my_id != 0 ){ 

      MPI_Send( &local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

   }else{

      total_int = local_int;

      for( int p = 1; p < num_proc; p++ ){
         MPI_Status stat;
         MPI_Recv(&local_int, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
         total_int += local_int;
         printf("Adding from process %d\n", stat.MPI_SOURCE);


      }

   } 

   // Print the result 
   if( my_id == 0 ){
      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f\n", a, b);
      printf("= %.15f\n", total_int);
   }

   // End MPI
   MPI_Finalize();

   return 0;
} 
