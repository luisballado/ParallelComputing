#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main( int argc, char **argv ){
	int run_number;
	char problem[ 100 ], hostname[ 100 ];
  	gethostname( hostname, 100 );

	if( argc == 3 ){
		sscanf(argv[1], "%s", problem);
		sscanf(argv[2], "%d", &run_number);
	}else{
		printf("ERROR - Run program as follows:\n\n");
		printf("\t./solve_serial PROBLEM RUN\n\n");
		exit(1);
	}
	
	printf("PROBLEM: %s\n", problem);
	printf("RUN NUMBER: %d\n", run_number);
	printf("This process is running on %s\n", hostname);

	return 0;
}
