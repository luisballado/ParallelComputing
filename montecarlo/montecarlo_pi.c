#include <stdio.h>
#include <stdlib.h> //atof, rand, RANDMAX
#include <mpi.h>
#include <math.h> //sqrt

//Referencias
//https://gigi.nullneuron.net/gigilabs/calculating-circle-area-the-monte-carlo-way/
//https://www.olcf.ornl.gov/tutorials/monte-carlo-pi/

#define SEMILLA 12344352
 
int main(int argc, char* argv[]){
  
  long npoints = atof(argv[1]);
  int my_id;           //ids de procesos
  int num_proc;        //num de procesos
  double x,y;          //x,y random
  int count=0;         //Contador del numero de buenos puntos
  double en_circulo;   //revisar si esta dentro del circulo unitario x^2+y^2<=1
  double pi;           //resultado de pi
  
  //Inicializar MPI
  MPI_Init(&argc, &argv);                      

  // Get number of processes and ID
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);  
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  
  int recieved[num_proc];
  long recvniter[num_proc];

  srand(SEMILLA+my_id);  //proporcionar semillas para variar el random
  
  if(my_id == 0){
    
    for(int i=0; i<num_proc; ++i){
      MPI_Recv(&recieved[i],num_proc,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      MPI_Recv(&recvniter[i],num_proc,MPI_LONG,MPI_ANY_SOURCE,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    int finalcount = 0;
    long finalniter = 0;

    for(int i = 0; i<num_proc; ++i){
      finalcount += recieved[i];
      finalniter += recvniter[i];
    }
    
    pi = ((double)finalcount/(double)finalniter)*4.0;
    printf("Pi: %f\n", pi);    //mostrar resultado
    
  }else{

    for (int i=0; i<npoints; ++i){        
      x= ((double)rand())/RAND_MAX; 
      y =((double)rand())/RAND_MAX; 
      en_circulo = sqrt(x*x+y*y);

      if (en_circulo<=1){
	count++;
      }
      
    }
    
    for(int i=0; i<num_proc; ++i){
      MPI_Send(&count,1,MPI_INT,0,1,MPI_COMM_WORLD);
      MPI_Send(&npoints,1,MPI_LONG,0,2,MPI_COMM_WORLD);
    }
    
    
  }

  MPI_Finalize(); //Cerrar instancia
  
  return 0;

}
