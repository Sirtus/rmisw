#include "stdio.h"
#include "stdlib.h"

#define A_ROWS 4
#define A_COLS 2
#define B_COLS 5

typedef struct Matrix {
    int** values;
    int rows;
    int cols;
} Matrix;


Matrix classic(Matrix A, Matrix B) {
    int** C = calloc(A.rows, sizeof(int*));
    
    for(int i = 0; i < A.rows; ++i) C[i] = calloc(B.cols, sizeof(int));

    int sum = 0;
    for(int i = 0; i < A.rows; ++i) {
        for(int j = 0; j < B.cols; ++j) {
            for(int k = 0; k < A.cols; ++k) {
                sum += A.values[i][k] * B.values[k][j];
            }
            C[i][j] = sum;
            sum = 0;
        }
    }
    
    Matrix M = {.values = C, .rows = A.rows, .cols=B.cols};
    return M;
} 


void printMatrix(Matrix matrix) {
    for(int i = 0; i < matrix.rows; ++i) {
        printf("[");
        for(int j = 0; j < matrix.cols; ++j) {
            if (j != matrix.cols-1) printf("%5d, ", matrix.values[i][j]);
            else printf("%5d", matrix.values[i][j]);
            
            
        }
        printf("]\n");
    }
} 

int main() {
    const int a_rows = A_ROWS;
    const int a_cols = A_COLS;
    const int b_rows = a_cols;
    const int b_cols = B_COLS;
    const int c_rows = a_rows;
    const int c_cols = b_cols;


    int a[A_ROWS][A_COLS] = {
        {2, 1},
        {3, 2},
        {2, 4},
        {5, 1}
    };

    int b[A_COLS][B_COLS] = {
        {2, 3, 4, 1, 0},
        {3, 2, 4, 3, 1}
    };

    int** A = calloc(a_rows, sizeof(int*));
    for(int i = 0; i < a_rows; ++i) {
        A[i] = calloc(a_cols, sizeof(int));
        for(int j = 0; j < a_cols; ++j) A[i][j] = a[i][j];
    }
    Matrix M1 = {.values = A, .rows = a_rows, .cols = a_cols};

    printf("Matrix A:\n");
    printMatrix(M1);

    int** B = calloc(b_rows, sizeof(int*));
    for(int i = 0; i < b_rows; ++i) {
        B[i] = calloc(b_cols, sizeof(int));
        for(int j = 0; j < b_cols; ++j) B[i][j] = b[i][j];
    }
    Matrix M2 = {.values = B, .rows = b_rows, .cols = b_cols};
    printf("Matrix B:\n");
    printMatrix(M2);

    printf("Matrix C = AB:\n");
    Matrix C = classic(M1, M2);
    printMatrix(C);

    free(M1.values);
    free(M2.values);
    free(C.values);

    return 0;
}