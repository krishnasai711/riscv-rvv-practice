#include <iostream>
#include <cstdlib>
#include "helper.h"
using namespace std;

void fillMatrix(float *matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i * N + j] = static_cast<float>(rand() % 10);
        }
    }
}

void printMatrix(const float *matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << matrix[i * N + j] << " ";
        }
        cout << "\n";
    }
}

bool checkMatrix(const float *matrix1, const float *matrix2, const float *matrix3, int N)
{
    float *matrix4 = (float *)malloc(N * N * sizeof(float));
    if (matrix4 == NULL)
    {
        return false;
    }

    for (int i = 0; i < N * N; i++)
    {
        matrix4[i] = 0.0f;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                matrix4[i * N + j] += matrix1[i * N + k] * matrix2[k * N + j];
            }
        }
    }

    for (int i = 0; i < N * N; i++)
    {
        if (matrix3[i] != matrix4[i])
        {
            free(matrix4);
            return false;
        }
    }

    free(matrix4);
    return true;
}
