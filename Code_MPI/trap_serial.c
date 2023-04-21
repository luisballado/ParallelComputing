#include <stdio.h>

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
int main() {
   double integral, a=0.0, b=3.0, h;
   int n=1024;

   h = (b-a)/n;
   integral = trap_rule(a, b, n, h);
   
   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f\n", a, b);
   printf("= %.15f\n", integral);

   return 0;
}  
