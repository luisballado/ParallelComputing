//WAVEFRONT ALGORITHM

#include "ANSI-color-codes.h" //Colores fancy
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//en main, declarar la matrix nxn
int nothing=0;
int wall=255;
int goal=1;
int robot=254;

// se declara las ubicaciones del robot y objetivo
int robot_x=5;
int robot_y=5;
int goal_x=0;
int goal_y=0;

int size_m = 6;
int size_n = 6;

//ubicaciones mapa
int x=0;
int y=0;
bool uno_a_uno = false;

int counter=0;
int steps=0; //pasos para llegar al resultado

//cuando se busca por un nodo con valor minimo
int minimum_node=250;
int min_node_location=250;
int new_state=1;
int old_state=1;
int trans=50;
int reset_min=250; //todo arriba de este valor es un item especial, pared o el robot

//X is vertical, Y is horizontal
int mapa[6][6] = {{0,0,0,0,255,255},
		  {0,0,0,0,255,255},
		  {0,255,255,255,255,255},
		  {0,0,0,0,0,0},
		  {0,0,0,0,0,0},
		  {0,0,0,0,0,0}};

//se declaran las funciones
int propagate_wavefront(int robot_x, int robot_y);
void unpropagate(int robot_x, int robot_y);
int min_surrounding_node_value(int x, int y);
void mostrar_mapa(void);

int main(void){
  system("clear");
  printf(RED "Se comienza Wavefront\n\n" RESET);
  sleep(1);
  
  //////////////wavefront code//////////////
  //hacer hasta encontrar el objetivo
  while(mapa[robot_x][robot_y]!=goal){
    //encontrar una nueva ubicacion para explorar
    new_state=propagate_wavefront(robot_x,robot_y);
    
    //actualizar la nueva ubicacion del robot
    if (new_state==1){
      robot_x--;
      //printf("x=%d y=%d\n\n",robot_x,robot_y);
    }
    if (new_state==2){
      robot_y++;
      //printf("x=%d y=%d\n\n",robot_x,robot_y);
    }
    if (new_state==3){
      robot_x++;
      //printf("x=%d y=%d\n\n",robot_x,robot_y);
    }
    if (new_state==4){
      robot_y--;
      //printf("x=%d y=%d\n\n",robot_x,robot_y);
    }
        
    //make new state the old state
    old_state=new_state;
    trans--;
  }
  //////////////////////////////////////////
  
  printf("steps: %d\n", steps);
  system("PAUSE");
  return 0;
}


int propagate_wavefront(int robot_x, int robot_y){
  //clear old wavefront
  unpropagate(robot_x, robot_y);
  
  //start location to begin scan at goal location
  mapa[goal_x][goal_y]=goal; //poner el objetivo en el mapa

  
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
      if (mapa[x][y] != wall && mapa[x][y] != goal){	
	mostrar_mapa();
	//a full trail to the robot has been located, finished!
	if (min_surrounding_node_value(x, y) < reset_min && mapa[x][y]==robot){
	  
	  mostrar_mapa();
	  printf("-termine-\n\n");
	  exit(0);
	  //finshed! tell robot to start moving down path
	  return min_node_location;
	}
	//record a value in to this node
	//if this isnt here, 'nothing' will go in the location
	else if (minimum_node!=reset_min)
	  mapa[x][y]=minimum_node+1;
      }

      //hacer exploracion una a una
      //go to next node and/or row
      if(uno_a_uno == false){
	x++;
	if (x==size_m && y!=size_n){
	  y++;
	  x=0;
	}	
      }
    }
    printf("Sweep #: %d\n",counter+1);
    mostrar_mapa();
    counter++;
  }
  return 0;
}

//clears old path to determine new path
void unpropagate(int robot_x, int robot_y){	
  //printf("Old Map:\n");
  mostrar_mapa();
  
  //printf("Starting Unpropagate\n");
  
  //stay within boundary
  for (x=size_m; x<size_n; x++)
    for (y=0; y<size_n; y++)
      if (mapa[x][y] != wall && mapa[x][y] != goal) //if this location is a wall or goal, just ignore it
	mapa[x][y] = nothing;//clear that space
  
  //old robot location was deleted, store new robot location in map
  mapa[robot_x][robot_y]=robot;
  
  //printf("Unpropagation Complete:\n");
  mostrar_mapa();
}

//this function looks at a node and returns the lowest value around that node
int min_surrounding_node_value(int x, int y){
  minimum_node=reset_min;//reset minimum
  
  //down
  if(x < 5)//not out of boundary
    //find the lowest number node, and exclude empty nodes (0's)
    if  (mapa[x+1][y] < minimum_node && mapa[x+1][y] != nothing){
      minimum_node = mapa[x+1][y];
      min_node_location=3;
    }
  
  //up
  if(x > 0)
    if  (mapa[x-1][y] < minimum_node && mapa[x-1][y] != nothing){
      minimum_node = mapa[x-1][y];
      min_node_location=1;
    }
  
  //right
  if(y < 5)
    if  (mapa[x][y+1] < minimum_node && mapa[x][y+1] != nothing){
      minimum_node = mapa[x][y+1];
      min_node_location=2;
    }
  
  //left
  if(y > 0)
    if  (mapa[x][y-1] < minimum_node && mapa[x][y-1] != nothing){
      minimum_node = mapa[x][y-1];
      min_node_location=4;
    }
  
  return minimum_node;
}

/***************************
 * Imprimir mapa
 ***************************/
void mostrar_mapa(void){

  //sleep(1);
  system("clear");
  
  for (int i=0;i<size_m;i++){
    for (int j=0;j<size_n;j++){
      if (mapa[i][j]==wall)
	//printf("# ");
	printf(BCYN " # " RESET);
      else if (mapa[i][j]==robot)
	printf(BYEL " R " RESET);
      else if (mapa[i][j]==goal)
	printf(BBLU " G " RESET);
      else
	printf(" %d ",mapa[i][j]);
    }
    printf("\n");//then go to next line Y
  }
  printf("\n");
  steps++;
}
