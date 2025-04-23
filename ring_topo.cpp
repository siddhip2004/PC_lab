#include <iostream>
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rank, size, sum = 0, recv_sum = 0, row = 0, num_processes;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Rank 0 takes user input for number of processes
    if (rank == 0)
    {
        std::cout << "Enter the number of processes: ";
        std::cin >> num_processes;

        // Ensure that the number of processes entered matches the size of the communicator
        if (num_processes != size)
        {
            std::cerr << "Error: The number of processes specified does not match the number of processes being run in this MPI communicator.\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Broadcast the number of processes to all other processes
    MPI_Bcast(&num_processes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Rank 0 starts with sum = 0 and prints its row
    if (rank == 0)
    {
        sum = 0;
        std::cout << "\n----------------------------\n";
        std::cout << "| Rank | Row | Sum |\n";
        std::cout << "----------------------------\n";
        printf("| %-6d | %-6d | %-6d |\n", rank, row, sum);
    }

    // Use MPI_Send and MPI_Recv to send the sum in a ring topology
    for (row = 1; row < size; row++)
    {
        if (rank == row)
        {
            // Receive sum from the previous rank
            MPI_Recv(&recv_sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum = recv_sum + rank;
            printf("| %-6d | %-6d | %-6d |\n", rank, row, sum);
        }

        // Send the updated sum to the next process in the ring
        if (rank == row - 1)
        {
            MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == size - 1)
    {
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        MPI_Recv(&sum, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}