#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

#define SEMILLA 234354

int main(int argc, char* argv[]){

  if(argc <= 1){
    fprintf(stderr,"Hay que pasar argumentos");
    MPI_Finalize();
    exit(-1);
  }
  
  int num_proc, my_id;
  
  double npoints = atof(argv[1]); //1000000;
  double resultado_pi;
  double random_x,random_y;
  int circle_count=0;
  double en_circulo;

  // Start MPI
  MPI_Init(&argc,&argv);

  // Get number of processes and ID
  MPI_Comm_size(MPI_COMM_WORLD,&num_proc);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_id);

  int recieved[num_proc];
  long recvniter[num_proc];
  
  srand(SEMILLA+my_id); //crear semilla para los random
  
  if(my_id==0){
    //printf("Hello from process %d!\n",my_id);
    for(int i=0;i<num_proc;i++){
      MPI_Recv(&recieved[i], num_proc, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      MPI_Recv(&recvniter[i], num_proc, MPI_LONG, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    int cont_final = 0;
    long finalniter = 0;

    for (int i=0; i<num_proc;i++){
      cont_final = cont_final + recieved[i];
      finalniter = finalniter + recvniter[i];
    }
    
    resultado_pi = 4.0*((double)circle_count/(double)npoints);
    printf("Pi: %f\n", resultado_pi);
    
  }else{

    //sprintf(message,"Hello from process %d!", my_id);
    //MPI_Send(message, strlen(message)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
    for (int i=0; i<npoints; i++){
      
      //crear puntos random
      //https://stackoverflow.com/questions/6218399/how-to-generate-a-random-number-between-0-and-1
      random_x = (double)random()/(double)RAND_MAX;
      random_y = (double)random()/(double)RAND_MAX;
      en_circulo = sqrt((random_x*random_x)+(random_y*random_y));
      
      //revisar si el punto esta en el circulo unitario
      //https://gigi.nullneuron.net/gigilabs/calculating-circle-area-the-monte-carlo-way/
      //If the point is represented by the coordinates (x, y), then the point lies inside the circle of radius r if (x2 + y2) <= r2
      if (en_circulo<=1){
	++circle_count;
      }
    }
    
    for(int i=0; i<num_proc; ++i){
      MPI_Send(&circle_count,1,MPI_INT,0,1,MPI_COMM_WORLD);
      MPI_Send(&npoints,1,MPI_LONG,0,2,MPI_COMM_WORLD);
    }
  }
    
  MPI_Finalize();
    
  return 0;

}
