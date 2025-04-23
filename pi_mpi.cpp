#include <mpi.h>
#include <iostream>
#include <cmath>

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    long long n_intervals = 0; // Number of intervals for the integration

    if (world_rank == 0) {
        // Process 0 prompts the user for the number of intervals
        std::cout << "Enter the number of intervals (higher = more accurate): ";
        std::cin >> n_intervals;
    }

    // Broadcast the number of intervals to all processes
    MPI_Bcast(&n_intervals, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Calculate the width of each interval
    double width = 1.0 / n_intervals;

    // Determine the sub-interval for this process
    long long local_start = world_rank * (n_intervals / world_size);
    long long local_end = (world_rank + 1) * (n_intervals / world_size);
    if (world_rank == world_size - 1) {
        // Last process handles the remaining intervals (if any)
        local_end = n_intervals;
    }

    // Compute the integral over the local sub-interval using the midpoint rule
    double local_sum = 0.0;
    for (long long i = local_start; i < local_end; ++i) {
        double x = (i + 0.5) * width; // Midpoint of the current interval
        local_sum += f(x);
    }
    local_sum *= width; // Multiply by the width to get the area

    // Use MPI_Reduce to sum up the results from all processes
    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Process 0 outputs the result
    if (world_rank == 0) {
        std::cout << "Approximation of Pi: " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
