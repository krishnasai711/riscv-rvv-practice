#include <iostream>
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

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                arr3[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }

    // printMatrix(&arr3[0][0], N);
    // if (checkMatrix(&arr1[0][0], &arr2[0][0], &arr3[0][0], N))
    //     cout << "Process is correct" << "\n";
    // else
    //     cout << "There is some error" << "\n";

    return 0;
}