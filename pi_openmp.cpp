#include <iostream>
#include <omp.h>

using namespace std;

double f(double x) {
    return 4.0 / (1 + x * x);  // Function for π integration
}

int main() {
    int num_intervals = 1000000;  // Number of intervals
    double step_size = 1.0 / num_intervals;  // Width of each interval
    double pi_estimate = 0.0;  // Result accumulator

    // Parallel computation
    #pragma omp parallel
    {
        double local_sum = 0.0;

        #pragma omp for
        for (int i = 0; i < num_intervals; i++) {
            double mid = (i + 0.5) * step_size;  // Midpoint of the subinterval
            local_sum += f(mid);  // Compute function value at midpoint
        }

        // Critical section to safely update shared result
        #pragma omp critical
        pi_estimate += local_sum;
    }

    // Multiply by step size to get the final integral value
    pi_estimate *= step_size;

    // Output result
    cout << "Estimated value of Pi: " << pi_estimate << endl;

    return 0;
}
