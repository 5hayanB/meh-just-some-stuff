#include <stdio.h>

int main(void)
{
    int a[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    },
    b[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    },
    c[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    printf("Output matrix:\n");
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            printf("%4d", c[x][y]);
        }
        printf("\n");
    }

    return 0;
}
