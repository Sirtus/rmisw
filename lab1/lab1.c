#include "stdio.h"
#include "stdlib.h"

#define A_ROWS 4
#define A_COLS 2
#define B_COLS 5

int** classic(int** A, int** B, int a_rows, int a_cols, int b_cols) {
    int** C = calloc(a_rows, sizeof(int*));
    
    for(int i = 0; i < a_rows; ++i) C[i] = calloc(b_cols, sizeof(int));

    int sum = 0;
    for(int i = 0; i < a_rows; ++i) {
        for(int j = 0; j < b_cols; ++j) {
            for(int k = 0; k < a_cols; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
            sum = 0;
        }
    }
    
    return C;
} 

void printMatrix(int** matrix, int n_rows, int n_cols) {
    for(int i = 0; i < n_rows; ++i) {
        printf("[");
        for(int j = 0; j < n_cols; ++j) {
            if (j != n_cols-1) printf("%5d, ", matrix[i][j]);
            else printf("%5d", matrix[i][j]);
            
            
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
    printf("Matrix A:\n");
    printMatrix(A, a_rows, a_cols);

    int** B = calloc(b_rows, sizeof(int*));
    for(int i = 0; i < b_rows; ++i) {
        B[i] = calloc(b_cols, sizeof(int));
        for(int j = 0; j < b_cols; ++j) B[i][j] = b[i][j];
    }
    printf("Matrix B:\n");
    printMatrix(B, b_rows, b_cols);

    int** C;
    printf("Matrix C = AB:\n");
    C = classic(A, B, a_rows, a_cols, b_cols);
    printMatrix(C, c_rows, c_cols);

    return 0;
}