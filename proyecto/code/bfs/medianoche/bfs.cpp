#include <iostream>
#include <queue>
#include <vector>
#include <map>

using namespace std;

vector<pair<int, int>> get_neighbors(int i, int j, int rows, int cols) {
  
  vector<pair<int, int>> neighbors;
  
  if (i > 0) neighbors.push_back(make_pair(i-1, j));
  if (i < rows-1) neighbors.push_back(make_pair(i+1, j));
  if (j > 0) neighbors.push_back(make_pair(i, j-1));
  if (j < cols-1) neighbors.push_back(make_pair(i, j+1));
  
  return neighbors;
  
}

void bfs(map<int, vector<int>>& adj_list, int start_node, vector<int>& distance) {

  queue<int> q;

  distance[start_node] = 0;
  q.push(start_node);
  
  while (!q.empty()) {
    int node = q.front();
    q.pop();
    
    for (int neighbor : adj_list[node]) {
      if (distance[neighbor] == -1) {
	distance[neighbor] = distance[node] + 1;
	q.push(neighbor);
      }
    }
  }
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

  cout << "NODOS " << num_nodes << endl;
  
  vector<int> distance(num_nodes, -1);

  bfs(adj_list, start_node, distance);
  
  for (int i = 0; i < num_nodes; i++) {
    cout << "Distancia del Nodo " << start_node << " a " << i << " es " << distance[i] << endl;
  }
  
  return 0;
}
