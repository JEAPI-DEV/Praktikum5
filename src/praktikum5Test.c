/*
 * DO NOT EDIT IT SHOULD COMPILE AS IT IS!
 *
 * common Pratikum V test routine
 *
 * University of Applied Sciences, Muenster, Germany
 * Lab for Computer Sciences.
 *
 *  @since:  23.11.2024
 *  @author: nwulff
 */
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "matrix.h"
#include "vector.h"

/** forward declaration of the test method.                        */
static void testPraktikumV();
/** forward declaration command line evaluation                    */
static void evaluateCmdLine(int argc, char **argv);
/**
 * the main bootstrap starting point
 * evaluating the command line arguments.
 */
int main(int argc, char **argv) {
	evaluateCmdLine(argc, argv);
	testPraktikumV();
	return EXIT_SUCCESS;
}

/** common global flags signal crashing methods to execute.        */
static int lets_crash_matrix_plus_matrix = 0;
static int lets_crash_vector_plus_vector = 0;
static int lets_crash_matrix_dot_matrix = 0;
static int lets_crash_matrix_dot_vector = 0;
static int lets_crash_vector_dot_vector = 0;
static int print_all = 0;
static int test_ctor = 0;

/**
 * evaluate the command line, show help
 * or set the crashing flags.
 */
static void evaluateCmdLine(int argc, char **argv) {
	if (argc > 1) {
		int j;
		for (j = 1; j < argc; j++) {
			if (strcmp("MpM", argv[j]) == 0) {
				lets_crash_matrix_plus_matrix = 1;
			} else if (strcmp("MxM", argv[j]) == 0) {
				lets_crash_matrix_dot_matrix = 1;
			} else if (strcmp("MxV", argv[j]) == 0) {
				lets_crash_matrix_dot_vector = 1;
			} else if (strcmp("VpV", argv[j]) == 0) {
				lets_crash_vector_plus_vector = 1;
			} else if (strcmp("VxV", argv[j]) == 0) {
				lets_crash_vector_dot_vector = 1;
			} else if (strcmp("INF", argv[j]) == 0) {
				test_ctor = 1;
			} else if (strcmp("PRINT", argv[j]) == 0) {
				print_all = 1;
			} else {
				static char *options[] = { "MpM: test matrix + matrix crash",
						"MxM: test matrix * matrix crash",
						"MxV: test matrix * vector crash",
						"VpV: test vector + vector crash",
						"VxV: test vector * vector crash", "",
						"INF: test malloc struct storage",
						"PRINT: show intermediate results" };
				int j, n = sizeof(options) / sizeof(char*);
				printf("HELP\n");
				printf("----------------------------------------\n");
				printf("%s <option>\n", argv[0]);
				printf("options are special crashing test cases:\n");
				printf("----------------------------------------\n\n");
				for (j = 0; j < n; j++)
					printf("%s\n", options[j]);

				printf("\nelse execute the standard tests\n");
				exit(EXIT_SUCCESS);
			}
		}
	}
}

#define N 4  /* first matrix row dimension.                        */
#define M 5  /* first matrix column | second matrix row dimension. */
#define O 3  /* second matrix column dimension.                    */

/** some macro and constants definitions. */
#define EPS 1.E-12
#define DABS(x)  ((x)<0 ? -(x):(x) )
#define EQUALS(x,y) (DABS(x-y)<EPS)
#define BEG(str) printf("%s:%d BEG %s \n",__FILE__,__LINE__,(str))
#define END(str) printf("%s:%d END %s \n",__FILE__,__LINE__,(str))
#define PTR(x)  ((void*) x)
#define PTRDIFF(x,y)  (((size_t) x) - ((size_t)y))

/** generate a 3 digit random number between -1 and 1.             */
static double rnd() {
	const int scale = 1000;
	static time_t seed = 0;
	double r;
	if (!seed) {
		seed = time(NULL);
		srand((unsigned int) seed);
	}
	r = ((2.0 * (rand() % scale)) / scale) - 1;
	return r;
}

