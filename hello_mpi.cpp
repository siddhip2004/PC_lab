#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Initialize MPI environment

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Number of processes

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // Rank of the process

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    std::cout << "Hello from processor " << processor_name
              << ", rank " << world_rank
              << " out of " << world_size << " processors" << std::endl;

    MPI_Finalize(); // Finalize MPI environment
    return 0;
}
