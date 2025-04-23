#include <mpi.h>
#include <iostream>
#include <cstring> // For memcpy

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Ensure we have exactly 4 processes
    // if (world_size != 4) {
    //     if (world_rank == 0) {
    //         std::cerr << "This program requires exactly 4 MPI processes.\n";
    //     }
    //     MPI_Finalize();
    //     return 1;
    // }

    // Create a 4x4 character matrix (each string is up to 10 characters)
    const int rows = 4;
    const int cols = 5; // Maximum string length + 1 (for null terminator)
    char matrix[rows][cols] = {"VJTI", "GOOD", "BEST", "DAYS"};

    // Buffer to store the string received by each process
    char recv_buffer[cols];

    // Scatter the strings from process 0 to all processes
    MPI_Scatter(matrix, cols, MPI_CHAR, recv_buffer, cols, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Each process prints its rank and the string it received
    std::cout << "Rank " << world_rank << " received string: " << recv_buffer << std::endl;

    MPI_Finalize();
    return 0;
}