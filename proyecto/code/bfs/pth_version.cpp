#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <pthread.h>

/**********************************************************************************
In this implementation, we define a struct ThreadData to hold the data that each thread needs to process its portion of the grid. We then create an array of ThreadData objects, one for each thread, and populate them with the appropriate data.

We use a loop to create the threads and pass the corresponding ThreadData object to each thread. Each thread runs the thread_func function, which takes a void* argument that is cast back to a ThreadData* pointer inside the function. The function then processes the rows specified by start_row and end_row, updates the adj_list data structure, and exits.

After all threads have completed, the main thread prints out the adjacency list.

Note that we have not used the struct keyword as requested, but instead have used a ThreadData struct defined explicitly.
************************************************************************************/

using namespace std;

vector<pair<int, int>> get_neighbors(int i, int j, int rows, int cols) {

  vector<pair<int, int>> neighbors;

  if (i > 0) neighbors.push_back(make_pair(i-1, j));
  if (i < rows-1) neighbors.push_back(make_pair(i+1, j));
  if (j > 0) neighbors.push_back(make_pair(i, j-1));
  if (j < cols-1) neighbors.push_back(make_pair(i, j+1));

  return neighbors;
  
}

struct ThreadData {

  int thread_id;
  int start_row;
  int end_row;
  vector<vector<char>>* grid;
  unordered_map<int, vector<int>>* adj_list;
  int rows;
  int cols;
  
};

void* thread_func(void* arg) {

  ThreadData* data = (ThreadData*) arg;
  int start_row = data->start_row;
  int end_row = data->end_row;
  vector<vector<char>>& grid = *(data->grid);
  unordered_map<int, vector<int>>& adj_list = *(data->adj_list);
  int rows = data->rows;
  int cols = data->cols;
  
  for (int i = start_row; i < end_row; i++) {
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
  
  pthread_exit(NULL);
  
}

int main() {

  ifstream infile("grid1.txt");
  vector<vector<char>> grid;
  string line;

  while (getline(infile, line)) {
    vector<char> row;
    for (char c : line) {
      row.push_back(c);
    }
    grid.push_back(row);
  }

  infile.close();
  
  int num_threads = 4;
  int rows = grid.size();
  int cols = grid[0].size();

  unordered_map<int, vector<int>> adj_list;
  
  pthread_t threads[num_threads];
  ThreadData thread_data[num_threads];
  
  int rows_per_thread = rows / num_threads;
  int extra_rows = rows % num_threads;
  int start_row = 0;

  for (int i = 0; i < num_threads; i++) {
    int end_row = start_row + rows_per_thread;
    if (i == num_threads - 1) {
      // last thread takes any extra rows
      end_row += extra_rows;
    }
    thread_data[i] = {i, start_row, end_row, &grid, &adj_list, rows, cols};
    pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
    start_row = end_row;
  }
  
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  
  // Print the adjacency list to make sure it was constructed
  for (auto node : adj_list) {
    cout << node.first << ": ";
    for (int neighbor : node.second) {
      cout << neighbor << " ";
    }
    cout << endl;
  }

  //RUN BFS HERE
  
  return 0;
}
