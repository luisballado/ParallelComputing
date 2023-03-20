#include <iostream>
#include <queue>
#include <vector>
#include <map>

//funcion get_neighbors para obtener los vecinos respecto a la matriz
std::vector<std::pair<int, int>> get_neighbors(int i, int j, int rows, int cols) {
  
  std::vector<std::pair<int, int>> neighbors;
  
  if (i > 0) neighbors.push_back(std::make_pair(i-1, j));
  if (i < rows-1) neighbors.push_back(std::make_pair(i+1, j));
  if (j > 0) neighbors.push_back(std::make_pair(i, j-1));
  if (j < cols-1) neighbors.push_back(std::make_pair(i, j+1));
  
  return neighbors;
  
}

//funcion bfs
void bfs(std::map<int, std::vector<int>>& adj_list, int start_node, std::vector<int>& distance) {

  std::queue<int> q;

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

  std::cin >> rows >> cols;

  std::vector<std::vector<char>> grid(rows, std::vector<char>(cols));

  for(int i=0;i<rows;i++){
    std::string line;
    std::cin >> line;
    for (int j = 0; j < cols; j++){
      grid[i][j] = line[j];
    }
  }
  
  std::map<int, std::vector<int>> adj_list;

  //crear lista de adyacencia recorriendo matriz,
  //se omite el signo # ya que representa una pared
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      //si el elemento es diferente a # continuamos
      if (grid[i][j] != '#') {
	int node = i * cols + j;
	std::vector<std::pair<int, int>> neighbors = get_neighbors(i, j, rows, cols);
	//analizamos los vecinos y si son diferentes a # los agregamos
	for (auto n : neighbors) {
	  int neighbor_node = n.first * cols + n.second;
	  if (grid[n.first][n.second] != '#') {
	    adj_list[node].push_back(neighbor_node);
	  }
	}
	
      }
    }
  }
  
  int num_nodes = rows*cols; //numero de nodos del grafo
  
  int start_node; //nodo inicio, donde se localiza el robot
  int finish_node; //nodo destino

  std::cin >> start_node >> finish_node;
  
  std::cout << "NODOS " << num_nodes << std::endl;

  //vector de distancias en -1
  std::vector<int> distance(num_nodes, -1);

  //analizar
  //pasandole el nodo inicio
  //nodo destino y vector de distancias
  bfs(adj_list, start_node, distance);

  //verificar si existe un camino
  if(distance[finish_node] != -1){
    std::cout << "Distancia del Nodo " << start_node
	      << " a " << finish_node
	      << " es " << distance[finish_node] << std::endl;
  }else{
    std::cout << "No existe un camino del nodo " << start_node
	      << " a " << finish_node << std::endl;
  }
  
  return 0;

}
