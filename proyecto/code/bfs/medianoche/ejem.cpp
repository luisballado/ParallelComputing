#include <pthread.h>
#include <queue>
#include <iostream>
#include <vector>

#define MAX_THREADS 4 // Number of threads to use
#define MAX_NODES 100 // Maximum number of nodes in the graph

std::vector<int> adjList[MAX_NODES]; // Adjacency list representing the graph
bool visited[MAX_NODES]; // Array to mark visited nodes
std::queue<int> bfsQueue; // Queue to hold nodes to visit
pthread_mutex_t queueMutex; // Mutex to synchronize access to the queue
pthread_t threads[MAX_THREADS]; // Array of threads

void* bfs(void* arg) {
  int threadNum = *(int*)arg;
  while (true) {
    // Obtain node from queue
    pthread_mutex_lock(&queueMutex);
    if (bfsQueue.empty()) {
      pthread_mutex_unlock(&queueMutex);
      break; // No more nodes to visit
    }
    int node = bfsQueue.front();
    bfsQueue.pop();
    pthread_mutex_unlock(&queueMutex);

    // Process node
    visited[node] = true;
    std::cout << "Thread " << threadNum << " visited node " << node << std::endl;

    // Add unvisited neighbors to queue
    for (int neighbor : adjList[node]) {
      if (!visited[neighbor]) {
        pthread_mutex_lock(&queueMutex);
        bfsQueue.push(neighbor);
        pthread_mutex_unlock(&queueMutex);
      }
    }
  }
  pthread_exit(NULL);
}

int main() {
  // Initialize graph
  adjList[0] = {1, 2};
  adjList[1] = {0, 3};
  adjList[2] = {0, 3};
  adjList[3] = {1, 2, 4};
  adjList[4] = {3};

  // Initialize BFS
  bfsQueue.push(0);
  visited[0] = true;

  // Initialize mutex
  pthread_mutex_init(&queueMutex, NULL);

  // Initialize thread arguments
  int threadArgs[MAX_THREADS];
  for (int i = 0; i < MAX_THREADS; i++) {
    threadArgs[i] = i;
  }

  // Create threads
  for (int i = 0; i < MAX_THREADS; i++) {
    pthread_create(&threads[i], NULL, bfs, &threadArgs[i]);
  }

  // Wait for threads to finish
  for (int i = 0; i < MAX_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Clean up mutex
  pthread_mutex_destroy(&queueMutex);

  return 0;
}
