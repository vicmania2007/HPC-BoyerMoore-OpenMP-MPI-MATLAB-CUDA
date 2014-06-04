#!/bin/sh	
#BSUB -J Vignesh-Horsepool_mp
#BSUB -o output_mp
#BSUB -e error_mp
#BSUB -n 1
#BSUB -q ht-10g

#BSUB cwd /home/kumarsubramanian.v/Project
work=/home/kumarsubramanian.v/Project

cd $work
rm output_mp error_mp
./a.out gene.txt "gttggta"
