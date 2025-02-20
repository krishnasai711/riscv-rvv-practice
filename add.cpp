#include <iostream>
#include <riscv_vector.h>
#include "helper.h"

using namespace std;

int main()
{
    int N = 200;
    float arr1[N][N];
    float arr2[N][N];
    float arr3[N][N] = {0};

    fillMatrix(&arr1[0][0], N);
    fillMatrix(&arr2[0][0], N);

    size_t vlmax = __riscv_vsetvlmax_e32m1(); // Max vector length

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j += vlmax)
        {
            size_t vl = __riscv_vsetvl_e32m1(N - j);
            vfloat32m1_t v1 = __riscv_vle32_v_f32m1(&arr1[i][j], vl);
            vfloat32m1_t v2 = __riscv_vle32_v_f32m1(&arr2[i][j], vl);
            vfloat32m1_t v3 = __riscv_vfadd_vv_f32m1(v1, v2, vl);
            __riscv_vse32_v_f32m1(&arr3[i][j], v3, vl);
        }
    }

    return 0;
}
