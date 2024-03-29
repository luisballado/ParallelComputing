#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>
#include <unistd.h>
#include <iomanip>
#include <random>

#include <mpi.h>

// RANDOM /////////////////////////////////////////////
// Generar numeros random /////////////////////////////
///////////////////////////////////////////////////////
std::random_device rd;
std::mt19937 gen(rd());
 
int random(int low, int high){
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}
///////////////////////////////////////////////////////

// SPLIT WORK /////////////////////////////////////////////
// Repartir trabajo respecto al num robots y num threads //
///////////////////////////////////////////////////////////
std::vector<int> get_workload(int robots, int threads){
  
  double work;
  int sum_work = 0;
  int sobra;
  
  work = robots / threads;

  std::vector<int> workers;

  //si work no es entero
  if(work*threads != robots){
    
    //me quedo con el resultado de la parte entera y completo las piedras
    work = int (work);
    
    for(int i=0;i<threads;i++){
      //std::cout << "repartiendo de a " << work << std::endl; 
      sum_work = sum_work + work;
      workers.push_back(work);
    }
    
    sobra = robots - sum_work;
    //std::cout << "me sobra " << sobra  << std::endl;  

    // rellenar
    for(int w=0;w<workers.size();++w){
      
      workers[w] = workers[w] + 1;
      sobra = sobra - 1;
      
      if(sobra==0)
	break;  
    }
    
  }else{

    //repartir la chamba equilibrada
    for(int i = 0;i<threads;i++){
      sum_work = sum_work + work;
      workers.push_back(work);
    }
    
  }
  
  return workers;
  
}

///////////////////////////////////////////////////////////

// Data structures used by the BFS algorithm
struct ThreadData {
  int thread_id;
  //pthread_mutex_t *mutex;
  //pthread_barrier_t* barrier;
  int num_nodes;
  int num_robots;
  std::map<int, std::vector<int>> adj_list;
  std::vector<std::tuple<int,int,double,int>> *vec_res;
  int start_node;
  int finish_node;
};

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

// funcion bfs
// regresar vector de resultados 
void bfs(std::map<int, std::vector<int>>& adj_list, std::vector<int>& distance,
	 int num_nodes, bool print,int num_robots,
	 std::vector<std::tuple<int,int,double,int>> &vec_res, int rows, int cols, int start_node, int finish_node){
  
  clock_t start, end;
  
  for(int r = 0; r < num_robots; r++){
    
    //se inicializa en -1 para cada nuevo robot
    distance.assign(num_nodes, -1); //inicializar todas en -1
    
    start = clock();

    //puede ser un vector
    //int start_node = random(0, num_nodes-1);
    //int finish_node = random(0, num_nodes-1);
  
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
      
	if(print){
	  print_dist(rows,cols,distance);
	}
      }
    }
    
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        
    vec_res.emplace_back(start_node,finish_node,time_taken,distance[finish_node]);
  }
}

// trabajo que corre en paralelo
void *parallel_bfs_thread(void *arg) {

  /*
    Se itera aqui respecto al numero de robots 
  */

  ThreadData *data = (ThreadData *)arg;
  int thread_id = data->thread_id;
  //pthread_mutex_t *mutex = data->mutex;
  int num_nodes = data->num_nodes;
  std::map<int,std::vector<int>> adj_list = data->adj_list;
  std::vector<std::tuple<int,int,double,int>> *vec_res = data->vec_res;
  int num_robots = data->num_robots;
  int start_node = data->start_node;
  int finish_node = data->finish_node;
  
  //std::cout << "dentro de parallel_bfs_thread se creo el thread: " << thread_id << " me tocaron:" << num_robots << std::endl; 
  
  clock_t start, end;

  //std::map<int, std::vector<int>> adj_list;
  std::vector<int> distance;//(num_nodes, -1); //cada hilo tiene su vector de distancias
  
  for(int r = 0; r < num_robots; r++){
    distance.assign(num_nodes, -1);
    start = clock();

    //int start_node = random(0, num_nodes-1);
    //int finish_node = random(0, num_nodes-1);

    //std::cout << "dentro de parallel_bfs_thread Nodo Inicio: " << start_node << std::endl;
    //std::cout << "dentro de parallel_bfs_thread Nodo Fin: " << finish_node << std::endl; 
    
    std::queue<int> q;
    distance[start_node] = 0;    //cada quien tiene su fontera
    q.push(start_node);  //cada quien tiene su fontera
    
    // Hasta que la cola se quede sin elementos
    while (!q.empty()) {
      int node = q.front();
      q.pop();
      
      for(int neighbor : adj_list[node]){
	if(distance[neighbor]==-1){
	  distance[neighbor] = distance[node] + 1;
	  q.push(neighbor);
	}
      }
    }
    

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    
    vec_res->emplace_back(start_node,finish_node,time_taken,distance[finish_node]);
    
    //std::cout << "THREADID: " << thread_id << "ROBOTS - " << num_robots << "modifique " << r << std::endl;

  }
  
  // Exit thread
  pthread_exit(NULL);
  
}


