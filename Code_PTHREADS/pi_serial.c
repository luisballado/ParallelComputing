#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double serial_pi(long long n){
	double sum = 0.0, factor = 1.0;

	for (long long i = 0; i < n; i++, factor = -factor) 
		sum += factor/(2*i+1);

	return 4.0*sum;
}

int main(int argc, char* argv[]){

	double pi;
	long long n;

	//  Number of terms?
	sscanf(argv[1], "%lld", &n);

	// Compute PI
	pi = serial_pi(n);

	// Print result
	printf("                   PI = %.10f\n", 4.0*atan(1.0));
	printf("Estimated value of PI = %.10lf\n", pi);

	return 0;
}