/** compare matrix A and B assert they are equal.*/
static void checkMatrixEqual(matrix a, matrix b) {
	unsigned short i, j;
	assert(rows(a) == rows(b));
	assert(cols(a) == cols(b));
	for (i = 0; i < rows(a); i++)
		for (j = 0; j < cols(a); j++)
			assert(EQUALS(getEntry(a,i,j), getEntry(b,i,j)));
}
/** compare vector a and b assert they are equal.*/
static void checkVectorEqual(vector a, vector b) {
	unsigned short i;
	assert(size(a) == size(b));
	for (i = 0; i < size(a); i++)
		assert(EQUALS(getValue(a,i), getValue(b,i)));
}

/** print a matrix to a file output stream.  */
void fprintMatrix(FILE *f, matrix a, const char *fmt) {
	unsigned short j, k;
	assert(f != NULL);
	assert(a != NULL);
	assert(fmt != NULL);
	fprintf(f, "matrix rows: %d columns: %d\n", rows(a), cols(a));
	for (j = 0; j < rows(a); j++) {
		for (k = 0; k < cols(a); k++) {
			fprintf(f, fmt, getEntry(a, j, k));
		}
		fprintf(f, "%c", '\n');
	}
	fflush(f);
}
/** print a vector to a file output stream.  */
void fprintVector(FILE *f, vector v, const char *fmt) {
	unsigned short j;
	assert(f != NULL);
	assert(v != NULL);
	assert(fmt != NULL);

	fprintf(f, "vector size: %d \n", size(v));
	for (j = 0; j < size(v); j++) {
		fprintf(f, fmt, getValue(v, j));
	}
	fprintf(f, "%c", '\n');
	fflush(f);
}
/** some macros to print matrix or vector to stdout. */
#define printfVector(v,fmt) (fprintVector(stdout,v, fmt))
#define printVector(v) (printfVector(v,"%6.3f "))
#define printfMatrix(a,fmt) fprintMatrix(stdout,a,fmt)
#define printMatrix(a) printfMatrix(a,"%6.3f ")
/** utility macro to fill random matrix */
#define rndMatrix(m) {unsigned short mj,mk; \
	for (mj = 0; mj < rows(m); mj++) \
      for (mk = 0; mk < cols(m);setEntry(m, mj, mk, rnd()), mk++); }

/** utility macro to fill random vector. */
#define rndVector(v){unsigned short vj; \
	for (vj = 0; vj < size(v); setValue(v, vj, rnd()),vj++); }

/** test vector addition with wrong dimensions */
static void testWrongPlusVector() {
	vector a, b, c;
	BEG("wrong vector addition");
	a = createVector(5);
	b = createVector(7);
	c = vectorPlusVector(a, b);
	printf("a+b => c ");
	printVector(c);
	END("ERROR should not be reached!");
	assert(0);
}
/** test vector scalar product with wrong dimensions */
static void testWrongDotVector() {
	vector a, b;
	double c;
	BEG("wrong vector multiplication");
	a = createVector(5);
	b = createVector(2);
	c = vectorDotVector(a, b);
	printf("a*b => c=%f\n", c);
	END("ERROR should not be reached!");
	assert(0);
}
/** test matrix addition with wrong dimensions */
static void testWrongPlusMatrix() {
	matrix a, b, c;
	BEG("wrong matrix addition");
	a = createMatrix(3, 5);
	b = createMatrix(7, 2);
	c = matrixPlusMatrix(a, b);
	printf("A+B => C ");
	printMatrix(c);
	END("ERROR should not be reached!");
	assert(0);
}
/** test matrix multiplication with wrong dimensions */
static void testWrongDotMatrix() {
	matrix a, b, c;
	BEG("wrong matrix multiplication");
	a = createMatrix(3, 5);
	b = createMatrix(7, 2);
	c = matrixDotMatrix(a, b);
	printf("A*B => C ");
	printMatrix(c);
	END("ERROR should not be reached!");
	assert(0);
}
/** test matrix vector multiplication with wrong dimensions */
static void testWrongMatrixDotVector() {
	vector x, y;
	matrix a;
	BEG("wrong matrix vector multiplication");
	x = createVector(5);
	a = createMatrix(5, 2);
	y = matrixDotVector(a, x);
	printf("A*x => y");
	printVector(y);
	END("ERROR should not be reached!");
	assert(0);
}
/** test vector values and get */
void testVectorValues() {
	unsigned short i;
	vector v;
	double *ptrV;
	BEG("data getValues vector test");
	v = createVector(5 * M);
	rndVector(v);
	if (print_all) {
		printf("V ");
		printVector(v);
	}
	ptrV = values(v);
	for (i = 0; i < 2 * N; i++)
		assert(EQUALS(ptrV[i],getValue(v,i)));
	rmVector(v);
	END("data getValues vector test");
}

