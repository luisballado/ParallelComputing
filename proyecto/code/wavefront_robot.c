//WAVEFRONT

#include <stdio.h>   //para el print .. etc
#include <stdlib.h>  //acceso a system
#include <unistd.h>  //sleep usleep
#include <stdbool.h> //bool stuff
#include <time.h>    // cosas de tiempo

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

//Mundo 1
#define X_SIZE_W1   10
#define Y_SIZE_W1   10

//Mundo 2
#define X_SIZE_W2   12
#define Y_SIZE_W2   12

//Mundo 3
#define X_SIZE_W3   14
#define Y_SIZE_W3   14

//Mundo 4
#define X_SIZE_W4   16
#define Y_SIZE_W4   16

//Mundo 5
#define X_SIZE_W4   18
#define Y_SIZE_W4   18

//Mundo 6
#define X_SIZE_W5   20
#define Y_SIZE_W5   20

#define SPECIAL_ITEM 50  //todo arriba de este valor es un item especial, pared o objetivo

//cosa para los tiempos
clock_t start, end;
double cpu_time_used;

int explore = 0;
int counter = 0;
int steps = 0; //pasos para llegar al resultado

int aux_size; //aux para seleccion de tamanios

//cuando se busca por un nodo con valor minimo
int min_node = 50;
int min_node_x;
int min_node_y;

int min_node_location = 50;

bool done = false;

int route_done = 0;

double tiempos[10];

//X is vertical, Y is horizontal
//mapa mundo 1 10x10
int wolrd1[X_SIZE_W1][Y_SIZE_W1] =
  {{88, 0, 0, 0, 0,88, 0,88,88, 0},
   {88,88, 0, 0, 0,88, 0, 0, 0, 0},
   { 0, 0, 0,88, 0,88, 0,88,88, 0},
   { 0, 0,88,88, 0,88, 0, 0, 0,88},
   { 0,88,88,88, 0, 0, 0, 0,88, 0},
   { 0, 0,88,88,88,88,88, 0,88,88},
   { 0, 0, 0, 0,88,88, 0, 0,88, 0},
   { 0, 0, 0, 0,88,88, 0, 0, 0,88},
   { 0, 0, 0, 0,88, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0,88, 0}
  };

//mapa mundo 2 12x12
int world2[X_SIZE_W2][Y_SIZE_W2] =
  {{88, 0, 0, 0, 0,88, 0,88,88, 0, 0, 0},
   {88,88, 0, 0, 0,88, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0,88, 0,88, 0,88,88, 0, 0, 0},
   { 0, 0,88,88, 0,88, 0, 0, 0,88, 0, 0},
   { 0,88,88,88, 0, 0, 0, 0,88, 0, 0, 0},
   { 0, 0,88,88,88,88,88, 0,88,88, 0, 0},
   { 0, 0, 0, 0,88,88, 0, 0,88, 0, 0, 0},
   { 0, 0, 0, 0,88,88, 0, 0, 0,88, 0, 0},
   { 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0},
   { 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0},
   { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0}
  };

//mapa mundo 3 14x14
int world3[X_SIZE_W3][Y_SIZE_W3] =
  {
    {88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88, 0, 0},
    {88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88, 0, 0},
    {88,88, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0,88, 0,88, 0,88,88, 0, 0, 0, 0, 0},
    { 0, 0,88,88, 0,88, 0, 0, 0,88, 0, 0,88, 0},
    { 0,88,88,88, 0, 0, 0, 0,88, 0, 0, 0,88, 0},
    { 0, 0,88,88,88,88,88, 0,88,88, 0, 0,88, 0},
    { 0, 0, 0, 0,88,88, 0, 0,88, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0,88,88, 0, 0, 0,88, 0, 0,88, 0},
    { 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0, 0,88},
    { 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0}
  };

//mapa mundo 4 16x16
int world4[X_SIZE_W4][Y_SIZE_W4] =
  {
    {88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0},
    {88, 0,88, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0},
    {88, 0,88, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0},
    {88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88, 0, 0, 0, 0},
    {88,88, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0,88, 0,88, 0,88,88, 0, 0, 0, 0, 0,88, 0},
    { 0, 0,88,88, 0,88, 0, 0, 0,88, 0, 0,88, 0,88, 0},
    { 0,88,88,88, 0, 0, 0, 0,88, 0, 0, 0,88, 0,88, 0},
    { 0, 0,88,88,88,88,88, 0,88,88, 0, 0,88, 0,88, 0},
    { 0, 0, 0, 0,88,88, 0, 0,88, 0, 0, 0, 0, 0,88, 0},
    { 0, 0, 0, 0,88,88, 0, 0, 0,88, 0, 0,88, 0,88, 0},
    { 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0, 0,88, 0, 0},
    { 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0, 0, 0}
  };

