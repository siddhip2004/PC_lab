#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

using namespace std;

int main() {
    int n;

    cout << "Enter number of elements in the array: ";
    cin >> n;

    vector<int> arr(n);
    srand(time(0));

    // Initialize array with random values
    cout << "Array elements: ";
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;  // Random values between 0-999
        cout << arr[i] << " ";
    }
    cout << endl;

    int max_value = arr[0];

    // Parallel block
    #pragma omp parallel
    {
        int local_max = max_value;  // Thread-local max

        #pragma omp for
        for (int i = 0; i < n; i++) {
            if (arr[i] > local_max) {
                local_max = arr[i];
            }
        }

        // Critical section to update global max
        #pragma omp critical
        {
            if (local_max > max_value) {
                max_value = local_max;
            }
        }
    }

    cout << "Largest element in the array: " << max_value << endl;

    return 0;
}
