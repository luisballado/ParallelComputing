#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

///////////////////////////////////////////////////////////
// Function to be integrated
double f(double x){
   return x*x;
}
///////////////////////////////////////////////////////////
// Trapezoidal rule
double trap_rule(double a, double b, int n){
   int my_id = omp_get_thread_num();
   int n_threads = omp_get_num_threads();

   double h = (b - a) / n; 
   int local_n = n / n_threads;  
   double local_a = a + my_id * local_n * h; 
   double local_b = local_a + local_n * h; 

   double local_result = ( f(local_a) + f(local_b) ) / 2.0; 
   for( int k = 1; k <= local_n-1; k++ )
      local_result += f( local_a + k * h );
   local_result = local_result * h; 

   return local_result; 
}
///////////////////////////////////////////////////////////
int main( int argc, char* argv[] ){
   double global_result=0.0, a=0.0, b=3.0;
   int n=600000000, n_threads=6;

#  pragma omp parallel num_threads( n_threads ) 
   {
      double my_result = 0.0;
      my_result += trap_rule( a, b, n );
#     pragma omp critical
      global_result += my_result;
   }

   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f\n", a, b);
   printf("= %.15f\n", global_result);

   return 0;
}