//mapa mundo 5 18x18
int world5[X_SIZE_W5][Y_SIZE_W5] =
  {
    { 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
    { 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
    { 0,88, 0,88, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
    { 0,88, 0,88, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0,88},
    { 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88, 0, 0, 0, 0,88},
    { 0,88,88, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88},
    { 0, 0, 0, 0,88, 0,88, 0,88,88, 0, 0, 0, 0, 0,88, 0,88},
    { 0, 0, 0,88,88, 0,88, 0, 0, 0,88, 0, 0,88, 0,88, 0,88},
    { 0, 0,88,88,88, 0, 0, 0, 0,88, 0, 0, 0,88, 0,88, 0,88},
    { 0, 0, 0,88,88,88,88,88, 0,88,88, 0, 0,88, 0,88, 0,88},
    { 0, 0, 0, 0, 0,88,88, 0, 0,88, 0, 0, 0, 0, 0,88, 0,88},
    { 0, 0, 0, 0, 0,88,88, 0, 0, 0,88, 0, 0,88, 0,88, 0,88},
    { 0, 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0, 0,88, 0, 0,88},
    { 0, 0, 0, 0, 0,88, 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0,88},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0, 0, 0, 0},
    { 0,88, 0, 0, 0, 0,88, 0,88,88, 0, 0,88,88, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0,88, 0, 0, 0,88, 0, 0, 0, 0}
  };

//mapa mundo 6 20x20
int world6[X_SIZE_W6][Y_SIZE_W6] =
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
int main(void)  {
    
  /*
   * Explorar solucion y de no encontrar
   * parar el ciclo y reportarlo
   */

  //Hacer la prueba con MAX_ROBOTS aleatorios
  for(int i=0;i<MAX_ROBOTS;i++){

    clear_map(); //limpiar mapa

    //TODO: Cambiar la seleccion de los mundos
    switch(i){
    case 1:

      aux_size_x = X_SIZE_W1;
      aux_size_y = Y_SIZE_W1;
      
      break;
    case 2:

      aux_size_x = X_SIZE_W2;
      aux_size_y = Y_SIZE_W2;
            
      break;
    case 3:

      aux_size_x = X_SIZE_W3;
      aux_size_y = Y_SIZE_W3;
      
      break;
    case 4:

      aux_size_x = X_SIZE_W4;
      aux_size_y = Y_SIZE_W4;
            
      break;
    case 5:
      
      aux_size_x = X_SIZE_W5;
      aux_size_y = Y_SIZE_W5;
      
      break;
    case 6:
      
      aux_size_x = X_SIZE_W6;
      aux_size_y = Y_SIZE_W6;

      break;
    deault:
      printf("Esto no debe pasar");
      exit(0);
      break;
    }
    

    //obtener ubicacion robot random
    int robotx = get_random(aux_size_x-1);
    int roboty = get_random(aux_size_y-1);

    //obtener ubicacion goal random
    int goalx = get_random(aux_size_x-1);
    int goaly = get_random(aux_size_y-1);
    

    //evitar que sean los mismos
    if((goalx == robotx) && (goaly == roboty)){
      int goalx = get_random(aux_size_x-1);
      int goaly = get_random(aux_size_y-1);
    }

    start = clock();
    // Medir tiempo de este
    explore = propagate_wavefront(robotx,roboty,goalx,goaly,aux_size);
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
void map_init(int rx, int ry, int gx, int gy, int world){

  system("clear"); //limpiar pantalla
  show_map();
  printf(RED "Se comienza Wavefront\n\n" RESET); //imprimir texto
  sleep(1);  // dormir para visualizar el texto antes de limpiar
  
  switch(world){
  case 1:
    world1[rx][ry] = ROBOT; //Agregar el robot en el mapa
    world1[gx][gy] = GOAL; //Agregar el objetivo en el mapa
    break;
  case 2:
    world2[rx][ry] = ROBOT; //Agregar el robot en el mapa
    world2[gx][gy] = GOAL; //Agregar el objetivo en el mapa
    break;
  case 3:
    world3[rx][ry] = ROBOT; //Agregar el robot en el mapa
    world3[gx][gy] = GOAL; //Agregar el objetivo en el mapa
    break;
  case 4:
    world4[rx][ry] = ROBOT; //Agregar el robot en el mapa
    world4[gx][gy] = GOAL; //Agregar el objetivo en el mapa
    break;
  case 5:
    world5[rx][ry] = ROBOT; //Agregar el robot en el mapa
    world5[gx][gy] = GOAL; //Agregar el objetivo en el mapa
    break;
  case 6:
    world6[rx][ry] = ROBOT; //Agregar el robot en el mapa
    world6[gx][gy] = GOAL; //Agregar el objetivo en el mapa
    break;
  default:
    break;
  }
  
  show_map(); //Pintar el mapa
  printf(RED "Se incluyo el objetivo y el robot\n\n" RESET);
  sleep(1);
}

/******************************
 * Propagar el frente de onda
 * return numero de iteraciones 
 ******************************/
int propagate_wavefront(int rx, int ry, int gx, int gy, int map_size){
    
  int min_neighbor;
  counter = 0;
  
  map_init(rx,ry,gx,gy,map_size); // Cargar las cosas al mapa
  
  //ciclos para pasadas de exploracion
  while(counter<=MAX_EXP){
    
    int x = 0;
    int y = 0;
    
    // hacer hasta explorar todo el mapa
    while((x < map_size) && (y < map_size)){
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

	  //TODO MEJORAR SELECCION DE MUNDOS
	  if(map_size == 1){
	    world1[x][y] = min_node + 1;
	  }else if (map_size == 2){
	    world2[x][y] = min_node + 1;
	  }else if (map_size == 3){
	    world3[x][y] = min_node + 1;
	  }else if (map_size == 4){
	    world4[x][y] = min_node + 1;
	  }else if (map_size == 5){
	    world5[x][y] = min_node + 1;
	  }else if (map_size == 6){
	    world6[x][y] = min_node + 1;
	  }
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
int explore_neighbors(int x, int y, int map_size, int world){

  min_node = SPECIAL_ITEM; //reset minimum
    
  // abajo
  if(x < map_size-1) //no salirse de la matrix

    //TODO MEJORAR SELECCION DE MUNDOS
    if(map_size == 1){
      world1[x][y] = min_node + 1;
      //encontrar el nodo de menor valor excluyendo los vacios
      if ((world1[x+1][y] < min_node) && (world1[x+1][y] != EMPTY)){
	min_node = world2[x+1][y];
	min_node_location = 3;
	min_node_x = x+1;
	min_node_y = y;
      }
    }else if (map_size == 2){
      world2[x][y] = min_node + 1;
      if ((world2[x+1][y] < min_node) && (world2[x+1][y] != EMPTY)){
	min_node = world2[x+1][y];
	min_node_location = 3;
	min_node_x = x+1;
	min_node_y = y;
      }
    }else if (map_size == 3){
      world3[x][y] = min_node + 1;
      if ((world3[x+1][y] < min_node) && (world3[x+1][y] != EMPTY)){
	min_node = world3[x+1][y];
	min_node_location = 3;
	min_node_x = x+1;
	min_node_y = y;
      }
    }else if (map_size == 4){
      world4[x][y] = min_node + 1;
      if ((world4[x+1][y] < min_node) && (world4[x+1][y] != EMPTY)){
	min_node = world4[x+1][y];
	min_node_location = 3;
	min_node_x = x+1;
	min_node_y = y;
      }
    }else if (map_size == 5){
      world5[x][y] = min_node + 1;
      if ((world5[x+1][y] < min_node) && (world5[x+1][y] != EMPTY)){
	min_node = world5[x+1][y];
	min_node_location = 3;
	min_node_x = x+1;
	min_node_y = y;
      }
    }else if (map_size == 6){
      world6[x][y] = min_node + 1;
      if ((world6[x+1][y] < min_node) && (world6[x+1][y] != EMPTY)){
	min_node = mapa[x+1][y];
	min_node_location = 3;
	min_node_x = x+1;
	min_node_y = y;
      }
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
  if(y < map_size-1)


    if ((mapa[x][y+1] < min_node) && (mapa[x][y+1] != EMPTY)){
      min_node = mapa[x][y+1];
      min_node_location=2;
      min_node_x = x;
      min_node_y = y+1;
    }

  return min_node;

}

//limpiar el mapa
void clear_map(int map_size){	
  
  printf("limpiando....\n");
    
  for (int x=0; x<map_size; x++)
    for (int y=0; y<map_size; y++)
      if (mapa[x][y] != WALL) //si la ubicacion es pared ignorar
	mapa[x][y] = EMPTY; //limpiar espacio
  
  printf("mapa limpiado\n");
  show_map();
}

/*
 * Imprimir mapa
 * TODO: pasarle los tamanos
 * respecto al mapa a mostrar
 */
void show_map(int map_size){
  
  //usleep(2000);
  system("clear");
  
  //recorrer el arreglo
  for (int i=0;i<map_size;i++){
    for (int j=0;j<map_size;j++){
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
