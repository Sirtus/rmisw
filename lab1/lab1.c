#include "stdio.h"
#include "stdlib.h"

#define A_ROWS 4
#define A_COLS 2
#define B_COLS 5
#define SIZE_THRESH 2

typedef struct Matrix
{
    int **values;
    int rows;
    int cols;
} Matrix;

typedef struct Slice
{
    int rs;
    int cs;
    int size;
} Slice; // [rs: rs + size, cs: cs + size]

void printMatrix(Matrix matrix)
{
    for (int i = 0; i < matrix.rows; ++i)
    {
        printf("[");
        for (int j = 0; j < matrix.cols; ++j)
        {
            if (j != matrix.cols - 1)
                printf("%5d, ", matrix.values[i][j]);
            else
                printf("%5d", matrix.values[i][j]);
        }
        printf("]\n");
    }
}

Matrix classic(Matrix A, Matrix B)
{
    int **C = calloc(A.rows, sizeof(int *));

    for (int i = 0; i < A.rows; ++i)
        C[i] = calloc(B.cols, sizeof(int));

    int sum = 0;
    for (int i = 0; i < A.rows; ++i)
    {
        for (int j = 0; j < B.cols; ++j)
        {
            for (int k = 0; k < A.cols; ++k)
            {
                sum += A.values[i][k] * B.values[k][j];
            }
            C[i][j] = sum;
            sum = 0;
        }
    }

    Matrix M = {.values = C, .rows = A.rows, .cols = B.cols};
    return M;
}

void classic_(Matrix A, Matrix B, Matrix C, Slice a, Slice b, Slice c)
{
    int sum = 0;
    for (int i = 0; i < c.size; ++i)
    {
        for (int j = 0; j < c.size; ++j)
        {
            for (int k = 0; k < c.size; ++k)
            {
                sum += A.values[i + a.rs][k + a.cs] * B.values[k + b.rs][j + b.cs];
            }
            // we are adding inplace, so assume partial sum was already there
            C.values[i + c.rs][j + c.cs] += sum;
            sum = 0;
        }
    }
    // printf("Filled slice rs=%d, cs=%d, size=%d \n", c.rs, c.cs, c.size);
    // printMatrix(C);
    // printf("\n");
}

void multiply_rec(Matrix A, Matrix B, Matrix C, Slice a, Slice b, Slice c)
{
    if (a.size <= SIZE_THRESH)
    {
        classic_(A, B, C, a, b, c);
        return;
    }

    // Binet block multiplication
    int size = (int)(a.size / 2);
    Slice a11 = {.rs = a.rs, .cs = a.cs, .size = size};
    Slice a12 = {.rs = a.rs, .cs = a.cs + size, .size = size};
    Slice a21 = {.rs = a.rs + size, .cs = a.cs, .size = size};
    Slice a22 = {.rs = a.rs + size, .cs = a.cs + size, .size = size};

    Slice b11 = {.rs = b.rs, .cs = b.cs, .size = size};
    Slice b12 = {.rs = b.rs, .cs = b.cs + size, .size = size};
    Slice b21 = {.rs = b.rs + size, .cs = b.cs, .size = size};
    Slice b22 = {.rs = b.rs + size, .cs = b.cs + size, .size = size};

    Slice c11 = {.rs = c.rs, .cs = c.cs, .size = size};
    Slice c12 = {.rs = c.rs, .cs = c.cs + size, .size = size};
    Slice c21 = {.rs = c.rs + size, .cs = c.cs, .size = size};
    Slice c22 = {.rs = c.rs + size, .cs = c.cs + size, .size = size};

    // up-left
    multiply_rec(A, B, C, a11, b11, c11);
    multiply_rec(A, B, C, a12, b21, c11);

    // up-right
    multiply_rec(A, B, C, a11, b12, c12);
    multiply_rec(A, B, C, a12, b22, c12);

    // down-left
    multiply_rec(A, B, C, a21, b11, c21);
    multiply_rec(A, B, C, a22, b21, c21);

    // down-right
    multiply_rec(A, B, C, a21, b12, c22);
    multiply_rec(A, B, C, a22, b22, c22);
}

Matrix multiply(Matrix A, Matrix B)
{
    int **C = calloc(A.rows, sizeof(int *));
    for (int i = 0; i < A.rows; ++i)
        C[i] = calloc(B.cols, sizeof(int));
    Matrix M = {.values = C, .rows = A.rows, .cols = B.cols};
    Slice initial = {.rs = 0, .cs = 0, .size = A.rows};

    multiply_rec(A, B, M, initial, initial, initial);

    return M;
}

void free_matrix(Matrix M)
{
    for (int i = 0; i < M.rows; i++)
        free(M.values[i]);
    free(M.values);
}

