# Commands to run codes:

## 1. MPI: 
mpic++ -o pi_mpi pi_mpi.cpp
<br>
mpirun -np 4 ./pi_mpi

## 2. Open MP:
Ctrl + Alt + N

## 3. Hybrid:

mpic++ -fopenmp pi_hybrid.cpp -o pi_hybrid
<br>
mpirun -np 4 ./pi_hybrid

