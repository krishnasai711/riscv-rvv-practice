#include <iostream>
#include <chrono>
#include <riscv_vector.h>
#include "helper.h"
using namespace std;

void naive_matmul(float *a, float *b, float *c, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                c[i * N + j] += a[i * N + k] * b[k * N + j];
            }
        }
    }
}

void vectorized_matmul(float *a, float *b, float *c, int N)
{
    size_t vl = __riscv_vsetvl_e32m1(8);
    int vlen = (N / 8) * 8;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            float scalar = a[i * N + j];
            vfloat32m1_t vec_a = __riscv_vfmv_v_f_f32m1(scalar, vl);

            for (int k = 0; k < vlen; k += 8)
            {
                vfloat32m1_t tmp = __riscv_vle32_v_f32m1(&c[i * N + k], vl);
                vfloat32m1_t vec_b = __riscv_vle32_v_f32m1(&b[j * N + k], vl);
                tmp = __riscv_vfmacc_vv_f32m1(tmp, vec_a, vec_b, vl);
                __riscv_vse32_v_f32m1(&c[i * N + k], tmp, vl);
            }

            for (int k = vlen; k < N; k++)
            {
                c[i * N + k] += a[i * N + j] * b[j * N + k];
            }
        }
    }
}

void reset_matrix(float *c, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            c[i * N + j] = 0.0f;
        }
    }
}

int main()
{
    const int N = 200;
    float arr1[N][N];
    float arr2[N][N];
    float arr3[N][N] = {0};

    fillMatrix(&arr1[0][0], N);
    fillMatrix(&arr2[0][0], N);

    for (int i = 0; i < 15; i++)
    {
        reset_matrix(&arr3[0][0], N);
        naive_matmul(&arr1[0][0], &arr2[0][0], &arr3[0][0], N);

        reset_matrix(&arr3[0][0], N);
        vectorized_matmul(&arr1[0][0], &arr2[0][0], &arr3[0][0], N);
    }

    reset_matrix(&arr3[0][0], N);

    auto start_naive = chrono::high_resolution_clock::now();
    naive_matmul(&arr1[0][0], &arr2[0][0], &arr3[0][0], N);
    auto end_naive = chrono::high_resolution_clock::now();

    auto naive_duration = chrono::duration_cast<chrono::microseconds>(end_naive - start_naive).count();
    cout << "Naive " << naive_duration << " ms" << endl;

    reset_matrix(&arr3[0][0], N);

    auto start_vec = chrono::high_resolution_clock::now();
    vectorized_matmul(&arr1[0][0], &arr2[0][0], &arr3[0][0], N);
    auto end_vec = chrono::high_resolution_clock::now();

    auto vec_duration = chrono::duration_cast<chrono::microseconds>(end_vec - start_vec).count();
    cout << "vector :" << vec_duration << " ms" << endl;

    return 0;
}