/** calculate vector scalar product */
static double calcVxV(double *x, double *y) {
	int i;
	double c = 0;
	for (i = 0; i < N; c += x[i] * y[i], i++)
		;
	return c;
}
/** calculate vector addition */
static void calcVpV(double *x, double *y, double *z) {
	int i;
	for (i = 0; i < N; z[i] = x[i] + y[i], i++)
		;
}
/** calculate matrix vector multiplication */
static void calcMxV(double *A[N], double *x, double *y) {
	unsigned short i, j;
	for (i = 0; i < N; i++)
		for (y[i] = 0, j = 0; j < M; y[i] += A[i][j] * x[j], j++)
			;
}
/** calculate matrix matrix multiplication */
static void calcMxM(double *A[N], double *B[M], double *C[N]) {
	unsigned short i, j, k;
	for (i = 0; i < N; i++)
		for (j = 0; j < O; j++)
			for (k = 0, C[i][j] = 0; k < M; C[i][j] += A[i][k] * B[k][j], k++)
				;
}
/** calculate matrix matrix addition */
static void calcMpM(double *A[N], double *B[N], double *C[N]) {
	unsigned short i, j;
	for (i = 0; i < N; i++)
		for (j = 0; j < M; C[i][j] = A[i][j] + B[i][j], j++)
			;
}

/** test vector plus vector */
static void testVpVector() {
	vector a, b, c, d;
	BEG("a+b vector test");
	a = createVector(N);
	b = createVector(N);
	d = createVector(N);
	rndVector(a);
	rndVector(b);
	calcVpV(values(a), values(b), values(d));
	c = vectorPlusVector(a, b);
	if (print_all) {
		printf("a ");
		printVector(a);
		printf("b ");
		printVector(b);
		printf("a+b => c ");
		printVector(c);
	}
	checkVectorEqual(d, c);
	rmVector(a);
	rmVector(b);
	rmVector(c);
	rmVector(d);
	END("a+b vector test");
}
/** test vector times vector */
void testVxVector() {
	vector a, b;
	double c, d;
	BEG("a*b vector test");
	a = createVector(N);
	b = createVector(N);
	rndVector(a);
	rndVector(b);
	d = calcVxV(values(a), values(b));
	c = vectorDotVector(a, b);
	if (print_all) {
		printf("a ");
		printVector(a);
		printf("b ");
		printVector(b);
		printf("a*b => c %f\n", c);
	}
	assert(EQUALS(d, c));
	rmVector(a);
	rmVector(b);
	END("a*b vector test");
}
/** test matrix times matrix */
void testMxMatrix() {
	matrix a, b, c, d;
	BEG("AxB matrix test");
	a = createMatrix(N, M);
	b = createMatrix(M, O);
	d = createMatrix(N, O);
	rndMatrix(a);
	rndMatrix(b);
	calcMxM(data(a), data(b), data(d));
	c = matrixDotMatrix(a, b);
	if (print_all) {
		printf("A ");
		printMatrix(a);
		printf("B ");
		printMatrix(b);
		printf("A*B => C ");
		printMatrix(c);
	}
	checkMatrixEqual(d, c);
	rmMatrix(a);
	rmMatrix(b);
	rmMatrix(c);
	rmMatrix(d);

	END("AxB matrix test");
}

