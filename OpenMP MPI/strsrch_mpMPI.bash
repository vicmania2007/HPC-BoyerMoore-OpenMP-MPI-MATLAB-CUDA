#!/bin/sh

#BSUB -J JOB.125
#BSUB -o output_file
#BSUB -e error_file
#BSUB -n 4
#BSUB -q ht-10g
#BSUB -cwd /home/kumarsubramanian.v/Project/MPMPI/

work=/home/kumarsubramanian.v/Project/MPMPI
cd $work
rm output_file error_file

tempfile1=hostlistrun
tempfile2=hostlist-tcp
echo $LSB_MCPU_HOSTS > $tempfile1
declare -a hosts
read -a hosts < ${tempfile1}
for ((i=0; i<${#hosts[@]}; i += 2)) ;
do
    HOST=${hosts[$i]}
    CORE=${hosts[(($i+1))]}
    echo $HOST:$CORE >> $tempfile2
done


mpirun -np 4 -prot -TCP -lsf /home/kumarsubramanian.v/Project/MPMPI/str /home/kumarsubramanian.v/Project/world192.txt "and this string is added to help in better analysis" 100

rm $work/$tempfile1
rm $work/$tempfile2
