#!/bin/bash
#PBS -V
#PBS -M email@cinvestav.mx
#PBS -m a
#PBS -N SOLVE
#PBS -q cpar
#PBS -l nodes=2:ppn=8
#PBS -o /home/cpar/TEST/
#PBS -e /home/cpar/TEST/
#PBS -t 1-50
cd $PBS_O_WORKDIR

PROBLEMS=( "P1" "P2" "P3" "P4" "P5" )
RUNS=10

INDEX=$((PBS_ARRAYID-1))
prob=${PROBLEMS[INDEX/RUNS]}
run=$((INDEX%RUNS+1))

mpirun -n 16 ./solve_mpi ${prob} ${run} > Results/mpi_${prob}_${run}.txt

