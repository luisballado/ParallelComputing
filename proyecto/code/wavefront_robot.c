//WAVEFRONT

#include "ANSI-color-codes.h" //Colores fancy

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//cosas del mapa
int vacio=0;
int pared=255;
int objetivo=1;
int robot=254;

//ubicacion robot
int robot_x=2; 
int robot_y=3; 

//ubicacion objetivo
int objetivo_x=6;
int objetivo_y=6;

//size de la matriz
int size_m = 7;
int size_n = 7;

//ubicaciones mapa
int x=0;
int y=0;

int counter=0;
int steps=0; //pasos para llegar al resultado

//cuando se busca por un nodo con valor minimo
int minimum_node=250;
int min_node_location=250;
int new_state=1;
int old_state=1;
int reset_min=250; //todo arriba de este valor es un item especial, pared o el robot

//X is vertical, Y is horizontal
int mapa[7][7] = {{0,0,0,0,255,255,0},
		  {0,0,0,0,255,255,0},
		  {0,0,0,0,255,255,0},
		  {0,255,255,255,255,255,255},
		  {0,0,0,0,0,0,0},
		  {0,0,0,0,0,0,0},
		  {0,0,0,0,0,0,0}};

//se declaran las funciones
int propagate_wavefront(int robot_x, int robot_y);
int min_surrounding_node_value(int x, int y);
void mostrar_mapa(void);

int main(void){

  system("clear"); //limpiar pantalla
  printf(RED "Se comienza Wavefront\n\n" RESET); //imprimir texto
  sleep(1);  // dormir para visualizar el texto antes de limpiar
  
  //////////////wavefront code//////////////
  // TODO: PONER LA RUTA ENCONTRADA
  
  new_state=propagate_wavefront(robot_x,robot_y);
  
  system("PAUSE");
  return 0;
}

int propagate_wavefront(int robot_x, int robot_y){
  
  mapa[robot_x][robot_y]=robot;
  
  //start location to begin scan at goal location
  mapa[objetivo_x][objetivo_y]=objetivo; //poner el objetivo en el mapa
  
  mostrar_mapa();

  printf("Se incluyo el objeto\n\n");
  
  counter=0;

  //allows for recycling until robot is found
  while(counter<50){
    x=0;
    y=0;
    //while the map hasnt been fully scanned
    while(x<size_m && y<size_n){
      //if this location is a wall or the goal, just ignore it
      if (mapa[x][y] != pared && mapa[x][y] != objetivo){	
	mostrar_mapa();
	
	//a full trail to the robot has been located, finished!
	if (min_surrounding_node_value(x, y) < reset_min && mapa[x][y]==robot){
	  
	  mostrar_mapa();
	  printf("-termine-\n\n");
	  exit(0);
	  //termine!
	  //hacer ciclo para mover al robot
	  //return min_node_location;
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
  return 0;
}

//this function looks at a node and returns the lowest value around that node
int min_surrounding_node_value(int x, int y){
  minimum_node=reset_min;//reset minimum
  
  //down
  if(x < 6)//not out of boundary
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

/***************************
 * Imprimir mapa
 ***************************/
void mostrar_mapa(void){

  usleep(2000);
  //sleep(1);
  system("clear");
  
  for (int i=0;i<size_n;i++){
    for (int j=0;j<size_n;j++){
      if (mapa[i][j]==pared)
	//printf("# ");
	printf(BCYN " # " RESET);
      else if (mapa[i][j]==robot)
	printf(BYEL " R " RESET);
      else if (mapa[i][j]==objetivo)
	printf(BBLU " X " RESET);
      else
	printf(" %d ",mapa[i][j]);
    }
    printf("\n");//then go to next line Y
  }
  printf("\n");
  steps++;
}
