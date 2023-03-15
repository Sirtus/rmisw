#include "stdio.h"
#include "stdlib.h"
#include "input_matrix.h"

#define A_ROWS 4
#define A_COLS 2
#define B_COLS 5
#define SIZE_THRESH 2

typedef struct Matrix
{
    float **values;
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
                printf("%f, ", matrix.values[i][j]);
            else
                printf("%f", matrix.values[i][j]);
        }
        printf("]\n");
    }
}

Matrix classic(Matrix A, Matrix B)
{
    float **C = calloc(A.rows, sizeof(float *));

    for (int i = 0; i < A.rows; ++i)
        C[i] = calloc(B.cols, sizeof(float));

    float sum = 0;
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
    float sum = 0;
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
    float **C = calloc(A.rows, sizeof(float *));
    for (int i = 0; i < A.rows; ++i)
        C[i] = calloc(B.cols, sizeof(float));
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
    float **A = calloc(N, sizeof(float *));
    for (int i = 0; i < N; ++i)
    {
        A[i] = calloc(N, sizeof(float));
        for (int j = 0; j < N; ++j)
            A[i][j] = a[i][j];
    }
    Matrix M1 = {.values = A, .rows = N, .cols = N};

    printf("Matrix A:\n");
    printMatrix(M1);

    float **B = calloc(N, sizeof(float *));
    for (int i = 0; i < N; ++i)
    {
        B[i] = calloc(N, sizeof(float));
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