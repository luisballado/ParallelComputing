#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <pthread.h>

using namespace std;

// Data structures used by the BFS algorithm
struct ThreadData {
  int thread_id;
  vector<int> *frontiers;
  map<int, bool> *visited;
  map<int, int> *distance;
  pthread_mutex_t *mutex;
  map<int, vector<int>> *adj_list;
};

void *parallel_bfs_thread(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  vector<int> *frontiers = data->frontiers;
  map<int, bool> *visited = data->visited;
  map<int, int> *distance = data->distance;
  pthread_mutex_t *mutex = data->mutex;
  map<int, vector<int>> *adj_list = data->adj_list;
  
  // Process nodes until there are no more nodes left
  while (!frontiers->empty()) {
    // Get the next node from the frontiers

    //BLOQUEAR PARA ACTUALIZAR FRONTERA
    pthread_mutex_lock(mutex);
    int node = frontiers->back();
    //quitar elemento de frontera por hacer

    //*********************************************
    
    frontiers->pop_back();
    pthread_mutex_unlock(mutex);
    cout << "Aqui voyyyyyy22" << endl;
    // Process the node if it hasn't been visited
    if (!(*visited)[node]) {
      // Mark the node as visited
      (*visited)[node] = true;
      
      // Update the distance to the node
      (*distance)[node] = (*distance)[node] == -1 ? 1 : (*distance)[node];
      
      // Add the node's neighbors to the next frontier
      for (int i = 0; i < (*adj_list)[node].size(); i++) {
	int neighbor = (*adj_list)[node][i];
	if (!(*visited)[neighbor]) {
	  // Add the neighbor to the next frontier
	  pthread_mutex_lock(mutex);
	  frontiers->push_back(neighbor);
	  pthread_mutex_unlock(mutex);
	}
      }
    }
  }
  
  // Exit the thread
  pthread_exit(NULL);
}

// Breadth-first search algorithm with parallelization using pthreads
void parallel_bfs(map<int, vector<int>> &adj_list, int start_node, map<int, int> &distance, int num_threads) {
  // Initialize distance and visited maps
  for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
    distance[it->first] = -1;
  }
  
  distance[start_node] = 0;

  map<int, bool> visited;
  visited[start_node] = true;

  
  
  // Create the first frontier
  vector<int> frontiers;
  frontiers.push_back(start_node);
  
  // Create thread data and initialize mutex
  pthread_t threads[num_threads];
  ThreadData thread_data[num_threads];
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  
  // Start the threads
  for (int i = 0; i < num_threads; i++) {
    thread_data[i].thread_id = i;
    thread_data[i].frontiers = &frontiers;
    thread_data[i].visited = &visited;
    thread_data[i].distance = &distance;
    thread_data[i].mutex = &mutex;
    thread_data[i].adj_list = &adj_list;
    pthread_create(&threads[i], NULL, parallel_bfs_thread, (void *)&thread_data[i]);
  }
  
  // Join the threads
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  
  // Clean up mutex
  pthread_mutex_destroy(&mutex);
}



// Main function
int main() {
  // Define an example adjacency list
  map<int, vector<int>> adj_list = {
				    {0, {1, 2}},
				    {1, {0, 3}},
				    {2, {0, 3}},
				    {3, {1, 2, 4}},
				    {4, {3}}
  };
  // Run the parallel BFS algorithm with 2 threads starting from node 0
  map<int, int> distance;
  parallel_bfs(adj_list, 0, distance, 2);
  
  // Print the distances from node 0 to each node
  for (auto it = distance.begin(); it != distance.end(); it++) {
    cout << "Distance from node 0 to node " << it->first << ": " << it->second << endl;
  }
  
  return 0;
}

