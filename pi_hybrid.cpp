#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

long long monteCarloPi(long long num_samples) {
    long long count = 0;
    #pragma omp parallel for reduction(+:count)
    for (long long i = 0; i < num_samples; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x*x + y*y <= 1.0) {
            count++;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    long long num_samples = 1000000; // Total samples to estimate PI
    long long local_samples = num_samples / size;
    
    srand(time(NULL) + rank); // Seed random generator differently for each process
    
    // Each MPI process computes part of the Monte Carlo estimation using OpenMP
    long long local_count = monteCarloPi(local_samples);
    
    long long global_count;
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        double pi_estimate = (4.0 * global_count) / num_samples;
        cout << "Estimated value of PI: " << pi_estimate << endl;
    }
    
    MPI_Finalize();
    return 0;
}
