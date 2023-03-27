#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>

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
  //sleep(1);
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

//funcion bfs
void bfs(std::map<int, std::vector<int>>& adj_list, int start_node, std::vector<int>& distance,int rows, int cols) {

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
      
      if(rows != -1 && cols != -1){
	print_dist(rows,cols,distance);
      }
      
    }
  }
}

void parallel_bfs(std::map<int, std::vector<int>>& adj_list, int start_node, std::vector<int>& distance,int rows, int cols,int num_threads) {

}

constexpr unsigned int str2int(const char* str, int h = 0){
  return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int main(int argc, char* argv[]) {

  clock_t start, end;
  
  bool PRINT       = false;
  bool show_result = false;
  //bool show_time   = false;
  int num_threads = 1;
  
  const char* MODE = "";
  std::string MODO;
  for(int count = 0; count < argc; count++ ){

    //mostrar frente de onda
    if(argv[count] == std::string("--SHOW"))
      PRINT = true;

    //modo de programa
    if(std::string(argv[count]).substr(0, 6) == std::string("--MODE")){
      MODE = argv[count];
    }
    //numero de threads
    if(std::string(argv[count]).substr(0,5) == std::string("--nth")){
      
      std::string prefijo("--nth=");
      num_threads = std::stoi(std::string(argv[count]).substr(prefijo.size()));

    }

    if(std::string(argv[count]).substr(0,9) == std::string("--results")){
      show_result = true;
    }
    
        
  }
    
  
  ///////////////////////////////////////////////

  int rows;
  int cols;

  std::cin >> rows >> cols;

  start = clock();
  
  //////////////////////////////////////////////////////////////////////
  //Creamos una representacion del grid para detectar las paredes y no
  //considerarlas como nodos en el grafo al construir la lista de adj
  //para verificar en todo momento si algun vecino es pared y omitirlo
  //////////////////////////////////////////////////////////////////////
  std::vector<std::vector<char>> grid(rows, std::vector<char>(cols));
  for(int i=0;i<rows;i++){
    std::string line;
    std::cin >> line;
    for (int j = 0; j < cols; j++){
      grid[i][j] = line[j];
    }
  }
  //////////////////////////////////////////////////////////////////////

  std::map<int, std::vector<int>> adj_list;

  //////////////////////////////////////////////////////////////////////
  //crear lista de adyacencia recorriendo matriz creada,
  //se omite el signo # ya que representa una pared
  //////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////
  
  int num_nodes = rows*cols; //numero de nodos del grafo

  ////// TODO MANEJAR NODOS COMO COORDENADAS
  int start_node; //nodo inicio, donde se localiza el robot
  int finish_node; //nodo destino

  std::cin >> start_node >> finish_node;
  
  //inicializar vector de distancias en -1
  //respecto a la cardinalidad num_nodes
  std::vector<int> distance(num_nodes, -1);

  
  
  //analizar pasandole el nodo inicio, nodo destino y vector de distancias
  //se analiza todo el mapa
  switch(str2int(MODE)){

  case str2int("--MODE=SECUENCIAL"):

    if(PRINT){
      bfs(adj_list, start_node, distance, rows, cols);
    }else{
      bfs(adj_list, start_node, distance, -1, -1);
    }
    
    break;
  case str2int("--MODE=PTHREADS"):

    if(PRINT){
      parallel_bfs(adj_list, start_node, distance,rows,cols, num_threads);
    }else{
      parallel_bfs(adj_list, start_node, distance, -1,-1, num_threads);
    }
    
    break;
  case str2int("--MODE=OPEN-MP"):
    show_result = false;
    std::cout << "ERROR: NO IMPLEMENTADO \n";
    return -1;
  case str2int("--MODE=OPEN-MPI"):
    show_result = false;
    std::cout << "ERROR: NO IMPLEMENTADO \n";
    return -1;
  case str2int("--MODE=HYBRID"):
    show_result = false;
    std::cout << "ERROR: NO IMPLEMENTADO \n";
    return -1;
  default:
    
    show_result = false;

    std::cout << "LAS POSIBLES OPCIONES SON: \n"
	      << "--SECUENCIAL\n"
	      << "--PTHREADS\n"
	      << "--OPEN-MP\n"
	      << "--OPEN-MPI\n"
	      << "--HYBRID\n"
		<< std::endl;
  
    return -1;
    
  }
  

  if(show_result){
    std::cout << "####################################" << std::endl;
    std::cout << "NODOS: " << num_nodes << std::endl;
    std::cout << MODE << std::endl;
    std::cout << "threads: " << num_threads << std::endl;
    std::cout << "####################################\n"
	      << "RESULTADO\n" 
	      << "####################################"
	      << std::endl;
    
    
    
    
    if(distance[finish_node] != -1){
      
      std::cout << "Distancia del Nodo " << start_node
		<< " a " << finish_node
		<< " es " << distance[finish_node] << std::endl;
      std::cout << "####################################\n" << std::endl;
    }else{
      
      std::cout << "No existe un camino del nodo " << start_node
		<< " a " << finish_node << std::endl;
      std::cout << "####################################\n" << std::endl;
    }
  }
  
  end = clock();
  
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

  if(show_result)
    
    std::cout << "Tiempo ejecucion : "
	      << std::fixed
	      << time_taken << std::setprecision(9)
	      << " seg " << std::endl;
  else{
    system("clear");
    std::cout << std::fixed
	      << time_taken << std::setprecision(9)
	      << std::endl;
  }
  
  return 0;
    
}
