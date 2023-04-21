#!/bin/bash
#PBS -V
#PBS -M email@cinvestav.mx
#PBS -m a
#PBS -N SOLVE
#PBS -q cpar
#PBS -l nodes=2:ppn=8
#PBS -o /home/cpar/TEST/
#PBS -e /home/cpar/TEST/
#PBS -t 1-10
cd $PBS_O_WORKDIR

mpirun -n 16 ./solve_mpi P1 ${PBS_ARRAYID} > Results/mpi_P1_${PBS_ARRAYID}.txt

