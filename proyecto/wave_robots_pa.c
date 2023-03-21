//WAVEFRONT
//TODO: HACER ENTRADAS DINAMICAS PARA DIFERENTES TIPOS DE PROBLEMAS

//https://www.digitalocean.com/community/tutorials/two-dimensional-array-in-c-plus-plus
#include <pthread.h>

#include <stdio.h>  //para el print .. etc
#include <stdlib.h> //acceso a system
#include <unistd.h> //sleep usleep
#include <stdbool.h> //bool stuff
#include <time.h>

/*
 * Colores fancy
 */

#define RESET "\x1B[0m"
#define RED "\e[0;31m"
#define GRNB "\e[42m"
#define WHTHB "\e[0;107m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"

/*
 * Cosas del Mapa
 */

#define MAX_ROBOTS 10 //Numero total de robots

#define MAX_EXP  20
#define EMPTY     0
#define WALL     88
#define ROBOT     1
#define GOAL    100

#define X_SIZE   20
#define Y_SIZE   20

#define SPECIAL_ITEM 50  //todo arriba de este valor es un item especial, pared o objetivo
#define NUM_THREADS 4 // number of threads to use

typedef struct {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
} thread_args_t;

//cosa para los tiempos
clock_t start, end;
double cpu_time_used;

int explore = 0;
int counter = 0;
int steps = 0; //pasos para llegar al resultado

//cuando se busca por un nodo con valor minimo
int min_node = 50;
int min_node_x;
int min_node_y;

int min_node_location = 50;

bool done = false;

int route_done = 0;

double tiempos[10];