void parallel_bfs(std::map<int, std::vector<int>> adj_list,int num_nodes, int num_threads, int num_robots,std::vector<std::tuple<int,int,double,int>> &vec_res, int start_node, int finish_node){

  //void parallel_bfs(std::map<int, std::vector<int>> adj_list,int num_nodes, int num_threads, int num_robots){

  //Se definen inicios random y destinos random
  //int start_node  = random(0, num_nodes-1);
  //int finish_node = random(0, num_nodes-1);
  
  std::vector<int> work_load;
  
  // obtener la carga de trabajo
  // es un vector con la cardinalidad de hilos con el num de trabajo
  // para cada hilo
  // hacer este paso si la opcion es pthreads
  if(num_threads>1)
    work_load = get_workload(num_robots, num_threads);
  else
    work_load = {num_robots};

  /*
  // Saber si el reparto de trabajos se realizo bien
  for(int w=0;w<work_load.size();++w){
     std::cout << work_load[w] << std::endl;
  }
  exit(-1);
  */
    
  // Crear thread data e inicializar
  ThreadData thread_data[num_threads];
  //pthread_t threads[num_threads];
  pthread_t* threads;
  threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);

  clock_t start, end;
  start = clock();
  
  // POOL DE THREADS
  for (int i = 0; i < num_threads; i++) {
    thread_data[i].thread_id = i;
    thread_data[i].num_nodes = num_nodes;
    thread_data[i].num_robots = work_load[i];
    thread_data[i].adj_list = adj_list;
    thread_data[i].vec_res = &vec_res;
    thread_data[i].start_node = start_node;
    thread_data[i].finish_node = finish_node;
    pthread_create(&threads[i], NULL, parallel_bfs_thread, (void *)&thread_data[i]);
  }
  
  // Join de threads
  for (int i = 0; i < num_threads; i++) {
    //std::cout << "TERMINE" << i << std::endl;
    pthread_join(threads[i], NULL);
  }

  end = clock();
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

  std::cout << time_taken << "seg" << std::endl;
  
  // Clean mutex
  //pthread_mutex_destroy(&mutex);

  free(threads);
    
}

