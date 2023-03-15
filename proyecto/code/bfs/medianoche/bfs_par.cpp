#include <iostream>
#include <vector>
#include <queue>
#include <pthread.h>

using namespace std;

struct ThreadArgs {
    vector<vector<int>> *adj_list;
    int start_node;
    vector<int> *distance;
    int *next_frontier;
    int *frontier_size;
    pthread_mutex_t *frontier_mutex;
    pthread_barrier_t *barrier;
};

void *worker(void *arg) {
    ThreadArgs *args = (ThreadArgs *) arg;
    vector<vector<int>> &adj_list = *(args->adj_list);
    int start_node = args->start_node;
    vector<int> &distance = *(args->distance);
    int *next_frontier = args->next_frontier;
    int *frontier_size = args->frontier_size;
    pthread_mutex_t *frontier_mutex = args->frontier_mutex;
    pthread_barrier_t *barrier = args->barrier;

    // Initialize the distance and visited arrays
    vector<bool> visited(adj_list.size(), false);
    visited[start_node] = true;
    distance[start_node] = 0;

    // Initialize the first frontier
    int curr_frontier = 0;
    next_frontier[curr_frontier] = start_node;
    frontier_size[curr_frontier] = 1;

    // Loop until all nodes have been visited
    while (true) {
        // Process the current frontier
        for (int i = 0; i < frontier_size[curr_frontier]; i++) {
            int curr_node = next_frontier[curr_frontier * adj_list.size() + i];

            // Process the neighbors of the current node
            for (int j = 0; j < adj_list[curr_node].size(); j++) {
                int neighbor = adj_list[curr_node][j];

                // Update the distance and visited arrays
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    distance[neighbor] = distance[curr_node] + 1;

                    // Add the neighbor to the next frontier
                    pthread_mutex_lock(frontier_mutex);
                    int next_frontier_idx = curr_frontier * adj_list.size() + frontier_size[curr_frontier];
                    next_frontier[next_frontier_idx] = neighbor;
                    frontier_size[curr_frontier]++;
                    pthread_mutex_unlock(frontier_mutex);
                }
            }
        }

        // Wait for all threads to finish processing the current frontier
        pthread_barrier_wait(barrier);

        // Check if all nodes have been visited
        bool done = true;
        for (bool v : visited) {
            if (!v) {
                done = false;
                break;
            }
        }

        if (done) {
            break;
        }

        // Switch to the next frontier
        curr_frontier = (curr_frontier + 1) % 2;
        next_frontier[curr_frontier * adj_list.size()] = -1;
        frontier_size[curr_frontier] = 0;

        // Wait for all threads to switch to the next frontier
        pthread_barrier_wait(barrier);
    }

    return NULL;
}

void parallel_bfs(vector<vector<int>> &adj_list, int start_node, vector<int> &distance) {
    int num_threads = 2;
    int next_frontier_size = adj_list.size() * 2;
    int next_frontier[num_threads][next_frontier_size];
    int frontier_size[num_threads] = {0, 0};
    pthread_mutex_t frontier_mutex;
    pthread_barrier_t barrier;
    pthread_mutex_init(&frontier_mutex, NULL);
    pthread_barrier_init(&barrier, NULL, num_threads);

    // Initialize the thread arguments
    ThreadArgs args[num_threads];
    pthread_t threads[num_threads];
    
    for (int i = 0; i < num_threads; i++) {
      args[i].adj_list = &adj_list;
      args[i].start_node = start_node;
      args[i].distance = &distance;
      args[i].next_frontier = next_frontier[i];
      args[i].frontier_size = frontier_size;
      args[i].frontier_mutex = &frontier_mutex;
      args[i].barrier = &barrier;
      pthread_create(&threads[i], NULL, worker, &args[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
      pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&frontier_mutex);
    pthread_barrier_destroy(&barrier);
    
}

int main() {
  int num_nodes = 7;
  int start_node = 0;
  vector<vector<int>> adj_list(num_nodes);
  adj_list[0] = {1, 2};
  adj_list[1] = {0, 3, 4};
  adj_list[2] = {0, 4};
  adj_list[3] = {1, 5};
  adj_list[4] = {1, 2, 5};
  adj_list[5] = {3, 4, 6};
  adj_list[6] = {5};

  vector<int> distance(num_nodes, -1);
  parallel_bfs(adj_list, start_node, distance);
  
  for (int i = 0; i < num_nodes; i++) {
    cout << "Distance from " << start_node << " to " << i << " is " << distance[i] << endl;
  }
  
  return 0;
}
