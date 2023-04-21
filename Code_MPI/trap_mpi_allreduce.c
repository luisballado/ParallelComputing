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
int main(){
   int my_id, num_proc, n = 1024, local_n;   
   double a = 0.0, b = 3.0, h, total_int;
   double local_a, local_b, local_int;

   // Start MPI
   MPI_Init( NULL, NULL );

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
   MPI_Allreduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

   // Print the result 
   printf("Final result in process %d = %.15f\n", my_id, total_int);

   // End MPI
   MPI_Finalize();

   return 0;
} 
