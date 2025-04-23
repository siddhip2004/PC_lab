#include <iostream>
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <limits> // For std::numeric_limits

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> data;
    int n = 0; // Number of elements

    if (rank == 0) {
        // Root process asks the user for input
        cout << "Enter the number of elements: ";
        cin >> n;

        data.resize(n);
        cout << "Enter " << n << " elements: ";
        for (int i = 0; i < n; ++i) {
            cin >> data[i];
        }

        cout << "Original Array: ";
        for (int i = 0; i < n; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    // Broadcast the size of the array to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate how many elements each process will receive
    int local_data_size = n / size;
    int remainder = n % size;

    // Create arrays for sending and receiving
    vector<int> sendcounts(size);
    vector<int> displacements(size);

    for (int i = 0; i < size; ++i) {
        sendcounts[i] = local_data_size + (i < remainder ? 1 : 0); // Distribute remainder
        displacements[i] = i * local_data_size + (i < remainder ? i : remainder); // Calculate displacements
    }

    // Resize local_data based on how many elements this process will receive
    vector<int> local_data(sendcounts[rank]);

    // Scatter data to all processes
    MPI_Scatterv(data.data(), sendcounts.data(), displacements.data(), MPI_INT,
                  local_data.data(), sendcounts[rank], MPI_INT,
                  0, MPI_COMM_WORLD);

    // Each process finds the local maximum only if it has received data
    int local_max;
    
    if (!local_data.empty()) {
        local_max = *max_element(local_data.begin(), local_data.end());
    } else {
        // If no data was received, set local_max to a very small number
        local_max = numeric_limits<int>::min();
    }

    // Reduce to find the global maximum at root process
    int global_max;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Root process prints the maximum value
    if (rank == 0) {
        cout << "Maximum value in the array: " << global_max << endl;
    }

    MPI_Finalize();
    return 0;
}