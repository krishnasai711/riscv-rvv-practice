#include <iostream>
#include <riscv_vector.h>
#include "helper.h"
using namespace std;

int main()
{
    int N;
    cout << "Enter value for N:";
    cin >> N;
    float arr[N][N];
    float tmp;
    size_t vlmax = __riscv_vsetvlmax_e32m1();
    cout << "vl max is " << vlmax << "\n";

    fillMatrix(&arr[0][0], N);

    cout << "Before rotation:\n";
    printMatrix(&arr[0][0], N);

    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j += vlmax)
        {
            size_t vl = __riscv_vsetvl_e32m1(N - j);
            vfloat32m1_t row = __riscv_vle32_v_f32m1(&arr[i][j], vl);
            vfloat32m1_t col = __riscv_vlse32_v_f32m1(&arr[j][i], N * sizeof(float), vl);
            __riscv_vsse32_v_f32m1(&arr[j][i], N * sizeof(float), row, vl);
            __riscv_vse32_v_f32m1(&arr[i][j], col, vl);
        }
    }

    cout << "after transpose:\n";
    printMatrix(&arr[0][0], N);

    for (int i = 0; i < N; i++)
    {
        float temp_row[N];
        int k = N;
        for (size_t vl; k > 0; k -= vl)
        {
            vl = __riscv_vsetvl_e64m1(k);
            uint32_t ind[vl];
            for (int j = 0; j < vl; j++)
            {
                ind[j] = vl - j - 1;
            }
            vuint32m1_t index = __riscv_vle32_v_u32m1(ind, vl);
            vfloat32m1_t row = __riscv_vle32_v_f32m1(&arr[i][N - k], vl);
            row = __riscv_vrgather_vv_f32m1(row, index, vl);
            __riscv_vse32_v_f32m1(&temp_row[k - vl], row, vl);
        }

        for (int j = 0; j < N; j++)
        {
            arr[i][j] = temp_row[j];
        }
    }

    cout << "After rotation:\n";
    printMatrix(&arr[0][0], N);

    return 0;
}