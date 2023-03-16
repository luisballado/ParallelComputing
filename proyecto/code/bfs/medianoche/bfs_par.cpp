#include <iostream>
#include <vector>
#include <queue>
#include <pthread.h>
#include <map>

using namespace std;

struct ThreadArgs {
  map<int, vector<int>> *adj_list;
  int start_node;
  vector<int> *distance;
  int *next_frontier;
  int *frontier_size;
  pthread_mutex_t *frontier_mutex;
  pthread_barrier_t *barrier;
};

vector<pair<int, int>> get_neighbors(int i, int j, int rows, int cols) {
  
  vector<pair<int, int>> neighbors;
  
  if (i > 0) neighbors.push_back(make_pair(i-1, j));
  if (i < rows-1) neighbors.push_back(make_pair(i+1, j));
  if (j > 0) neighbors.push_back(make_pair(i, j-1));
  if (j < cols-1) neighbors.push_back(make_pair(i, j+1));
  
  return neighbors;
  
}

void *worker(void *arg) {
  
  ThreadArgs *args = (ThreadArgs *) arg;
  map<int, vector<int>> &adj_list = *(args->adj_list);
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

void parallel_bfs(map<int,vector<int>>& adj_list, int start_node, vector<int>& distance){
  
  int num_threads = 4;
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
  
  int rows;
  int cols;
  
  cin >> rows >> cols;
  
  vector<vector<char>> grid(rows, vector<char>(cols));
  
  for(int i=0;i<rows;i++){
    string line;
    cin >> line;
    for (int j = 0; j < cols; j++){
      grid[i][j] = line[j];
    }
  }
  
  map<int, vector<int>> adj_list;
    
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[i][j] != '#') {
	int node = i * cols + j;
	vector<pair<int, int>> neighbors = get_neighbors(i, j, rows, cols);
	
	for (auto n : neighbors) {
	  int neighbor_node = n.first * cols + n.second;
	  if (grid[n.first][n.second] != '#') {
	    adj_list[node].push_back(neighbor_node);
	  }
	}
      }
    }
  }
  
  // Print the adjacency list to make sure it was constructed correctly
  for (auto node : adj_list) {
    cout << node.first << ": ";
    for (int neighbor : node.second) {
      cout << neighbor << " ";
    }
    cout << endl;
  }
  
  int num_nodes = rows*cols;
  int start_node = 0;
  
  vector<int> distance(num_nodes, -1);

  parallel_bfs(adj_list, start_node, distance);
  
  for (int i = 0; i < num_nodes; i++) {
    cout << "Distancia del nodo " << start_node
	 << " a " << i << " es " << distance[i] << endl;
  }
  
  return 0;
}
