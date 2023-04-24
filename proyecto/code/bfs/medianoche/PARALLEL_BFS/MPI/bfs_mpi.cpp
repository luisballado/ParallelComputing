#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <mpi.h>

//#define DEBUG

//compile
//mpic++ bfs_mpi.cpp -o bfs_mpi

//run
//time mpirun -n 1 --use-hwthread-cpus ./bfs_mpi < problem_size/1M.txt  --robots=32 --locations=robot_32.txt --results

//IMPORTANTE --locations=robots.txt debe ser de la cardinalidad de --robots=XX 
//time ./bfs_openmp < problem_size/1M.txt --MODE=OPENMP --robots=10 --nth=2 --locations=robots.txt --results

///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////
// Funcion get_neighbors para obtener los vecinos respecto a la matriz
///////////////////////////////////////////////////////////////////////
std::vector<std::pair<int, int>> get_neighbors(int i, int j, int rows, int cols) {

  std::vector<std::pair<int, int>> neighbors;
  
  if (i > 0) neighbors.push_back(std::make_pair(i-1, j));
  if (i < rows-1) neighbors.push_back(std::make_pair(i+1, j));
  if (j > 0) neighbors.push_back(std::make_pair(i, j-1));
  if (j < cols-1) neighbors.push_back(std::make_pair(i, j+1));
  
  return neighbors;
  
}

///////////////////////////////////////////////////////////////////////
// FUNCION BFS
///////////////////////////////////////////////////////////////////////
void bfs(std::map<int, std::vector<int>>& adj_list, int num_nodes, int mis_robots, std::vector<std::vector<int>>& ubicaciones_robots, int mpiID, int start_robot, int finish_robot,int num_proc){

  clock_t start, end;
  std::vector<int> distance;

  //talvez saber que proceso es
  printf(">> DENTRO DE BFS - PROCESO %d!\n", mpiID);
  
  //saber cuantos robots me tocaron aqui
  //saber la ubicacion de los robots que me tocaron
  //se debe compartir la información al nodo maestro
  
  for(int r = start_robot; r <= finish_robot; r++){
    
    //se inicializa en -1 para cada nuevo robot
    distance.assign(num_nodes, -1); //inicializar todas en -1
    
    start = clock();
        
    //puede ser un vector
    int start_node  = ubicaciones_robots[r][0];
    int finish_node = ubicaciones_robots[r][1];
      
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
    
    end = clock();
    MPI_Status status;
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    //QUITAMOS RESULTADOS
    /*****
    int nodo_inicio = start_node;
    int nodo_fin = finish_node;
    int distancia = distance[finish_node];
      
    // Empaquetar los datos en un búfer de memoria
    int buffer_size = 3 * sizeof(int) + sizeof(double) + 1;
    char* buffer = (char*) malloc(buffer_size);
    int position = 0;
    
    //vec_res.emplace_back(start_node,finish_node,time_taken,distance[finish_node]);
    MPI_Pack(&nodo_inicio, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(&nodo_fin, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(&time_taken, 1, MPI_DOUBLE, buffer, buffer_size, &position, MPI_COMM_WORLD);
    MPI_Pack(&distancia, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
    
    MPI_Send(buffer,buffer_size,MPI_PACKED,0,0,MPI_COMM_WORLD);
    
    if(mpiID == 0){
      //unpack
      for(int p = 1; p < num_proc; p++){
	//printf("dentro del for desempaqutando%d\n",num_proc);
	MPI_Recv(buffer,buffer_size,MPI_PACKED,mpiID,0,MPI_COMM_WORLD,&status);
	position = 0;
	MPI_Unpack(buffer,buffer_size,&position,&nodo_inicio,1,MPI_INT,MPI_COMM_WORLD);
	MPI_Unpack(buffer,buffer_size,&position,&nodo_fin,1,MPI_INT,MPI_COMM_WORLD);
	MPI_Unpack(buffer,buffer_size,&position,&time_taken,1,MPI_DOUBLE,MPI_COMM_WORLD);
	MPI_Unpack(buffer,buffer_size,&position,&distancia,1,MPI_INT,MPI_COMM_WORLD);
	
	vec_res.emplace_back(nodo_inicio,nodo_fin,time_taken,distancia);
      }
    }
    **/
    
    
  }
}

