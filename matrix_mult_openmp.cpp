#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

#define N 3 

void multiply_matrices_parallel(int A[N][N], int B[N][N], int C[N][N]) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_matrices_sequential(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void print_matrix(int M[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int A[N][N] = {{100000, 200000, 300000}, {400000, 500000, 600000}, {700000, 800000, 900000}};
    int B[N][N] = {{900000, 80000, 70000}, {60000, 50000, 40000}, {30000, 20000, 10000}};
    int C_parallel[N][N] = {0};
    int C_sequential[N][N] = {0};
    
    cout << "Matrix A:" << endl;
    print_matrix(A);
    
    cout << "Matrix B:" << endl;
    print_matrix(B);
    
    auto start = chrono::high_resolution_clock::now();
    multiply_matrices_parallel(A, B, C_parallel);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> parallel_time = end - start;
    
    cout << "Resultant Matrix C (Parallel):" << endl;
    print_matrix(C_parallel);
    cout << "Time taken using OpenMP: " << parallel_time.count() << " seconds" << endl;
    
    start = chrono::high_resolution_clock::now();
    multiply_matrices_sequential(A, B, C_sequential);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> sequential_time = end - start;
    
    cout << "Resultant Matrix C (Sequential):" << endl;
    print_matrix(C_sequential);
    cout << "Time taken without using OpenMP: " << sequential_time.count() << " seconds" << endl;
    
    return 0;
}