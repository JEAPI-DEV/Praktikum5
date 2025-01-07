/*
 * DO NOT EDIT IT SHOULD COMPILE AS IT IS!
 *
 * University of Applied Sciences, Muenster, Germany
 * Lab for Computer Sciences.
 *
 *  @since:  23.11.2024
 *  @author: nwulff
 */

#ifndef VECTOR_H_
#define VECTOR_H_
/** forward declaration to an internal hidden vector structure. */
typedef struct vector_struct *vector;

/** create a (zero filled) N dimensional double vector.
 *  @param n the length of the vector 0<n
 *  @return a N-dimensional double vector reference
 */
vector createVector(unsigned short n);
/**
 * remove a vector freeing allocated memory.
 *  @param a valid vector reference
 */
void rmVector(vector v);

/** @return length N of the vector */
unsigned short size(vector v);
/** access to the internal data array.*/
double* values(vector v);
/** get the vector entry with index j
 * @param Vector vector reference
 * @param j the index number  0<=j < N
 * @return the entry
 * @print error and exits if dimensions wrong
 */
double getValue(vector Vector, unsigned short j);
/** set the vector entry with index j
 * @param v vector reference
 * @param j the index number  0<=j < N
 * @param value the entry to set
 * @print error and exits if dimensions wrong
 */
void setValue(vector v, unsigned short j, double value);
/** scalar product between to vectors.
 * @param VectorA vector reference
 * @param VectorA vector reference
 * @return scalar product a*b
 * @print error and exits if dimensions wrong
 */
double vectorDotVector(vector VectorA, vector VectorB);
/** addition of two vectors.
 * @param VectorA vector reference
 * @param VectorA vector reference
 * @return c=a+b vector
 * @print error and exits if dimensions wrong
 */
vector vectorPlusVector(vector VectorA, vector VectorB);
#endif /* VECTOR_H_ */
