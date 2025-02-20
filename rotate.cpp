#include <iostream>
#include <riscv_vector.h>
#include "helper.h"
using namespace std;

int main()
{
    int N = 200;

    float arr[3][N][N];
    size_t vlmax = __riscv_vsetvlmax_e32m1();

    fill(&arr[0][0][0], N);

    // cout << "Before rotation:\n";
    // print(&arr[0][0][0], N);

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j += vlmax)
            {
                size_t vl = __riscv_vsetvl_e32m1(N - j);
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
            int k = N;
            float temp_row[N];
            for (int j = 0; j < N; j++)
            {
                temp_row[j] = arr[c][i][j];
            }
            for (size_t vl; k > 0; k -= vl)
            {
                vl = __riscv_vsetvl_e32m1(k);
                uint32_t ind[vl];
                for (int j = 0; j < vl; j++)
                {
                    ind[j] = vl - j - 1;
                }

                vuint32m1_t index = __riscv_vle32_v_u32m1(ind, vl);
                vfloat32m1_t row = __riscv_vle32_v_f32m1(&temp_row[N - k], vl);
                row = __riscv_vrgather_vv_f32m1(row, index, vl);
                __riscv_vse32_v_f32m1(&arr[c][i][k - vl], row, vl);
            }
        }
    }

    // cout << "After rotation:\n";
    // print(&arr[0][0][0], N);

    return 0;
}