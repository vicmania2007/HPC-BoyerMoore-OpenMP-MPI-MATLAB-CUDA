#!/bin/sh

#BSUB -J Hari-VectorSum
#BSUB -o output_file
#BSUB -e error_file
#BSUB -n 10
#BSUB -q ht-10g
#BSUB cwd /home/patteri.h/hw4/
work=/home/patteri.h/hw4/

cd $work
rm output_file error_file
./str_search_seq alice.txt "alice"
