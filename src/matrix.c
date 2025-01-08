#include "matrix.h"
#include "vector.h"
#include <malloc.h>
#include <stdlib.h>

#define RED "\033[31m"
#define RESET "\033[0m"

#define DEBUG_MATRIX 0

struct matrix_struct {
    double **data;
    unsigned short rows, cols;
};

void printMatrix(matrix Matrix) {
    unsigned short i, j;
    for (i = 0; i < rows(Matrix); i++) {
        for (j = 0; j < cols(Matrix); j++) {
            printf("%.2f ", getEntry(Matrix, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

matrix createMatrix(unsigned short rows, unsigned short cols) {
    double *dataBlock;
    unsigned short i;
    size_t row_pointers_size = rows * sizeof(double*);
    size_t data_block_size = rows * cols * sizeof(double);
    matrix Matrix = calloc(1, sizeof(struct matrix_struct) + row_pointers_size + data_block_size);

    if (!Matrix) {
        printf(RED "Memory allocation failed for matrix structure and data.\n" RESET);
        exit(-1);
    }
    Matrix->rows = rows;
    Matrix->cols = cols;

    Matrix->data = (double**)(Matrix + 1);
    dataBlock = (double*)(Matrix->data + rows);

    for (i = 0; i < rows; i++) {
            Matrix->data[i] = dataBlock + i * cols;
    }

    return Matrix;
}

void rmMatrix(matrix Matrix) {
    free(Matrix);
    Matrix = NULL;
}

unsigned short rows(matrix Matrix) {
    return Matrix->rows;
}

unsigned short cols(matrix Matrix) {
    return Matrix->cols;
}

double** data(matrix Matrix) {
    return Matrix->data;
}

double getEntry(matrix Matrix, unsigned short row, unsigned short col) {
    if (row >= Matrix->rows || col >= Matrix->cols) {
        printf(RED "Error: Matrix index out of bounds. Row: %u, Col: %u\n" RESET, row, col);
        exit(-1);
    }
    return Matrix->data[row][col];
}

void setEntry(matrix Matrix, unsigned short row, unsigned short col, double value) {
    if (row >= Matrix->rows || col >= Matrix->cols) {
        printf(RED "Error: Matrix index out of bounds. Row: %u, Col: %u\n" RESET, row, col);
        exit(-1);
    }
    Matrix->data[row][col] = value;
}

matrix matrixDotMatrix(matrix MatrixA, matrix MatrixB) {
    matrix MatrixC;
    unsigned short i, j, k;
    double sum = 0;

    if (cols(MatrixA) != rows(MatrixB)) {
        printf(RED "Error: Matrix dimensions mismatch for multiplication."
                        "MatrixA cols: %u, MatrixB rows: %u\n" RESET, cols(MatrixA), rows(MatrixB));
        exit(-1);
    }

    MatrixC = createMatrix(rows(MatrixA), cols(MatrixB));

    printMatrix(MatrixA);
    printMatrix(MatrixB);

    for (i = 0; i < rows(MatrixA); i++) {
        for (j = 0; j < cols(MatrixB); j++) {
            sum = 0;
            for (k = 0; k < cols(MatrixA); k++) {
                sum += getEntry(MatrixA, i, k) * getEntry(MatrixB, k, j);
            }
            setEntry(MatrixC, i, j, sum);
        }
    }
    return MatrixC;
}

matrix matrixPlusMatrix(matrix MatrixA, matrix MatrixB) {
    matrix MatrixC;
    unsigned short i, j;

    if (rows(MatrixA) != rows(MatrixB) || cols(MatrixA) != cols(MatrixB)) {
        printf(RED "Error: Matrix dimensions mismatch for addition. "
                        "MatrixA dimensions: %ux%u, MatrixB dimensions: %ux%u\n" RESET, rows(MatrixA), cols(MatrixA), rows(MatrixB), cols(MatrixB));
        exit(-1);
    }

    MatrixC = createMatrix(rows(MatrixA), cols(MatrixA));
    for (i = 0; i < rows(MatrixA); i++) {
        for (j = 0; j < cols(MatrixA); j++) {
            setEntry(MatrixC, i, j, getEntry(MatrixA, i, j) + getEntry(MatrixB, i, j));
        }
    }
    return MatrixC;
}

vector matrixDotVector(matrix MatrixA, vector VectorA) {
    vector VectorB;
    unsigned short i, j;
    double sum;

    if (cols(MatrixA) != size(VectorA)) {
        printf(RED "Error: Matrix and vector dimensions mismatch for multiplication. "
                        "Matrix cols: %u, Vector size: %u\n" RESET, cols(MatrixA), size(VectorA));
        exit(-1);
    }

    VectorB = createVector(rows(MatrixA));
    for (i = 0; i < rows(MatrixA); i++) {
        sum = 0.0;
        for (j = 0; j < cols(MatrixA); j++) {
            sum += getEntry(MatrixA, i, j) * getValue(VectorA, j);
        }
        setValue(VectorB, i, sum);
    }
    return VectorB;
}