#include <iostream>
#include <chrono>
#include <riscv_vector.h>
#include "helper.h"
using namespace std;

void naive_matrix_add(float *a, float *b, float *c, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            c[i * N + j] = a[i * N + j] + b[i * N + j];
        }
    }
}

void vectorized_add(float *a, float *b, float *c, int N)
{
    size_t vlmax = __riscv_vsetvlmax_e32m1();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j += vlmax)
        {
            size_t vl = __riscv_vsetvl_e32m1(N - j);
            vfloat32m1_t v1 = __riscv_vle32_v_f32m1(&a[i * N + j], vl);
            vfloat32m1_t v2 = __riscv_vle32_v_f32m1(&b[i * N + j], vl);
            vfloat32m1_t v3 = __riscv_vfadd_vv_f32m1(v1, v2, vl);
            __riscv_vse32_v_f32m1(&c[i * N + j], v3, vl);
        }
    }
}

int main()
{
    int N = 200;
    float *arr1 = new float[N * N];
    float *arr2 = new float[N * N];
    float *result_naive = new float[N * N];
    float *result_vector = new float[N * N];

    fillMatrix(arr1, N);
    fillMatrix(arr2, N);

    for (int i = 0; i < 15; i++)
    {
        naive_matrix_add(arr1, arr2, result_naive, N);
        vectorized_add(arr1, arr2, result_vector, N);
    }

    auto start_naive = chrono::high_resolution_clock::now();
    naive_matrix_add(arr1, arr2, result_naive, N);
    auto end_naive = chrono::high_resolution_clock::now();
    auto naive_duration = chrono::duration_cast<chrono::microseconds>(end_naive - start_naive).count();
    cout << "Naive: " << naive_duration << " ms" << endl;

    auto start_vec = chrono::high_resolution_clock::now();
    vectorized_add(arr1, arr2, result_vector, N);
    auto end_vec = chrono::high_resolution_clock::now();
    auto vec_duration = chrono::duration_cast<chrono::microseconds>(end_vec - start_vec).count();
    cout << "Vector: " << vec_duration << " ms" << endl;

    delete[] arr1;
    delete[] arr2;
    delete[] result_naive;
    delete[] result_vector;

    return 0;
}