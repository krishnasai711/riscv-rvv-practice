#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "helper.h"
using namespace std;

void fillMatrix(float *matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i * N + j] = i + j + 1;
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

void fill(float *arr, int N)
{
    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {

                arr[c * N * N + i * N + j] = i + j + 1;
            }
        }
    }
}

void print(float *arr, int N)
{

    for (int c = 0; c < 1; c++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cout << arr[c * N * N + i * N + j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}