//X is vertical, Y is horizontal
int mapa[X_SIZE][Y_SIZE] =
  {
   { 0,88, 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
   { 0, 0, 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
   {88,88,88,88, 0,88, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
   { 0, 0, 0,88, 0,88, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
   { 0, 0, 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88, 0, 0, 0, 0,88},
   {88, 0, 0,88,88, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88},
   {88, 0, 0, 0, 0, 0,88, 0,88, 0,88,88, 0, 0, 0, 0, 0,88, 0,88},
   {88,88, 0, 0, 0,88,88, 0,88, 0, 0, 0,88, 0, 0,88, 0,88, 0,88},
   {88, 0, 0, 0,88,88,88, 0, 0, 0, 0,88, 0, 0, 0,88, 0,88, 0,88},
   {88,88, 0, 0, 0,88,88,88,88,88, 0,88,88, 0, 0,88, 0,88, 0,88},
   {88,88, 0, 0, 0, 0, 0,88,88, 0, 0,88, 0, 0, 0, 0, 0,88, 0,88},
   { 0, 0, 0, 0, 0, 0, 0,88,88, 0, 0, 0,88, 0, 0,88, 0,88, 0,88},
   {88,88, 0, 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88},
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0, 0,88, 0, 0,88},
   {88,88, 0, 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0,88},
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0, 0, 0, 0},
   {88,88, 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0, 0, 0, 0},
   { 0, 0, 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0, 0},
   { 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0, 0, 0, 0},
   { 0,88, 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0, 0}
  };


/*
 * Se declaran las funciones para compilar
 */
void map_init();
int propagate_wavefront(int rx, int ry, int gx, int gy);
int explore_neighbors(int x, int y);
void clear_map();
void show_map(void);
int get_random(int upper);

/*
 * Programa Principal
 */
int main(void){
  
  /*
   * Explorar solucion y de no encontrar
   * parar el ciclo y reportarlo
   */

  //Hacer la prueba con MAX_ROBOTS aleatorios
  for(int i=0;i<MAX_ROBOTS;i++){

    clear_map(); //limpiar mapa
    
    //obtener ubicacion robot random
    int robotx = get_random(X_SIZE-1);
    int roboty = get_random(Y_SIZE-1);

    //obtener ubicacion goal random
    int goalx = get_random(X_SIZE-1);
    int goaly = get_random(Y_SIZE-1);

    //evitar que sean los mismos
    if((goalx == robotx) && (goaly == roboty)){
      goalx = get_random(X_SIZE-1);
      goaly = get_random(Y_SIZE-1);
    }

    start = clock();
    // Medir tiempo de este
    explore = propagate_wavefront(robotx,roboty,goalx,goaly);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    tiempos[i] = cpu_time_used;
    
    printf(RED "El tiempo es: %f\n" RESET , cpu_time_used);
    
    //Para evitar loops grandes
    if(explore == MAX_EXP){
      printf(RED "Problema muy grande o no encontre solucion\n\n" RESET);
    }

    sleep(1);
  }
  
  for(int loop = 0; loop < MAX_ROBOTS; loop++)
    printf("Robot %d: %f\n", loop+1 ,tiempos[loop]);
}

/***************************
 * - Inicializar mapa -
 * Limpiar la consola
 * Mostrar mapa
 * Agregar el robot
 * Agregar el objetivo
 * Mostrar mapa con objetos
 ***************************/
void map_init(int rx, int ry, int gx, int gy){

  system("clear"); //limpiar pantalla
  show_map();
  printf(RED "Se comienza Wavefront\n\n" RESET); //imprimir texto
  sleep(1);  // dormir para visualizar el texto antes de limpiar
  mapa[rx][ry] = ROBOT; //Agregar el robot en el mapa
  mapa[gx][gy] = GOAL; //Agregar el objetivo en el mapa
  show_map(); //Pintar el mapa
  printf(RED "Se incluyo el objetivo y el robot\n\n" RESET);
  sleep(1);
}

/*********
void* explore_map(void* arg){
  ThreadArgs* args = (ThreadArgs*)arg;

  // Loop through the assigned sub-region and explore each node
  for (int x = args->start_x; x <= args->end_x; x++){
    for (int y = args->start_y; y <= args->end_y; y++){
      if ((mapa[x][y] != WALL) && (mapa[x][y] != ROBOT)){
        min_neighbor = explore_neighbors(x,y);

        // se ha encontrado el objetivo
        if ((min_neighbor < SPECIAL_ITEM) && (mapa[x][y] == GOAL)){
          done = true;
          min_node_location = // update min_node_location here

          return (void*)min_node_location;
        }

        //marcar el nodo como visitado
        //indicar el costo para visitarlo
        else if (min_node != SPECIAL_ITEM){
          mapa[x][y] = min_node + 1;
        }
      }
    }
  }

  return NULL;
}
*********/
void *propagate_wavefront_thread(void *args) {
    thread_args_t *thread_args = (thread_args_t *) args;

    int min_neighbor;
    
    //ciclos para pasadas de exploracion
    while (counter <= MAX_EXP) {
        // hacer hasta explorar todo el mapa
        for (int x = thread_args->start_x; x < thread_args->end_x; x++) {
            for (int y = thread_args->start_y; y < thread_args->end_y; y++) {
                // si la ubicacion actual es diferente a pared y el objetivo
                if ((mapa[x][y] != WALL) && (mapa[x][y] != ROBOT)) {

                    min_neighbor = explore_neighbors(x, y);

                    // se ha encontrado el objetivo
                    if ((min_neighbor < SPECIAL_ITEM) && (mapa[x][y] == GOAL)) {
		      show_map(); //mostrar mapa
		      done = true;
                        min_node_location = (x << 16) | y; // Save the location of the min node
                        return NULL;
                    }

                    // marcar el nodo como visitado
                    // indicar el costo para visitarlo
                    else if (min_node != SPECIAL_ITEM) {
                        mapa[x][y] = min_node + 1;
                    }
                }
            }
        }

        counter++;
    }

    return NULL;
}

/**
int propagate_wavefront(int rx, int ry, int gx, int gy) {
    map_init(rx, ry, gx, gy);
    // divide the map into portions to be explored by the threads
    int x_step = X_SIZE / NUM_THREADS;
    int y_step = Y_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i].x_start = i * x_step;
        tasks[i].x_end = (i == NUM_THREADS - 1) ? X_SIZE - 1 : (i + 1) * x_step - 1;
        tasks[i].y_start = i * y_step;
        tasks[i].y_end = (i == NUM_THREADS - 1) ? Y_SIZE - 1 : (i + 1) * y_step - 1;
    }
    // initialize the mutex and barrier
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    // create the threads
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, explore_map, &thread_ids[i]);
    }

    // wait for the threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
    }
    // destroy the mutex and barrier
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    return counter;
}
**/

int propagate_wavefront(int rx, int ry, int gx, int gy) {
    int min_neighbor;
    counter = 0;

    map_init(rx, ry, gx, gy); // Cargar las cosas al mapa

    // Define the number of threads and their arguments
    int num_threads = 4; // Change this value to the desired number of threads
    thread_args_t thread_args[num_threads];
    int chunk_size_x = X_SIZE / num_threads;
    int chunk_size_y = Y_SIZE / num_threads;

    pthread_t threads[num_threads];

    // Start the threads
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].start_x = i * chunk_size_x;
        thread_args[i].start_y = i * chunk_size_y;
        thread_args[i].end_x = (i + 1) * chunk_size_x;
        thread_args[i].end_y = (i + 1) * chunk_size_y;

        pthread_create(&threads[i], NULL, propagate_wavefront_thread, &thread_args[i]);
    }

    // Join the threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return counter;
}



