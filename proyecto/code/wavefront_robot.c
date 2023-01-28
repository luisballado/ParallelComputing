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

#define MAX_EXP 20

/*
 * Cosas del mapa
 */
int vacio=0;
int pared=255;
int objetivo=1;
int robot=254;

/*
 * ubicacion robot
 */
int robot_x=2; 
int robot_y=3; 

/*
 * ubicacion objetivo
 */
int objetivo_x=6;
int objetivo_y=6;

//size de la matriz
int size_m = 7;
int size_n = 7;

//ubicaciones mapa
int x=0;
int y=0;

int explore=0;
int counter=0;
int steps=0; //pasos para llegar al resultado

//cuando se busca por un nodo con valor minimo
int minimum_node=250;
int min_node_location=250;

int reset_min=250; //todo arriba de este valor es un item especial, pared o el robot

//X is vertical, Y is horizontal
int mapa[7][7] = {{0,  0,  0,  0,  0,  0,  0},
		  {0,  0,  0,255,255,255,  0},
		  {0,  0,255,  0,255,255,  0},
		  {0,255,255,  0,  0,255,  0},
		  {0,  0,255,  0,  0,  0,  0},
		  {0,  0,  0,255,255,255,255},
		  {0,  0,  0,  0,  0,  0,  0}};

/*
 * Se declaran las funciones para compilar
 */
void inicializar_mapa(void);
int propagate_wavefront(int robot_x, int robot_y);
int min_surrounding_node_value(int x, int y);
void mostrar_mapa(void);

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
void inicializar_mapa(void){

  system("clear"); //limpiar pantalla
  printf(RED "Se comienza Wavefront\n\n" RESET); //imprimir texto
  sleep(1);  // dormir para visualizar el texto antes de limpiar
  
  //Agregar el robor en el mapa
  mapa[robot_x][robot_y]=robot;
  
  //Agregar el objetivo en el mapa
  mapa[objetivo_x][objetivo_y]=objetivo; //poner el objetivo en el mapa
  
  //Pintar el mapa
  mostrar_mapa();
  
  printf(RED "Se incluyo el mapa y el robot\n\n" RESET);
  sleep(1);

}

int propagate_wavefront(int robot_x, int robot_y){
  
  counter=0;

  inicializar_mapa(); // Cargar las cosas al mapa
  
  //ciclos para pasadas de exploracion
  while(counter<20){
    
    x=0;
    y=0;

    // hacer hasta explorar todo el mapa
    while(x<size_m && y<size_n){
      // si la ubicacion actual es diferente a pared y el objetivo
      if (mapa[x][y] != pared && mapa[x][y] != objetivo){	

	mostrar_mapa(); //mostrar mapa
	
	// se ha encontrado el objetivo
	if (min_surrounding_node_value(x, y) < reset_min && mapa[x][y]==robot){
	  
	  mostrar_mapa();
	  
	  printf(GRNB "-termine-\n\n" RESET);
	  //exit(0);
	  //termine!
	  //hacer ciclo para mover al robot
	  return min_node_location;
	}
	
	//record a value in to this node
	//if this isnt here, 'nothing' will go in the location
	else if (minimum_node!=reset_min)
	  mapa[x][y]=minimum_node+1;
      }

      //hacer exploracion una a una
      //go to next node and/or row
      y++;
      if (y==size_m && x!=size_n){
	x++;
	y=0;
      }	
    }

    printf("#:%d\n",counter+1);
    mostrar_mapa();
    counter++;

  }

  return counter;

}

//de un nodo, explorar y regresar el valor mas bajo
int min_surrounding_node_value(int x, int y){
  minimum_node=reset_min;//reset minimum
  
  //down
  if(x < 6) //not out of boundary
    //find the lowest number node, and exclude empty nodes (0's)
    if  (mapa[x+1][y] < minimum_node && mapa[x+1][y] != vacio){
      minimum_node = mapa[x+1][y];
      min_node_location=3;
    }
  
  //up
  if(x > 0)
    if  (mapa[x-1][y] < minimum_node && mapa[x-1][y] != vacio){
      minimum_node = mapa[x-1][y];
      min_node_location=1;
    }
  
  //right
  if(y < 6)
    if  (mapa[x][y+1] < minimum_node && mapa[x][y+1] != vacio){
      minimum_node = mapa[x][y+1];
      min_node_location=2;
    }
  
  //left
  if(y > 0)
    if  (mapa[x][y-1] < minimum_node && mapa[x][y-1] != vacio){
      minimum_node = mapa[x][y-1];
      min_node_location=4;
    }

  return minimum_node;

}

/*
 * Imprimir mapa
 */
void mostrar_mapa(void){

  usleep(2000);
  //sleep(1);
  system("clear");

  //recorrer el arreglo
  for (int i=0;i<size_n;i++){
    for (int j=0;j<size_n;j++){
      if (mapa[i][j]==pared)
	printf(WHTHB "  # " RESET);
      else if (mapa[i][j]==robot)
	printf(BYEL "  R " RESET);
      else if (mapa[i][j]==objetivo)
	printf(BBLU "  X " RESET);
      else
	if(mapa[i][j]>9){
	  printf(" %d ",mapa[i][j]);
	}else{
	  printf("  %d ",mapa[i][j]);
	}
	
    }
    printf("\n"); //salto de linea para el recorrido Y
  }
  
  printf("\n"); //salto de linea para mostrar texto abajo
  
  steps++;
}
