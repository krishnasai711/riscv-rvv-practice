#include <iostream>
#include <riscv_vector.h>
#include "helper.h"
using namespace std;

int main()
{
    int N = 15;
    float arr1[N][N];
    float arr2[N][N];
    float arr3[N][N] = {0};

    fillMatrix(&arr1[0][0], N);
    fillMatrix(&arr2[0][0], N);

    size_t vl = __riscv_vsetvl_e32m1(8);
    int vlen = (N / 8) * 8;

    for (int i = 0; i < N; i++)
    {
        vfloat32m1_t tmp = __riscv_vfmv_v_f_f32m1(0, vl);

        for (int j = 0; j < N; j++)
        {
            float scalar = arr1[i][j];
            vfloat32m1_t vec_a = __riscv_vfmv_v_f_f32m1(scalar, vl);

            for (int k = 0; k < vlen; k += 8)
            {
                vfloat32m1_t tmp = __riscv_vle32_v_f32m1(&arr3[i][k], vl);
                vfloat32m1_t vec_b = __riscv_vle32_v_f32m1(&arr2[j][k], vl);
                tmp = __riscv_vfmacc_vv_f32m1(tmp, vec_a, vec_b, vl);
                __riscv_vse32_v_f32m1(&arr3[i][k], tmp, vl);
            }

            for (int k = vlen; k < N; k++)
            {
                arr3[i][k] += arr1[i][j] * arr2[j][k];
            }
        }
    }

    printMatrix(&arr3[0][0], N);
    if (checkMatrix(&arr1[0][0], &arr2[0][0], &arr3[0][0], N))
        cout << "Process is correct" << "\n";
    else
        cout << "There is some error" << "\n";

    return 0;
}