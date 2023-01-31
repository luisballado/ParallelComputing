//WAVEFRONT

#include <stdio.h>  //para el print .. etc
#include <stdlib.h> //acceso a system
#include <unistd.h> //sleep usleep

#include <stdbool.h> //bool stuff

/*
 * Definir colores fancy
 */
#define RESET "\x1B[0m"
#define RED "\e[0;31m"
#define GRNB "\e[42m"
#define WHTHB "\e[0;107m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"

/*
 * Variables HARDCODE
 */
#define MAX_EXP  20
#define EMPTY     0
#define PARED    88
#define ROBOT     1
#define GOAL    100

#define X_SIZE   7
#define Y_SIZE   7

#define SPECIAL_ITEM 50  //todo arriba de este valor es un item especial, pared o objetivo

#define MAX_ROBOTS 5 //Numero total de robots

/*
 * ubicacion robot
 * TODO: Hacer varios robots 
 */

//int robot_x = 6; 
//int robot_y = 6; 

int x = 0;
int y = 0;

/*
 * ubicacion objetivo
 */
//int goal_x = 0;
//int goal_y = 6;

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

//X is vertical, Y is horizontal
/**
int mapa[X_SIZE][Y_SIZE] = {{88,88,88,0,  0, 0, 0},
			    { 0, 0, 0, 0,88,88, 0},
			    { 0, 0, 0,88, 0,88, 0},
			    { 0, 0,88,88, 0,88, 0},
			    { 0,88,88,88, 0, 0, 0},
			    { 0,88,88,88,88,88,88},
			    { 0, 0, 0, 0, 0, 0, 0}};
*/

/**
int mapa[X_SIZE][Y_SIZE] = {{0, 0, 0, 0, 0, 0, 0},
			    {0,88,88,88,88,88, 0},
			    {0, 0,88, 0,88,88, 0},
			    {88,0,88, 0,88,88, 0},
			    {0, 0,88, 0, 0, 0, 0},
			    {0,88,88,88,88,88,88},
			    {0, 0, 0, 0, 0, 0, 0}};
*/
/**
int mapa[X_SIZE][Y_SIZE] = {{88, 0, 0, 0, 0,88, 0},
			    {88,88, 0, 0, 0,88, 0},
			    { 0, 0, 0,88, 0,88, 0},
			    { 0, 0,88,88, 0,88, 0},
			    { 0,88,88,88, 0, 0, 0},
			    { 0, 0,88,88,88,88,88},
			    { 0, 0, 0, 0, 0, 0, 0}};
**/

int mapa[X_SIZE][Y_SIZE] = {{ 0,88,88, 0, 0,88, 0},
			    { 0,88, 0, 0, 0,88, 0},
			    { 0, 0, 0,88, 0, 0, 0},
			    { 0, 0, 0,88,88,88,88},
			    { 0,88, 0, 0, 0, 0, 0},
			    { 0,88,88,88,88,88,88},
			    { 0, 0, 0, 0, 0, 0, 0}};


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
  for(int i=1;i<=MAX_ROBOTS;i++){

    clear_map(); //limpiar mapa
    
    //obtener ubicacion robot random
    int robotx = get_random(X_SIZE-1);
    int roboty = get_random(Y_SIZE-1);

    //obtener ubicacion goal random
    int goalx = get_random(X_SIZE-1);
    int goaly = get_random(Y_SIZE-1);

    //evitar que sean los mismos
    if((goalx == robotx) && (goaly == roboty)){
      int goalx = get_random(X_SIZE-1);
      int goaly = get_random(Y_SIZE-1);
    }
  
    // Medir tiempo de este
    explore = propagate_wavefront(robotx,roboty,goalx,goaly);
    
    //printf("Ciclos de exploracion: %d\n\n\n",explore);
    //printf("Pasos: %d\n\n\n",steps);
    printf("min_node_XY: %d,%d\n",min_node_x,min_node_y);
    
    //Explorar para sacar la ruta
    if(done!=0){
      //se para donde tengo que ir
      //se la ubicacion
      //hacer un ciclo
      do{
	
	explore_neighbors(min_node_x,min_node_y);
	printf("min_node_XY: %d,%d\n",min_node_x,min_node_y);
	
	if(mapa[min_node_x][min_node_y]==1)
	  route_done = 1;
	
	printf("route - %d\n",route_done);

      }while(route_done <= 0);
	
      //show_map();
      // return route_done;
      
    }
    //
    
    //Para evitar loops grandes
    if(explore == MAX_EXP){
      printf(RED "Problema muy grande o no encontre solucion\n\n" RESET);
    }
    
    sleep(2);

    
    
  }
  
  //return 0;

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

/******************************
 * Propagar el frente de onda
 * return numero de iteraciones 
 ******************************/
int propagate_wavefront(int rx, int ry, int gx, int gy){
    
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
      if ((mapa[x][y] != PARED) && (mapa[x][y] != ROBOT)){	
	
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
	  //printf(GRNB "-%d-\n\n" RESET,minimum_node);
	  //sleep(2);
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

  min_node = SPECIAL_ITEM; //reset minimum
    
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
  
  printf("cleaning....\n");
    
  //stay within boundary
  for (int x=0; x<Y_SIZE; x++)
    for (int y=0; y<Y_SIZE; y++)
      if (mapa[x][y] != PARED) //if this location is a wall or goal, just ignore it
	mapa[x][y] = EMPTY; //limpiar espacio
  
  printf("new scenario compleated:\n");
  show_map();
}

/*
 * Imprimir mapa
 */
void show_map(void){

  usleep(2000);
  //sleep(1);
  system("clear");

  //recorrer el arreglo
  for (int i=0;i<X_SIZE;i++){
    for (int j=0;j<Y_SIZE;j++){
      if (mapa[i][j]==PARED)
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

int get_random(int upper){
    int i;
    int num;
    for (i = 0; i < 1; i++) {
        num = (rand() %
        (upper - 0 + 1)) + 0;
        //printf("%d ", num);
    }
    return num;
}