//string -> int
//para identificar banderas
constexpr unsigned int str2int(const char* str, int h = 0){
  return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int ierr;
int mis_robots;
int start_robot;
int finish_robot;

///////////////VECTOR RESULTADOS///////////////////////
std::vector<std::tuple<int,int,double,int>> vec_res;
///////////////////////////////////////////////////////

// PROGRAMA MAIN
int main(int argc, char* argv[]) {

  int num_proc, my_id;
  
  ierr = MPI_Init(&argc, &argv);

  if(ierr != 0){
    std::cout << "\n";
    std::cout << "FATAL ERROR!\n";
    std::cout << " MPI_Init returnet ierr = " << ierr << "\n";
    exit(1);
  }
  
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  
  printf("PROCESO %d!\n", my_id);
  printf("NUM PROC %d!\n", num_proc);
  clock_t start, end;
  clock_t start_all, end_all;
  
  bool show_result = false;
  
  //-----DEFAULT---------
  int num_threads = num_proc;
  int num_robots = 1;
  //---------------------

  std::string location_robots;

  int robot_inicio, robot_fin;
  printf("ANTES DE UBUCACIONES\n");

  //vector para almacenar las ubicaciones
  //el hilo 0 se debe encargar de almacenarlas

  if(my_id == 0){

    
    
  }else{
    
  }
  
  
  std::vector<std::vector<int>> ubicaciones_robots;
  
  for(int count = 0; count < argc; count++ ){
    
    if(std::string(argv[count]).substr(0,9) == std::string("--results")){
      show_result = true;
    }

    if(std::string(argv[count]).substr(0,8) == std::string("--robots")){
      std::string prefijo_r("--robots=");
      num_robots = std::stoi(std::string(argv[count]).substr(prefijo_r.size()));
    }
    
    //ARCHIVO DE ROBOTS
    if(std::string(argv[count]).substr(0,11) == std::string("--locations")){
      std::string prefijo("--locations=");

      //Hacer resize al vector de ubicaciones robot
      
      ubicaciones_robots.resize(num_robots);
      
      location_robots = std::string(argv[count]).substr(prefijo.size()); 
    
      std::ifstream infile(location_robots);

      if (!infile.is_open()) {
        std::cout << "Failed to open file: " << location_robots << "\n";
        return 1;
      }
      
      int _robots_ = 0;
      std::string line;
      
      while (std::getline(infile, line)) {
        std::istringstream iss(line);
	std::string word;

	int count = 0;

	while (iss >> word) {
	  if(count==0){
	    robot_inicio = std::stoi(word);
	    count = 1;
	  }else{
	    robot_fin = std::stoi(word);
	    count = 0;
	  }	  
	}

	ubicaciones_robots[_robots_].push_back(robot_inicio);
	ubicaciones_robots[_robots_].push_back(robot_fin);
	_robots_++;
	
      }
    }
  }
  
  int rows; //fila
  int cols; //columna
  
  //cargar fila columna desde archivo
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
  
  
  std::map<int, std::vector<int>> adj_list;

  //////////////////////////////////////////////////////////////////////
  //crear lista de adyacencia recorriendo matriz creada,
  //se omite el signo # ya que representa una pared incomunicada
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

  MPI_Bcast(&num_nodes, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&mis_robots, 1, MPI_INT, 0, MPI_COMM_WORLD);

  //siempre seran divisibles
  //el nodo llega aqui, calcula y sigue su camino
  if((num_robots%num_proc)==0){
    //A cada uno le tocan partes iguales
    printf("SON DIVISIBLES\n");
  }

  //dividir la carga de trabajo entre los nodos
  //todos los nodos conoces cuantos les tocaron
  mis_robots = num_robots/num_proc;
  start_robot = my_id * mis_robots;
  finish_robot = start_robot + mis_robots -1;
    
  MPI_Barrier(MPI_COMM_WORLD);
      
  //int start_node; //nodo inicio, donde se localiza el robot
  //int finish_node; //nodo destino
  double time_taken_all;
  
  //std::cin >> start_node >> finish_node;
  
  //inicializar vector de distancias en -1
  //respecto a la cardinalidad num_nodes
  
  //std::vector<int> distance(num_nodes, -1);
  
      
  
  //SE TOMA EL TIEMPO DEL ALGORITMO YA QUE AHI SE PARALELIZA O NO
  
  //analizar pasandole el nodo inicio, nodo destino y vector de distancias
  //se analiza todo el mapa

  bfs(adj_list,num_nodes, mis_robots,ubicaciones_robots, my_id, start_robot, finish_robot,num_proc);
      
  if(show_result){
    std::cout << "####################################" << std::endl;
    std::cout << "NODOS: " << num_nodes << std::endl;
    std::cout << "threads: " << num_threads << std::endl;
    std::cout << "robots: " << num_robots << std::endl;

    //si vec_res no existe no mostrar
    if(vec_res.size()!=0){
      std::cout << "####################################\n"
		<< "RESULTADO\n" 
		<< "####################################"
		<< std::endl;

      printf("PROCESO -  %d!\n", my_id);
      
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
  
  MPI_Finalize();
  
  return 0;
    
}
