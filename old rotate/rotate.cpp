#include <iostream>
#include <riscv_vector.h>
#include "helper.h"
using namespace std;

int main()
{
    int N = 8;
    float arr[N][N];
    int tmp;
    size_t vl = __riscv_vsetvlmax_e32m1();
    const unsigned int ind[N] = {7, 6, 5, 4, 3, 2, 1, 0};
    vuint32m1_t index = __riscv_vle32_v_u32m1(ind, vl);
    fillMatrix(&arr[0][0], N);

    cout << "before :\n";
    printMatrix(&arr[0][0], N);

    for (int i = 0; i < N - 1; i++)
    {
        size_t vlr = __riscv_vsetvl_e32m1(N - (i + 1));

        vfloat32m1_t row_i = __riscv_vle32_v_f32m1(&arr[i][i + 1], vlr);

        vfloat32m1_t col;
        float temp_col[N];

        for (int k = 0; k < N - (i + 1); k++)
        {
            temp_col[k] = arr[i + 1 + k][i];
        }

        col = __riscv_vle32_v_f32m1(temp_col, vlr);

        __riscv_vse32_v_f32m1(&arr[i][i + 1], col, vlr);

        float rowtmp[N];
        __riscv_vse32_v_f32m1(rowtmp, row_i, vlr);
        for (int k = 0; k < N - (i + 1); k++)
        {
            arr[i + 1 + k][i] = rowtmp[k];
        }
    }

    cout << "After transpose:\n";
    printMatrix(&arr[0][0], N);

    for (int i = 0; i < N; i++)
    {
        vfloat32m1_t row = __riscv_vle32_v_f32m1(&arr[i][0], vl);
        row = __riscv_vrgather_vv_f32m1(row, index, vl);
        __riscv_vse32_v_f32m1(&arr[i][0], row, vl);
    }

    cout << "After rotation:\n";
    printMatrix(&arr[0][0], N);

    return 0;
}