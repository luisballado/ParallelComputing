//Compilar
mpicc montecarlo_pi.c -o montecarlo_pi

//Correr
mpirun -n 4 ./montecarlo_pi 1000000 
