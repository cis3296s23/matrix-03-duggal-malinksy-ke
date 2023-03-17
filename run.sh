#!/bin/sh

rm -f graphSIMD.txt graphUnoptimizedMmult.txt


make main_out
make main_out_vector

echo "Non Vectorized times\n" > graphSIMD.txt
echo "Non Vectorized times\n" > graphUnoptimizedMmult.txt
./main_out 3
./main_out 4
./main_out 5

echo "Vectorized times\n" >> graphSIMD.txt
echo "Vectorized times\n" >> graphUnoptimizedMmult.txt
./main_out_vector 3
./main_out_vector 4
./main_out_vector 5
