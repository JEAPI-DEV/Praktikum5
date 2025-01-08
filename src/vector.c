#include "vector.h"
#include <malloc.h>
#include <stdlib.h>

#define RED "\033[31m"
#define RESET "\033[0m"

void withinBoundCheck(vector Vector, unsigned short j) {
    if (j >= size(Vector)) {
        printf(RED "Error: Index out of bounds!\n" RESET);
        exit(-1);
    }
}

void vectorSameSizeCheck(vector VectorA, vector VectorB, char* operation) {
    if (size(VectorA) != size(VectorB)) {
        printf(RED "Error: Vector size does not match %s!\n" RESET, operation);
        exit(-1);
    }
}

struct vector_struct {
    double *data;
    unsigned short size;
};

vector createVector(unsigned short n) {
    size_t vector_size = n * sizeof(double);
    vector Vector = calloc(1, sizeof(struct vector_struct) + vector_size);
    if (!Vector) {
        printf(RED "Memory allocation failed for vector structure and data.\n" RESET);
        exit(-1);
    }
    Vector->size = n;
    Vector->data = (double*)(Vector + 1);

    return Vector;
}

void rmVector(vector Vector) {
    free(Vector);
    Vector = NULL; /* Fix for -fsanitize=address bug.*/
}

unsigned short size(vector Vector) {
    return Vector->size;
}

double* values(vector Vector) {
    return Vector->data;
}

double getValue(vector Vector, unsigned short j) {
    withinBoundCheck(Vector, j);
    return Vector->data[j];
}

void setValue(vector Vector, unsigned short j, double value) {
    withinBoundCheck(Vector, j);
    Vector->data[j] = value;
}

double vectorDotVector(vector VectorA, vector VectorB) {
    double sum = 0.0;
    unsigned short i;
    vectorSameSizeCheck(VectorA, VectorB, "Multiplication");
    for (i = 0; i < size(VectorA); i++) {
        sum += VectorA->data[i] * VectorB->data[i];
    }
    return sum;
}

vector vectorPlusVector(vector VectorA, vector VectorB) {
    vector VectorC;
    unsigned short i;
    vectorSameSizeCheck(VectorA, VectorB, "Addition");
    VectorC = createVector(size(VectorA));
    for (i = 0; i < size(VectorA); i++) {
        VectorC->data[i] = VectorA->data[i] + VectorB->data[i];
    }
    return VectorC;
}