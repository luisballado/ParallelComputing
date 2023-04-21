#!/bin/bash
#PBS -V
#PBS -M email@cinvestav.mx
#PBS -m a
#PBS -N SOLVE
#PBS -q cpar
#PBS -l nodes=2:ppn=8
#PBS -o /home/cpar/TEST/
#PBS -e /home/cpar/TEST/
cd $PBS_O_WORKDIR

mpirun -n 16 ./solve_mpi P1 1 > Results/mpi_P1_1.txt

