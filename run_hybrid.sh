#!/bin/bash
#PBS -V
#PBS -M email@cinvestav.mx
#PBS -m a
#PBS -N HELLO
#PBS -q cpar
#PBS -l nodes=2:ppn=8
#PBS -o /home/cpar/TEST/
#PBS -e /home/cpar/TEST/
cd $PBS_O_WORKDIR

mpirun -n 2 --map-by node ./hello_hybrid 8 > output.txt
