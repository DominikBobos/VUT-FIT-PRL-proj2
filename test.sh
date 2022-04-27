#!/bin/bash
#/**
# * Subject: PRL
# * Algorithm: Pre-order tree tour
# * Author: Dominik Boboš <xbobos00@stud.fit.vutbr.cz>
# * Ac.year: 2021/2022
# */

if [ $# -ne 1 ];then
    echo 'One argument with the alphabet sequence is expected.'
    exit 1
fi;

number_of_processors=$(expr ${#1} \* 2 - 2)

# Build
mpic++ --prefix /usr/local/share/OpenMPI -o preorder preorder.cpp

# MPI run
mpirun --oversubscribe --prefix /usr/local/share/OpenMPI \
       -np $number_of_processors preorder $1

# Clean
rm -f preorder