//WAVEFRONT

#include <stdio.h>  //para el print .. etc
#include <stdlib.h> //acceso a system
#include <unistd.h> //sleep usleep

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
#define MAX_EXP 20
#define EMPTY    0
#define PARED  255
#define ROBOT    1
#define GOAL   254

#define X_SIZE   7
#define Y_SIZE   7

/*
 * ubicacion robot
 */
int robot_x = 6; 
int robot_y = 6; 

/*
 * ubicacion objetivo
 */
int goal_x = 0;
int goal_y = 6;

int explore = 0;
int counter = 0;
int steps = 0; //pasos para llegar al resultado

//cuando se busca por un nodo con valor minimo
int minimum_node = 250;
int min_node_location = 250;

int reset_min = 250; //todo arriba de este valor es un item especial, pared o el robot

//X is vertical, Y is horizontal
/**
int mapa[X_SIZE][Y_SIZE] = {{255,255,255,0,  0,  0,  0},
			    {0,  0,  0,  0,255,255,  0},
			    {0,  0,  0,255,  0,255,  0},
			    {0,  0,255,255,  0,255,  0},
			    {0,255,255,255,  0,  0,  0},
			    {0,255,255,255,255,255,255},
			    {0,  0,  0,  0,  0,  0,  0}};
*/

/**
int mapa[X_SIZE][Y_SIZE] = {{0,  0,  0,  0,  0,  0,  0},
			    {0,255,255,255,255,255,  0},
			    {0,  0,255,  0,255,255,  0},
			    {255,0,255,  0,255,255,  0},
			    {0,  0,255,  0,  0,  0,  0},
			    {0,255,255,255,255,255,255},
			    {0,  0,  0,  0,  0,  0,  0}};
*/
/**
int mapa[X_SIZE][Y_SIZE] = {{255,0,  0,  0,  0,  0,  0},
			    {0,255,  0,  0,255,255,  0},
			    {0,  0,  0,255,  0,255,  0},
			    {0,  0,255,255,  0,255,  0},
			    {0,255,255,255,  0,  0,  0},
			    {0,  0,255,255,255,255,255},
			    {0,  0,  0,  0,  0,  0,  0}};
*/

int mapa[X_SIZE][Y_SIZE] = {{255,255,255,  0,  0,  0,  0},
			    {  0,  0,  0,  0,  0,  0,  0},
			    {  0,  0,  0,255,  0,255,  0},
			    {  0,  0,  0,  0,  0,255,  0},
			    {  0,255,  0,  0,  0,  0,  0},
			    {  0,255,255,255,255,255,255},
			    {  0,  0,  0,  0,  0,  0,  0}};


/*
 * Se declaran las funciones para compilar
 */
void map_init(void);
int propagate_wavefront(int robot_x, int robot_y);
int min_neighbor(int x, int y);
void show_map(void);

/*
 * Programa Principal
 */
int main(void){
  
  /*
   * Explorar solucion y de no encontrar
   * parar el ciclo y reportarlo
   */

  // Medir tiempo de este
  explore = propagate_wavefront(robot_x,robot_y);

  printf("Ciclos de exploracion: %d\n\n\n",explore);
  printf("Pasos: %d\n\n\n",steps);
      
  if(explore == MAX_EXP){
    printf(RED "Problema muy grande o no encontre solucion\n\n" RESET);
  }
  
  return 0;
}

/*******************
 * Inicializar mapa
 ******************/
void map_init(void){

  system("clear"); //limpiar pantalla
  printf(RED "Se comienza Wavefront\n\n" RESET); //imprimir texto
  sleep(1);  // dormir para visualizar el texto antes de limpiar
  
  //Agregar el robot en el mapa
  mapa[robot_x][robot_y]=ROBOT;
  
  //Agregar el objetivo en el mapa
  mapa[goal_x][goal_y]=GOAL; //poner el objetivo en el mapa
  
  //Pintar el mapa
  show_map();
  
  printf(RED "Se incluyo el objetivo y el robot\n\n" RESET);
  sleep(1);

}

int propagate_wavefront(int robot_x, int robot_y){

  int x,y;

  counter = 0;
  
  map_init(); // Cargar las cosas al mapa
  
  //ciclos para pasadas de exploracion
  while(counter<50){
        
    x = 0;
    y = 0;

    // hacer hasta explorar todo el mapa
    while(x<X_SIZE && y<Y_SIZE){
      
      // si la ubicacion actual es diferente a pared y el objetivo
      if (mapa[x][y] != PARED && mapa[x][y] != ROBOT){	
	
	show_map(); //mostrar mapa
	
	// se ha encontrado el objetivo
	if (min_neighbor(x, y) < reset_min && mapa[x][y] == GOAL){
	  
	  show_map();
	  
	  printf(GRNB "-termine-\n\n" RESET);
	  //exit(0);
	  //termine!
	  //hacer ciclo para mover al robot
	  return counter; //min_node_location;
	}
	
	//marcar el nodo como visitado
	//indicar el costo para visitarlo
	//record a value in to this node
	//if this isnt here, 'nothing' will go in the location
	else if (minimum_node!=reset_min){
	  //printf(GRNB "-%d-\n\n" RESET,minimum_node);
	  //sleep(2);
	  mapa[x][y] = minimum_node + 1;
	}
      }

      //hacer exploracion por fila bajo la cardinalidad del mapa
      y++;
      if (y==X_SIZE && x!=Y_SIZE){
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
int min_neighbor(int x, int y){

  minimum_node = reset_min; //reset minimum
  
  // abajo
  if(x < X_SIZE-1) //not out of boundary
    //find the lowest number node, and exclude empty nodes (0's)
    if  (mapa[x+1][y] < minimum_node && mapa[x+1][y] != EMPTY){
      minimum_node = mapa[x+1][y];
      min_node_location = 3;
    }
  
  // arriba
  if(x > 0)
    if  (mapa[x-1][y] < minimum_node && mapa[x-1][y] != EMPTY){
      minimum_node = mapa[x-1][y];
      min_node_location=1;
    }
  
  // izquierda
  if(y > 0)
    if  (mapa[x][y-1] < minimum_node && mapa[x][y-1] != EMPTY){
      minimum_node = mapa[x][y-1];
      min_node_location=4;
    }
  
  // derecha
  if(y < Y_SIZE-1)
    if  (mapa[x][y+1] < minimum_node && mapa[x][y+1] != EMPTY){
      minimum_node = mapa[x][y+1];
      min_node_location=2;
    }
  
  

  return minimum_node;

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
	printf(WHTHB "  # " RESET);
      else if (mapa[i][j] == ROBOT)
	printf(BYEL "  R " RESET);
      else if (mapa[i][j] == GOAL)
	printf(BBLU "  X " RESET);
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