/** test matrix data and get */
void testMatrixData() {
	unsigned short i, j;
	matrix a;
	double **ptrA;
	BEG("data getEntry matrix test");
	a = createMatrix(2 * N, 3 * M);
	rndMatrix(a);

	if (print_all) {
		printf("A ");
		printMatrix(a);
	}
	ptrA = data(a);
	for (i = 0; i < 2 * N; i++)
		for (j = 0; j < 3 * M; j++)
			assert(EQUALS(ptrA[i][j],getEntry(a,i,j)));
	rmMatrix(a);
	END("data getEntry matrix test");
}

/** test matrix plus matrix */
void testMpMatrix() {
	matrix a, b, c, d;
	BEG("A+B matrix test");
	a = createMatrix(N, M);
	b = createMatrix(N, M);
	d = createMatrix(N, M);
	rndMatrix(a);
	rndMatrix(b);
	calcMpM(data(a), data(b), data(d));
	c = matrixPlusMatrix(a, b);
	if (print_all) {
		printf("A ");
		printMatrix(a);
		printf("B ");
		printMatrix(b);
		printf("A+B => C ");
		printMatrix(c);
	}
	checkMatrixEqual(d, c);

	rmMatrix(a);
	rmMatrix(b);
	rmMatrix(c);
	rmMatrix(d);

	END("A+B matrix test");
}
/** test matrix times vector */
void testVxMatrix() {
	matrix a;
	vector x, c, d;
	BEG("A*x matrix vector test");
	a = createMatrix(N, M);
	x = createVector(M);
	d = createVector(N);
	rndMatrix(a);
	rndVector(x);
	calcMxV(data(a), values(x), values(d));
	c = matrixDotVector(a, x);
	if (print_all) {
		printf("A ");
		printMatrix(a);
		printf("x ");
		printVector(x);
		printf("A*x => y ");
		printfVector(c, "%5.2f ");
	}
	checkVectorEqual(d, c);
	rmMatrix(a);
	rmVector(x);
	rmVector(c);
	rmVector(d);
	END("A*x matrix vector test");
}
/** test the 3x3 matrix example. */
void test3x3Example() {
	matrix a, b, c;
	vector x, y, z;
	BEG("3x3 matrix example");

	x = createVector(3);
	assert(3 == size(x));
	setValue(x, 0, 1);
	setValue(x, 1, 2);
	setValue(x, 2, 4);

	z = createVector(3);
	assert(3 == size(z));
	setValue(z, 0, 17.25);
	setValue(z, 1, -26.5);
	setValue(z, 2, -28.5);

	a = createMatrix(3, 3);
	assert(3 == rows(a));
	assert(3 == cols(a));

	setEntry(a, 0, 0, 0.5);
	setEntry(a, 0, 1, -1);
	setEntry(a, 0, 2, -1);
	setEntry(a, 1, 0, -1);
	setEntry(a, 1, 1, 1);
	setEntry(a, 1, 2, 2);
	setEntry(a, 2, 0, -1);
	setEntry(a, 2, 1, 2);
	setEntry(a, 2, 2, 1);

	b = createMatrix(3, 3);
	assert(3 == rows(b));
	assert(3 == cols(b));

	setEntry(b, 0, 0, 0.5);
	setEntry(b, 0, 1, 0);
	setEntry(b, 0, 2, 1.5);
	setEntry(b, 1, 0, 5);
	setEntry(b, 1, 1, -0.5);
	setEntry(b, 1, 2, -3);
	setEntry(b, 2, 0, -1);
	setEntry(b, 2, 1, -1);
	setEntry(b, 2, 2, -0.75);
	c = matrixDotMatrix(a, b);
	assert(3 == rows(c));
	assert(3 == cols(c));
	if (print_all) {
		printf("A ");
		printMatrix(a);
		printf("B ");
		printMatrix(b);
	}
	printf("A*B => C ");
	printfMatrix(c, "%5.2f ");
	y = matrixDotVector(c, x);
	printf("y ");
	printVector(y);

	assert(EQUALS(getEntry(c,0,0), -3.75));
	assert(EQUALS(getEntry(c,0,1), 1.5));
	assert(EQUALS(getEntry(c,0,2), 4.5));

	assert(EQUALS(getEntry(c,1,0), 2.5));
	assert(EQUALS(getEntry(c,1,1), -2.5));
	assert(EQUALS(getEntry(c,1,2), -6.0));

	assert(EQUALS(getEntry(c,2,0), 8.5));
	assert(EQUALS(getEntry(c,2,1), -2.0));
	assert(EQUALS(getEntry(c,2,2), -8.25));

	checkVectorEqual(z, y);
	rmMatrix(a);
	rmMatrix(b);
	rmMatrix(c);
	rmVector(x);
	rmVector(y);
	rmVector(z);

	END("3x3 matrix example passed");
}

