#include "ANSI-color-codes.h" //Colores fancy
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

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
