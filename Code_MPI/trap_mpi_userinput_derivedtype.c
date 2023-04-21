#include <stdio.h>
#include <mpi.h>

///////////////////////////////////////////////////////////
// Function to be integrated
double f(double x){
   return x*x;
}
///////////////////////////////////////////////////////////
// Reads input parameters as input
void get_input( int my_id, int num_proc, double* a, double* b, int* n ){

   // Create derived datatype
   MPI_Datatype user_input;

   int length[3] = { 1, 1, 1 };
   MPI_Datatype type[3] = { MPI_DOUBLE, MPI_DOUBLE, MPI_INT };

   MPI_Aint displacement[3];
   MPI_Aint a_addr, b_addr, n_addr;
   MPI_Get_address( a, &a_addr );
   MPI_Get_address( b, &b_addr );
   MPI_Get_address( n, &n_addr );
   displacement[0] = 0; 
   displacement[1] = b_addr - a_addr; 
   displacement[2] = n_addr - a_addr; 

   MPI_Type_create_struct( 3, length, displacement, type, &user_input );
   MPI_Type_commit( &user_input );

   // Read input and broadcast to all processes
   if( my_id == 0 ){
      printf("Enter a, b, and n:\n");
      scanf("%lf %lf %d", a, b, n);
   }
   MPI_Bcast(a, 1, user_input, 0, MPI_COMM_WORLD);

   // Free derived datatype
   MPI_Type_free( &user_input );

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
   int my_id, num_proc, n, local_n;   
   double a, b, h, total_int;
   double local_a, local_b, local_int;

   // Start MPI
   MPI_Init( NULL, NULL );

   // Get number of processes and ID
   MPI_Comm_size(MPI_COMM_WORLD, &num_proc); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_id); 

   // Read user input
   get_input( my_id, num_proc, &a, &b, &n );

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
   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

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
