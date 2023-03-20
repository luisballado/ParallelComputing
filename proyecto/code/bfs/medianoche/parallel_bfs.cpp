#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <pthread.h>

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

/****
struct bfs_thread_args {
    std::map<int, std::vector<int>> adj_list;
    std::vector<int> distance;
    std::queue<int> q;
    pthread_mutex_t* q_mutex;
    pthread_barrier_t* barrier;
    int start_node;
    std::vector<int> nodes;
};



void* bfs_thread(void* args_ptr) {
    bfs_thread_args* args = (bfs_thread_args*) args_ptr;
    std::map<int, std::vector<int>> adj_list = args->adj_list;
    std::vector<int> distance = args->distance;
    std::queue<int> q = args->q;
    pthread_mutex_t* q_mutex = args->q_mutex;
    pthread_barrier_t* barrier = args->barrier;
    int start_node = args->start_node;
    std::vector<int> nodes = args->nodes;

    for (int i = 0; i < nodes.size(); i++) {
        int node = nodes[i];

        if (node == start_node) {
            distance[start_node] = 0;
        }

        for (int neighbor : adj_list[node]) {
            if (distance[neighbor] == -1) {
                distance[neighbor] = distance[node] + 1;
                pthread_mutex_lock(q_mutex);
                q.push(neighbor);
                pthread_mutex_unlock(q_mutex);
            }
        }
    }

    pthread_barrier_wait(barrier);

    return nullptr;
}

void bfs(std::map<int, std::vector<int>>& adj_list, int start_node, std::vector<int>& distance, int num_threads) {
  
  //const int num_threads = 4; // number of threads to use
  
  std::queue<int> q;
  q.push(start_node);
  
  pthread_t threads[num_threads];
  bfs_thread_args thread_args[num_threads];
  
  pthread_mutex_t q_mutex;
  pthread_mutex_init(&q_mutex, nullptr);
  
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, nullptr, num_threads);

  std::vector<int> nodes;
  
  for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
    nodes.push_back(it->first);
  }
  
  int num_nodes = nodes.size();
  int nodes_per_thread = num_nodes / num_threads;
  
  for (int i = 0; i < num_threads; i++) {
    int start_index = i * nodes_per_thread;
    int end_index = (i == num_threads - 1) ? num_nodes : (i + 1) * nodes_per_thread;
    
    thread_args[i].adj_list = adj_list;
    thread_args[i].distance = distance;
    thread_args[i].q = q;
    thread_args[i].q_mutex = &q_mutex;
    thread_args[i].barrier = &barrier;
    thread_args[i].start_node = start_node;
    thread_args[i].nodes = std::vector<int>(nodes.begin() + start_index, nodes.begin() + end_index);
    
    pthread_create(&threads[i], nullptr, bfs_thread, &thread_args[i]);
  }
  
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], nullptr);
  }
  
  pthread_mutex_destroy(&q_mutex);
  pthread_barrier_destroy(&barrier);
}
**/


/***
    struct bfs_thread_args {
    std::map<int, std::vector<int>> adj_list;
    std::vector<int> distance;
    std::queue<int> q;
    pthread_mutex_t* q_mutex;
    pthread_barrier_t* barrier;
    int start_node;
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
    
    void* bfs_thread(void* args_ptr) {
    bfs_thread_args* args = (bfs_thread_args*) args_ptr;
    std::map<int, std::vector<int>> adj_list = args->adj_list;
    std::vector<int> distance = args->distance;
    std::queue<int> q = args->q;
    pthread_mutex_t* q_mutex = args->q_mutex;
    pthread_barrier_t* barrier = args->barrier;
    int start_node = args->start_node;
    
    while (true) {
    pthread_mutex_lock(q_mutex);
    if (q.empty()) {
    pthread_mutex_unlock(q_mutex);
    break;
    }
    int node = q.front();
    q.pop();
    pthread_mutex_unlock(q_mutex);
    
    for (int neighbor : adj_list[node]) {
    if (distance[neighbor] == -1) {
    distance[neighbor] = distance[node] + 1;
    pthread_mutex_lock(q_mutex);
    q.push(neighbor);
    pthread_mutex_unlock(q_mutex);
    }
    }
    }
    
    pthread_barrier_wait(barrier);
    
    return nullptr;
    }
    
    void bfs(std::map<int, std::vector<int>>& adj_list, int start_node, std::vector<int>& distance) {
    const int num_threads = 1; // number of threads to use
    
    std::queue<int> q;
    q.push(start_node);
    
    pthread_t threads[num_threads];
    bfs_thread_args thread_args[num_threads];
    
    pthread_mutex_t q_mutex;
    pthread_mutex_init(&q_mutex, nullptr);
    
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, nullptr, num_threads);
    
    for (int i = 0; i < num_threads; i++) {
    thread_args[i].adj_list = adj_list;
    thread_args[i].distance = distance;
    thread_args[i].q = q;
    thread_args[i].q_mutex = &q_mutex;
    thread_args[i].barrier = &barrier;
    thread_args[i].start_node = start_node;
    
    pthread_create(&threads[i], nullptr, bfs_thread, &thread_args[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], nullptr);
    }
    
    pthread_mutex_destroy(&q_mutex);
    pthread_barrier_destroy(&barrier);
    }
*****/


struct populate_adj_list_thread_args {
    std::vector<std::vector<char>> grid;
    int rows;
    int cols;
    std::map<int, std::vector<int>> adj_list;
    int start_row;
    int end_row;
};

void* populate_adj_list_thread(void* args_ptr) {
    populate_adj_list_thread_args* args = (populate_adj_list_thread_args*) args_ptr;
    std::vector<std::vector<char>> grid = args->grid;
    int rows = args->rows;
    int cols = args->cols;
    std::map<int, std::vector<int>> adj_list = args->adj_list;
    int start_row = args->start_row;
    int end_row = args->end_row;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] != '#') {
                int node = i * cols + j;
                std::vector<std::pair<int, int>> neighbors = get_neighbors(i, j, rows, cols);

                for (auto n : neighbors) {
                    int neighbor_node = n.first * cols + n.second;
                    if (grid[n.first][n.second] != '#') {
                        adj_list[node].push_back(neighbor_node);
                    }
                }
            }
        }
    }

    return nullptr;
}

int main(int argc, char **argv) {
  
  int rows;
  int cols;
  int num_threads = atoi(argv[1]);
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
  
  std::cout << "THREADS " << num_threads << std::endl;
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
