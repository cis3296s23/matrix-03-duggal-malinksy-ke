#!/bin/sh

rm -f graphSIMD.txt graphUnoptimizedMmult.txt graphOMP.txt graphMPIOMP.txt


make

echo "Non Vectorized times\n" > graphSIMD.txt
echo "Non Vectorized times\n" > graphUnoptimizedMmult.txt
echo "Non Vectorized times\n" > graphOMP.txt
for i in {1..500}
do
  ./main_out $i
done

echo "Vectorized times\n" >> graphSIMD.txt
echo "Vectorized times\n" >> graphUnoptimizedMmult.txt
echo "Vectorized times\n" >> graphOMP.txt
for i in {1..500}
do
  ./main_out_vector $i
done

echo "MP times" > graphMPIOMP.txt

for i in {1..500}
do
  mpiexec -f ~/hosts -n 4 ./mmult_mpi_omp_variable $i
done

pip install matplotlib

python graphGenerate.py
