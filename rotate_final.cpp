#include <iostream>
#include <chrono>
#include <riscv_vector.h>
#include "helper.h"
using namespace std;

void naive_rotate(float arr[3][200][200], int N)
{
    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                swap(arr[c][i][j], arr[c][j][i]);
            }
        }
    }

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N / 2; j++)
            {
                swap(arr[c][i][j], arr[c][i][N - j - 1]);
            }
        }
    }
}

void vectorized_rotate(float arr[3][200][200], int N)
{
    size_t vlmax = __riscv_vsetvlmax_e32m1();

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j += vlmax)
            {
                size_t vl = __riscv_vsetvl_e32m1(min(vlmax, static_cast<size_t>(N - j)));
                vfloat32m1_t row = __riscv_vle32_v_f32m1(&arr[c][i][j], vl);
                vfloat32m1_t col = __riscv_vlse32_v_f32m1(&arr[c][j][i], N * sizeof(float), vl);
                __riscv_vsse32_v_f32m1(&arr[c][j][i], N * sizeof(float), row, vl);
                __riscv_vse32_v_f32m1(&arr[c][i][j], col, vl);
            }
        }
    }

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < N; i++)
        {

            float temp_row[N];
            for (int j = 0; j < N; j++)
            {
                temp_row[j] = arr[c][i][j];
            }

            int remaining = N;
            for (size_t vl; remaining > 0; remaining -= vl)
            {
                vl = __riscv_vsetvl_e32m1(remaining);

                uint32_t ind[vl];
                for (size_t j = 0; j < vl; j++)
                {
                    ind[j] = vl - j - 1;
                }

                vuint32m1_t index = __riscv_vle32_v_u32m1(ind, vl);
                vfloat32m1_t row = __riscv_vle32_v_f32m1(&temp_row[N - remaining], vl);
                row = __riscv_vrgather_vv_f32m1(row, index, vl);
                __riscv_vse32_v_f32m1(&arr[c][i][remaining - vl], row, vl);
            }
        }
    }
}

void copy_array(float src[3][200][200], float dst[3][200][200], int N)
{
    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                dst[c][i][j] = src[c][i][j];
            }
        }
    }
}

int main()
{
    int N = 200;

    float original[3][200][200];
    float naive_result[3][200][200];
    float vector_result[3][200][200];
    fill(&original[0][0][0], N);

    for (int i = 0; i < 15; i++)
    {
        copy_array(original, naive_result, N);
        naive_rotate(naive_result, N);
        copy_array(original, vector_result, N);
        vectorized_rotate(vector_result, N);
    }

    copy_array(original, naive_result, N);

    auto start_naive = chrono::high_resolution_clock::now();
    naive_rotate(naive_result, N);
    auto end_naive = chrono::high_resolution_clock::now();

    auto naive_duration = chrono::duration_cast<chrono::microseconds>(end_naive - start_naive).count();
    cout << "Naive :" << naive_duration << " ms" << endl;

    copy_array(original, vector_result, N);

    auto start_vec = chrono::high_resolution_clock::now();
    vectorized_rotate(vector_result, N);
    auto end_vec = chrono::high_resolution_clock::now();

    auto vec_duration = chrono::duration_cast<chrono::microseconds>(end_vec - start_vec).count();
    cout << "Vector : " << vec_duration << " ms" << endl;

    return 0;
}