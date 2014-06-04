#!/bin/sh
#BSUB -J Vignesh-Horsepool_bible
#BSUB -o Horsepool_output_world
#BSUB -e Horsepool_error
#BSUB -n 1
#BSUB -q ht-10g
#BSUB cwd /home/kumarsubramanian.v/
work=/home/kumarsubramanian.v/
cd $work
./horse world192.txt "and this string is added to help in better analysis"
