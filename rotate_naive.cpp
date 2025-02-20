#include <iostream>
#include "helper.h"
using namespace std;

int main()
{
    int N = 200;

    float arr[3][N][N];
    fill(&arr[0][0][0], N);

    // cout << "Before rotation:\n";
    // print(&arr[0][0][0], N);

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

    // cout << "After rotation:\n";
    // print(&arr[0][0][0], N);

    return 0;
}