/******************************
 * Propagar el frente de onda
 * return numero de iteraciones 
 ******************************/
int propagate_wavefront_serial(int rx, int ry, int gx, int gy){
    
  int min_neighbor;
  counter = 0;
  
  map_init(rx,ry,gx,gy); // Cargar las cosas al mapa
  
  //ciclos para pasadas de exploracion
  while(counter<=MAX_EXP){
    
    int x = 0;
    int y = 0;
    
    // hacer hasta explorar todo el mapa
    while((x < X_SIZE) && (y < Y_SIZE)){
      //      exit(0);
      // si la ubicacion actual es diferente a pared y el objetivo
      if ((mapa[x][y] != WALL) && (mapa[x][y] != ROBOT)){	
	
	show_map(); //mostrar mapa
	min_neighbor = explore_neighbors(x,y);
	
	// se ha encontrado el objetivo
	if ((min_neighbor < SPECIAL_ITEM) && (mapa[x][y] == GOAL)){
	  
	  show_map();
	  done = true;
	  printf(GRNB "-termine-\n\n" RESET);
	  
	  return min_node_location;
	}
	
	//marcar el nodo como visitado
	//indicar el costo para visitarlo
	else if (min_node != SPECIAL_ITEM){
	  mapa[x][y] = min_node + 1;
	}
      }
      
      //hacer exploracion por fila bajo la cardinalidad del mapa
      y++;
      if ((y==Y_SIZE) && (x!=X_SIZE)){
	x++;
	y=0;
      }	
    }
    
    printf("#:%d\n",counter+1);
    show_map();
    counter++;
  }
  return counter;
}

//de un nodo, explorar y regresar el valor mas bajo
//min_node_location - ubicacion node 1 arriba, 2 derecha, 3 abajo, 4 izquierda
int explore_neighbors(int x, int y){

  min_node = SPECIAL_ITEM; //reset al minimo
    
  // abajo
  if(x < X_SIZE-1) //no salirse de la matrix
    //encontrar el nodo de menor valor excluyendo los vacios
    if ((mapa[x+1][y] < min_node) && (mapa[x+1][y] != EMPTY)){
      min_node = mapa[x+1][y];
      min_node_location = 3;
      min_node_x = x+1;
      min_node_y = y;
    }
  
  // arriba
  if(x > 0)
    if ((mapa[x-1][y] < min_node) && (mapa[x-1][y] != EMPTY)){
      min_node = mapa[x-1][y];
      min_node_location=1;
      min_node_x = x-1;
      min_node_y = y;
    }
  
  // izquierda
  if(y > 0)
    if ((mapa[x][y-1] < min_node) && (mapa[x][y-1] != EMPTY)){
      min_node = mapa[x][y-1];
      min_node_location=4;
      min_node_x = x;
      min_node_y = y-1;
    }
  
  // derecha
  if(y < Y_SIZE-1)
    if ((mapa[x][y+1] < min_node) && (mapa[x][y+1] != EMPTY)){
      min_node = mapa[x][y+1];
      min_node_location=2;
      min_node_x = x;
      min_node_y = y+1;
    }

  return min_node;

}

//limpiar el mapa
void clear_map(){	
  
  printf("limpiando....\n");
    
  //no salirse de la matriz
  for (int x=0; x<Y_SIZE; x++)
    for (int y=0; y<Y_SIZE; y++)
      if (mapa[x][y] != WALL) //si es la pared ignorar
	mapa[x][y] = EMPTY; //limpiar espacio
  
  printf("mapa limpiado\n");
  show_map();
}

/*
 * Imprimir mapa
 */
void show_map(void){
  
  //usleep(2000);
  system("clear");
  
  //recorrer el arreglo
  for (int i=0;i<X_SIZE;i++){
    for (int j=0;j<Y_SIZE;j++){
      if (mapa[i][j]==WALL)
	printf(WHTHB "####" RESET);
      else if (mapa[i][j] == ROBOT)
	printf(BYEL " -R-" RESET);
      else if (mapa[i][j] == GOAL)
	printf(BBLU " -X-" RESET);
      else if (mapa[i][j] == 44)
	printf(BYEL " -R-" RESET);
      else
	if(mapa[i][j]>9){
	  printf(" %d " , mapa[i][j]);
	}else{
	  printf("  %d " , mapa[i][j]);
	}
    }
    printf("\n"); //salto de linea para el recorrido Y
  }
  
  printf("\n"); //salto de linea para mostrar texto abajo
  
  steps++;
}

//funcion para generar un numero random dentro de un rango 0-RANGO
int get_random(int upper){
    
    int num;

    for (int i = 0; i < 3; i++) {
        num = (rand() % (upper - 0 + 1)) + 0;
    }

    return num;
}
