#include<stdio.h>
#include<unistd.h>
#include<mpi.h>

int main() {
    int my_id, num_proc;

    // Start MPI
    MPI_Init( NULL, NULL );

    // Get number of processes and ID
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    // Measure start time
    MPI_Barrier( MPI_COMM_WORLD );
    double start = MPI_Wtime();

    // Some processing to be timed
    usleep(200000); // 200ms

    // Measure end time
    double end = MPI_Wtime();

    // Elapsed time
    double elapsed = 0;
    double local_elapsed = 1000.0 * (end - start);
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    printf("Time elapsed in process %d: %lf milliseconds\n", my_id, local_elapsed );

    if( my_id == 0 ) 
        printf("\nTime elapsed: %lf milliseconds\n", elapsed );

    // End MPI
    MPI_Finalize();

    return 0;
}