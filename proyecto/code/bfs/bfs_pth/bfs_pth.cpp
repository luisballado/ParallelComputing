#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <pthread.h>

using namespace std;

// Global variables
vector<vector<char>> grid;
int rows, cols;
int start_row, start_col, end_row, end_col;
bool found;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

vector<pair<int, int>> get_neighbors(int i, int j) {
  vector<pair<int, int>> neighbors;
  if (i > 0) neighbors.push_back(make_pair(i-1, j));
  if (i < rows-1) neighbors.push_back(make_pair(i+1, j));
  if (j > 0) neighbors.push_back(make_pair(i, j-1));
  if (j < cols-1) neighbors.push_back(make_pair(i, j+1));
  return neighbors;
}

void* bfs(void* arg) {
  queue<pair<int, int>> q;
  unordered_set<int> visited;
  
  int thread_num = *(int*)arg;
  int start_row_thread = start_row + thread_num;
  int end_row_thread = end_row + thread_num;
  
  for (int i = start_row_thread; i <= end_row_thread; i++) {

    for (int j = 0; j < cols; j++) {

      if (grid[i][j] == 'S') {
	q.push(make_pair(i, j));
	visited.insert(i * cols + j);
	break;
      }

    }

  }
  
  while (!q.empty() && !found) {

    pair<int, int> curr = q.front();
    q.pop();
    
    vector<pair<int, int>> neighbors = get_neighbors(curr.first, curr.second);
    for (auto n : neighbors) {
      int neighbor_row = n.first;
      int neighbor_col = n.second;
      int neighbor_node = neighbor_row * cols + neighbor_col;
      
      if (visited.find(neighbor_node) == visited.end()
	  && grid[neighbor_row][neighbor_col] != '#') {

	if (neighbor_row == end_row && neighbor_col == end_col) {
	  found = true;
	  pthread_mutex_lock(&mutex);
	  cout << "Thread " << thread_num << " found the exit!" << endl;
	  pthread_mutex_unlock(&mutex);
	  break;
	}

	q.push(make_pair(neighbor_row, neighbor_col));
	visited.insert(neighbor_node);
	
      }
    }
  }
  
  pthread_exit(NULL);
}

void dungeon_master_bfs(int num_threads) {
  found = false;
  
  // Determine rows to search for each thread
  int rows_per_thread = rows / num_threads;
  int remaining_rows = rows % num_threads;
  int start = 0;
  
  pthread_t threads[num_threads];
  int thread_nums[num_threads];
  
  for (int i = 0; i < num_threads; i++) {
    start_row = start;
    end_row = start + rows_per_thread - 1;

    if (remaining_rows > 0) {
      end_row++;
      remaining_rows--;
    }
    
    thread_nums[i] = i;
    pthread_create(&threads[i], NULL, bfs, (void*)&thread_nums[i]);
    
    start = end_row + 1;
  }
  
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  
  if (!found) {
    cout << "Exit not found." << endl;
  }
  
}

int main() {
  ifstream infile("grid.txt");
  string line;
  while (getline(infile, line)) {
    vector<char> row;
    for
      
