#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Function to check if a number is prime
bool isPrime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

int main() {
    int n;
    cout << "Enter the value of n: ";
    cin >> n;

    vector<int> primes; // To store prime numbers

    #pragma omp parallel for
    for (int i = 2; i <= n; i++) {
        if (isPrime(i)) {
            #pragma omp critical
            primes.push_back(i); // Only one thread can modify the array at a time
        }
    }

    // Printing the primes
    cout << "Prime numbers up to " << n << " are: ";
    for (int prime : primes) {
        cout << prime << " ";
    }
    cout << endl;

    return 0;
}

