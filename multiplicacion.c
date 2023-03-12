#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 8

int **A, **B, **C;
int N, M, P;
int num_threads;

void *multiply(void *arg) {
    int tid = *((int *) arg);
    int start_row = tid * (N / num_threads);
    int end_row = (tid + 1) * (N / num_threads);

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;
            for (int k = 0; k < M; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <matrix A file> <matrix B file> <N> <M/P>\n", argv[0]);
        return 1;
    }

    char *filenameA = argv[1];
    char *filenameB = argv[2];
    N = atoi(argv[3]);
    M = atoi(argv[4]);
    P = M; // square matrix
    num_threads = (MAX_THREADS < N) ? MAX_THREADS : N;

    // Allocate memory for matrices
    A = malloc(N * sizeof(int *));
    B = malloc(N * sizeof(int *));
    C = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        A[i] = malloc(M * sizeof(int));
        B[i] = malloc(P * sizeof(int));
        C[i] = malloc(P * sizeof(int));
    }

    // Read matrices from files
    FILE *fA = fopen(filenameA, "r");
    FILE *fB = fopen(filenameB, "r");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fscanf(fA, "%d", &A[i][j]);
        }
        for (int j = 0; j < P; j++) {
            fscanf(fB, "%d", &B[i][j]);
        }
    }
    fclose(fA);
    fclose(fB);

    // Create threads
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print result
    printf("Result:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Free memory
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