//string -> int
constexpr unsigned int str2int(const char* str, int h = 0){
  return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int main(int argc, char* argv[]) {

  clock_t start, end;
  clock_t start_all, end_all;
  
  bool PRINT       = false;
  bool show_result = false;
  //bool show_time   = false;

  //DEFAULT
  int num_threads = 1;
  int num_robots = 1;
  
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

    if(std::string(argv[count]).substr(0,8) == std::string("--robots")){
      std::string prefijo_r("--robots=");
      num_robots = std::stoi(std::string(argv[count]).substr(prefijo_r.size()));
    }

    /*
    if(std::string(argv[count]).substr(0,4) == std::string("--rs")){
      std::string prefijo("--rs=");
      num_threads = std::stoi(std::string(argv[count]).substr(prefijo.size()));

    }
    */

    if(std::string(argv[count]).substr(0,8) == std::string("--robots_file")){
      std::string prefijo_r("--robots_file=");
      
    }
  }
  
  /////////////////////RESULTADOS///////////////////////
  // Donde se guardan los resultados
  std::vector<std::tuple<int,int,double,int>> vec_res;
  //////////////////////////////////////////////////////
  
  int rows;
  int cols;

  std::cin >> rows >> cols;
  
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

  //CUIDAR NO MOSTRAR TANTOS NODOS
  if(num_nodes > 2000 && PRINT){
    std::cout << "NUM NODES: " << num_nodes << " se omite el PRINT" << std::endl; 
    PRINT = false;
  }
  
  int start_node; //nodo inicio, donde se localiza el robot
  int finish_node; //nodo destino
  double time_taken_all;
  
  std::cin >> start_node >> finish_node;
  
  //inicializar vector de distancias en -1
  //respecto a la cardinalidad num_nodes
  //std::vector<int> distance(num_nodes, -1);
  std::vector<int> distance(num_nodes, -1);

  //SE TOMA EL TIEMPO DEL ALGORITMO YA QUE AHI SE PARALELIZA O NO
  
  
  //analizar pasandole el nodo inicio, nodo destino y vector de distancias
  //se analiza todo el mapa
  switch(str2int(MODE)){

  case str2int("--MODE=SECUENCIAL"):
    start_all = clock();
    bfs(adj_list, distance, num_nodes, PRINT, num_robots, vec_res, rows, cols, start_node, finish_node);
    end_all = clock();
    break;

    //se paraleliza por exploracion de robot
  case str2int("--MODE=PTHREADS"):

    //no hay print en este modulo
    if(PRINT)
      std::cout << "NO PRINT EN PTHREADS" << std::endl;
    start_all = clock();
    parallel_bfs(adj_list, num_nodes, num_threads, num_robots, vec_res, start_node, finish_node);
    //parallel_bfs(adj_list, num_nodes, num_threads, num_robots);
    end_all = clock();
    break;

  default:
    
    show_result = false;

    std::cout << "ERROR: EL MODO DEBE SER ELEGIDO. "
	      << "LAS POSIBLES OPCIONES SON: \n"
	      << "--MODE=SECUENCIAL\n"
	      << "--MODE=PTHREADS\n"
	      << std::endl;
    
    return -1;
    
  }  

  
            
  time_taken_all = double(end_all - start_all) / double(CLOCKS_PER_SEC);
  
  if(show_result){
    std::cout << "####################################" << std::endl;
    std::cout << "NODOS: " << num_nodes << std::endl;
    std::cout << MODE << std::endl;
    
    //No hay num threads si es secuencial
    if (str2int("--MODE=SECUENCIAL")!=str2int(MODE)) {
      std::cout << "threads: " << num_threads << std::endl;
    }

    std::cout << "robots: " << num_robots << std::endl;

    //si vec_res no existe no mostrar
    if(vec_res.size()!=0){
      std::cout << "####################################\n"
		<< "RESULTADO\n" 
		<< "####################################"
		<< std::endl;
      
      for(const auto& tpl: vec_res){
	
	std::cout << std::get<0>(tpl) << "," << std::get<1>(tpl) << "," << std::fixed << std::get<2>(tpl) << " distancia: " << std::get<3>(tpl) << std::endl;
	
	if(std::get<3>(tpl) != -1){
	  
	  std::cout << "Distancia del Nodo " << std::get<0>(tpl)
		    << " a " << std::get<1>(tpl)
		    << " es " << std::get<3>(tpl)
		    << " tiempo " << std::get<2>(tpl)
		    << std::endl;
	  std::cout << "####################################" << std::endl;
	  
	}else{
	  
	  std::cout << "No existe un camino del nodo " << std::get<0>(tpl)
		    << " a " << std::get<1>(tpl)
		    << " tiempo " << std::get<2>(tpl)
		    << std::endl;
	  std::cout << "####################################" << std::endl;
	}
      }
    }
  }
  
  
  
  if(show_result)
    
    std::cout << "Tiempo ejecucion : "
	      << std::fixed
	      << time_taken_all << std::setprecision(9)
	      << " seg " << std::endl;
  else{
    //system("clear");
    std::cout << std::fixed
	      << time_taken_all << std::setprecision(9)
	      << std::endl;
  }
  
  
  return 0;
    
}
