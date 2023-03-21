#include <iostream>
#include <mpi.h>

#define ARRAY_SIZE 1000000

int main(int argc, char** argv) {
  int rank, size, chunkSize;
  int sum = 0, totalSum = 0;
  int array[ARRAY_SIZE];

  // Initialize MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Calculate chunk size
  chunkSize = ARRAY_SIZE / size;

  // Initialize array
  for (int i = 0; i < ARRAY_SIZE; i++) {
    array[i] = i + 1;
  }

  // Calculate partial sum for this process
  int partialSum = 0;
  for (int i = rank * chunkSize; i < (rank + 1) * chunkSize; i++) {
    partialSum += array[i];
  }

  // Reduce partial sums to obtain total sum
  MPI_Reduce(&partialSum, &totalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // Print result from process 0
  if (rank == 0) {
    std::cout << "The sum of elements in the array is: " << totalSum << std::endl;
  }

  // Finalize MPI
  MPI_Finalize();

  return 0;
}
