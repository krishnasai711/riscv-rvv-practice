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
            arr3[i][j] = arr1[i][j] + arr2[i][j];
        }
    }

    return 0;
}