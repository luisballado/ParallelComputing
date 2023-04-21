#!/bin/bash
#PBS -V
#PBS -M email@cinvestav.mx
#PBS -m a
#PBS -N SOLVE
#PBS -q cpar
#PBS -l nodes=1:ppn=1
#PBS -o /home/cpar/TEST/
#PBS -e /home/cpar/TEST/
cd $PBS_O_WORKDIR

./solve_serial P1 1 > Results/serial_P1_1.txt

