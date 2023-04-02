#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>
#include <map>
#include <unistd.h>

//funcion get_neighbors para obtener los vecinos respecto a la matriz
std::vector<std::pair<int, int>> get_neighbors(int i, int j, int rows, int cols) {
  
  std::vector<std::pair<int, int>> neighbors;
  
  if (i > 0) neighbors.push_back(std::make_pair(i-1, j));
  if (i < rows-1) neighbors.push_back(std::make_pair(i+1, j));
  if (j > 0) neighbors.push_back(std::make_pair(i, j-1));
  if (j < cols-1) neighbors.push_back(std::make_pair(i, j+1));
  
  return neighbors;
  
}

// print distance from source node to each node in grid
void print_dist(int rows,int cols,std::vector<int>& distance) {
  usleep(1000);
  system("clear");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int u = i*cols + j;
      if (distance[u] == -1) {
	std::cout << "  # ";
      } else {
	//poner distancias fancy
	if(std::to_string(distance[u]).length() == 1){
	  std::cout << "  " << distance[u] << " ";
	}else{
	  std::cout << " " << distance[u] << " ";
	}
      }
    }
    std::cout << std::endl;
  }
}

//using namespace std;
void bfs(int node, std::map<int, std::vector<int>>& adj_list, std::vector<bool>& visited, std::vector<int>& distance,int rows, int cols) {
  
  std::queue<int> q;
  
  q.push(node);
  visited[node] = true;
  distance[node] = 0;
  
  while (!q.empty()) {
    int curr_node = q.front();
    q.pop();
    
    // Process the current node
    std::cout << "visited node " << curr_node << " at thread " << omp_get_thread_num() << std::endl;
        
    // Visit neighbors
    #pragma omp parallel for
    for (int neighbor : adj_list[curr_node]) {
      if (!visited[neighbor]) {
	visited[neighbor] = true;
	distance[neighbor] = distance[curr_node] + 1;
	q.push(neighbor);
      }
      /**
      if(rows != -1 && cols != -1){
	print_dist(rows,cols,distance);
      }
      */
    }
  }
}

int main() {

  int rows;
  int cols;

  std::cin >> rows >> cols;

  std::vector<std::vector<char>> grid(rows, std::vector<char>(cols));

  for(int i = 0; i < rows; i++){
    std::string line;
    std::cin >> line;

    for(int j=0;j<cols;j++){
      grid[i][j] = line[j];
    }
  }

  std::map<int, std::vector<int>> adj_list;

  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      if(grid[i][j] != '#'){
	int node = i*cols+j;
	std::vector<std::pair<int,int>> neighbors = get_neighbors(i,j,rows,cols);
	for(auto n : neighbors){
	  int neighbor_node = n.first*cols+n.second;
	  if(grid[n.first][n.second] != '#'){
	    adj_list[node].push_back(neighbor_node);
	  }
	}
      }
    }
  }

  int num_nodes = rows * cols;

  int start_node;
  int finish_node;

  std::cin >> start_node >> finish_node;

  std::vector<int> distance(num_nodes, -1);
  
  int num_threads = 10;
  
  omp_set_num_threads(num_threads);
  
  std::vector<bool> visited(num_nodes, false);
    
  #pragma omp parallel
  {
    #pragma omp single
    {
      // Start BFS from node 0
      bfs(start_node, adj_list, visited, distance, rows, cols);
    }
  }
  
  // Print distances from source node
  /**
  for (int i = 0; i < num_nodes; i++) {
    std::cout << "Distance from node 0 to node " << i << " is " << distance[i] << std::endl;
  }
  */
  
  return 0;
}