static void testCreate() {
	/* array with different NxM matrix to test */
	unsigned short nm[][2] = { { 4, 5 }, { 256, 512 }, { 31, 69 },
			{ 1023, 2045 }, { 2024, 2034 } };
	int i, j, k = sizeof(nm) / (2 * sizeof(nm[0][0]));
	size_t b1, b2;
	unsigned short l, n, m;
	matrix a;
	vector v;
	void *x, *y, *z;
	BEG("create test");
	for (i = 0; i < k; i++) {
		n = nm[i][0];
		m = nm[i][1];
		l = m;
		a = createMatrix(n, m);
		assert(n == rows(a));
		assert(m == cols(a));
		x = PTR(a);
		printf("A    : %p\n", x);
		y = PTR(data(a));
		b1 = PTRDIFF(y, x);
		printf("a**  : %p %ld \n", y, b1);
		z = PTR(data(a)[0]);
		b1 = PTRDIFF(z, y) / sizeof(double*);
		printf("a[0] : %p %ld\n", z, b1);
		y = PTR(data(a)[0]);
		for (j = 1; j < 4; j++) {
			z = PTR(data(a)[j]);
			b2 = PTRDIFF(z,y) / sizeof(double);
			printf("a %d-%d: %p %ld \n", j, j - 1, z, b2);
			y = z;
		}
		assert(n == (unsigned short )b1);
		assert(m == (unsigned short )b2);
		rmMatrix(a);

		v = createVector(l);
		x = PTR(v);
		printf("v     : %p\n", x);
		y = PTR(&values(v)[0]);
		b1 = PTRDIFF(y, x);
		printf("v*    : %p %ld \n", y, b1);
		for (j = 1; j < 4; j++) {
			z = PTR(&values(v)[j]);
			b2 = PTRDIFF(z, y) / sizeof(double*);
			printf("v%d-%d: %p %ld\n", j, j - 1, z, b2);
			y = z;
		}
		rmVector(v);
	}
	END("create test");
}

void testPraktikumV() {
	BEG("Praktikum V tests");

	if (lets_crash_vector_dot_vector) {
		testWrongDotVector();
	}
	if (lets_crash_vector_plus_vector) {
		testWrongPlusVector();
	}
	if (lets_crash_matrix_dot_matrix) {
		testWrongDotMatrix();
	}
	if (lets_crash_matrix_plus_matrix) {
		testWrongPlusMatrix();
	}
	if (lets_crash_matrix_dot_vector) {
		testWrongMatrixDotVector();
	}

	testMatrixData();
	testVectorValues();
	test3x3Example();
	testVxVector();
	testVpVector();
	testMpMatrix();
	testMxMatrix();
	testVxMatrix();
	if (test_ctor) {
		testCreate();
	}

	END("All Praktikum V tests passed");
}

