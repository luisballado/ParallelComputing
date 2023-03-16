#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <pthread.h>

using namespace std;

// Struct to hold thread-specific data
struct ThreadData {
    int id;
    vector<int> *distance;
    map<int, vector<int>> *adj_list;
    vector<bool> *visited;
    vector<int> *frontier;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond_var;
    bool *done;
};

// Function to process a frontier
void process_frontier(vector<int> &frontier, vector<bool> &visited, vector<int> &next_frontier, map<int, vector<int>> &adj_list, vector<int> &distance) {
    for (int i = 0; i < frontier.size(); i++) {
        int curr_node = frontier[i];
        visited[curr_node] = true;

        // Process the neighbors of the current node
        for (int j = 0; j < adj_list[curr_node].size(); j++) {
            int neighbor = adj_list[curr_node][j];

            // Update the distance and visited arrays
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[curr_node] + 1;

                // Add the neighbor to the next frontier
                next_frontier.push_back(neighbor);
            }
        }
    }
}

// Function to be executed by each thread
void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    while (true) {
        // Acquire the mutex and wait for the condition variable
        pthread_mutex_lock(data->mutex);
        while (data->frontier->empty() && !(*data->done)) {
            pthread_cond_wait(data->cond_var, data->mutex);
        }

        // Check if all nodes have been visited
        if (*data->done) {
            pthread_mutex_unlock(data->mutex);
            break;
        }

        // Process the frontier
        vector<int> next_frontier;
        process_frontier(*data->frontier, *data->visited, next_frontier, *data->adj_list, *data->distance);

        // Clear the current frontier and add the next frontier
        data->frontier->clear();
        *data->frontier = next_frontier;

        // Notify the other threads
        pthread_cond_broadcast(data->cond_var);
        pthread_mutex_unlock(data->mutex);
    }

    pthread_exit(NULL);
}

void parallel_bfs(map<int, vector<int>> &adj_list, int start_node, vector<int> &distance, int num_threads) {
    // Initialize the distance and visited arrays
    vector<bool> visited(adj_list.size(), false);
    visited[start_node] = true;
    distance[start_node] = 0;

    // Initialize the first frontier
    vector<int> frontier = {start_node};

    // Initialize the thread data
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
    bool done = false;
    vector<pthread_t> threads(num_threads);
    vector<ThreadData> thread_data(num_threads);
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].id = i;
        thread_data[i].distance = &distance;
        thread_data[i].adj_list = &adj_list;
        thread_data[i].visited = &visited;
        thread_data[i].frontier = &frontier;
        thread_data[i].mutex = &mutex;
        thread_data[i].cond_var = &cond_var;
        thread_data[i].done = &done;
	pthread_mutex_init(&thread_data[i].mutex, NULL);
	pthread_cond_init(&thread_data[i].cond_var, NULL);
    }
    
    // Create the threads
    for (int i = 0; i < num_threads; i++) {
      pthread_create(&threads[i], NULL, thread_func, (void *)&thread_data[i]);
    }
    
    // Wait for the threads to finish
    for (int i = 0; i < num_threads; i++) {
      pthread_join(threads[i], NULL);
    }
    
    // Clean up the thread data
    for (int i = 0; i < num_threads; i++) {
      pthread_mutex_destroy(&thread_data[i].mutex);
      pthread_cond_destroy(&thread_data[i].cond_var);
    }

    }

int main() {
// Create the adjacency list
map<int, vector<int>> adj_list;
adj_list[0] = {1, 2};
adj_list[1] = {0, 3, 4};
adj_list[2] = {0, 5, 6};
adj_list[3] = {1};
adj_list[4] = {1};
adj_list[5] = {2};
adj_list[6] = {2};
 // Run the parallel BFS algorithm
int start_node = 0;
int num_threads = 2;
vector<int> distance(adj_list.size(), -1);
parallel_bfs(adj_list, start_node, distance, num_threads);

// Print the distance array
for (int i = 0; i < distance.size(); i++) {
    cout << "Distance from " << start_node << " to " << i << " is " << distance[i] << endl;
}

return 0;
}