int main()
{
    const int N = 16;

    int a[16][16] = {
        {1, 3, 1, 7, 1, 8, 8, 8, 4, 1, 4, 5, 2, 1, 1, 2},
        {1, 9, 5, 6, 5, 3, 7, 3, 1, 1, 5, 2, 9, 5, 9, 1},
        {8, 5, 1, 4, 2, 7, 8, 9, 5, 8, 5, 8, 6, 8, 5, 7},
        {1, 7, 4, 4, 8, 5, 3, 9, 8, 1, 6, 8, 6, 4, 9, 3},
        {9, 1, 2, 1, 9, 1, 6, 2, 2, 7, 7, 5, 5, 9, 3, 2},
        {8, 8, 5, 5, 5, 3, 8, 2, 9, 4, 1, 1, 1, 5, 8, 7},
        {7, 1, 2, 6, 5, 1, 7, 5, 4, 2, 8, 6, 4, 1, 8, 8},
        {3, 1, 6, 5, 1, 3, 5, 5, 5, 4, 7, 4, 2, 1, 2, 7},
        {7, 8, 1, 5, 7, 7, 9, 4, 1, 3, 9, 1, 8, 4, 3, 2},
        {5, 6, 7, 5, 2, 1, 2, 5, 8, 4, 9, 7, 2, 4, 3, 3},
        {3, 8, 1, 9, 7, 6, 6, 7, 5, 5, 2, 3, 3, 2, 7, 5},
        {2, 7, 4, 5, 9, 2, 6, 3, 9, 2, 8, 8, 6, 3, 6, 9},
        {9, 7, 6, 1, 6, 4, 9, 8, 7, 3, 6, 8, 5, 5, 9, 6},
        {2, 4, 8, 2, 6, 3, 7, 5, 2, 9, 9, 4, 4, 7, 4, 5},
        {8, 1, 9, 4, 5, 8, 7, 2, 5, 1, 2, 9, 7, 7, 4, 6},
        {3, 7, 8, 9, 3, 2, 7, 7, 7, 8, 8, 1, 4, 3, 3, 8}};
    int b[16][16] = {
        {2, 3, 4, 4, 3, 5, 6, 7, 9, 5, 6, 4, 2, 1, 7, 5},
        {4, 3, 5, 3, 9, 8, 2, 7, 3, 1, 2, 8, 4, 5, 7, 3},
        {7, 5, 5, 3, 8, 5, 4, 5, 7, 6, 9, 4, 4, 6, 6, 1},
        {9, 3, 6, 7, 2, 1, 1, 6, 6, 3, 2, 1, 5, 8, 7, 6},
        {2, 2, 3, 7, 4, 7, 3, 5, 7, 5, 4, 1, 6, 8, 5, 5},
        {5, 1, 4, 6, 7, 7, 1, 4, 9, 5, 3, 8, 5, 6, 6, 6},
        {5, 4, 1, 2, 4, 7, 1, 3, 2, 1, 2, 5, 3, 1, 5, 4},
        {5, 7, 6, 9, 8, 7, 4, 2, 5, 7, 5, 7, 1, 1, 5, 1},
        {2, 4, 6, 9, 2, 9, 9, 3, 8, 5, 3, 4, 3, 6, 4, 9},
        {2, 6, 5, 8, 9, 5, 4, 4, 7, 1, 7, 6, 5, 3, 5, 2},
        {6, 6, 3, 9, 9, 8, 1, 3, 9, 9, 6, 2, 4, 2, 9, 5},
        {2, 1, 9, 4, 3, 4, 8, 8, 7, 7, 7, 1, 8, 4, 7, 2},
        {2, 2, 7, 5, 3, 7, 5, 5, 7, 9, 7, 4, 7, 7, 1, 7},
        {5, 7, 3, 2, 3, 6, 6, 2, 1, 1, 7, 7, 2, 5, 6, 8},
        {9, 2, 5, 7, 2, 4, 2, 1, 7, 5, 7, 2, 5, 3, 1, 5},
        {8, 4, 4, 4, 1, 9, 6, 1, 3, 9, 9, 8, 7, 4, 4, 3}};

    int **A = calloc(N, sizeof(int *));
    for (int i = 0; i < N; ++i)
    {
        A[i] = calloc(N, sizeof(int));
        for (int j = 0; j < N; ++j)
            A[i][j] = a[i][j];
    }
    Matrix M1 = {.values = A, .rows = N, .cols = N};

    printf("Matrix A:\n");
    printMatrix(M1);

    int **B = calloc(N, sizeof(int *));
    for (int i = 0; i < N; ++i)
    {
        B[i] = calloc(N, sizeof(int));
        for (int j = 0; j < N; ++j)
            B[i][j] = b[i][j];
    }
    Matrix M2 = {.values = B, .rows = N, .cols = N};
    printf("Matrix B:\n");
    printMatrix(M2);

    printf("Matrix C = AB:\n");
    Matrix C = multiply(M1, M2);
    printMatrix(C);

    free_matrix(M1);
    free_matrix(M2);
    free_matrix(C);

    return 0;
}