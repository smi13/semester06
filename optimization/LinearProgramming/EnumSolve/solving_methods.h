/* System solving functions declaration module */

#ifndef _SOLVING_METHODS_H_
#define _SOLVING_METHODS_H_

/* Global linear systems size */
#define N 3

#define E 0.00001

#include "system_work.h"

/* Orthogonalization scheme solving function */
VECTOR OrtogonalizationScheme( SYSTEM *S );

/* LDR factorization scheme solving function */
VECTOR LDR( SYSTEM *S, int *det_null );

/* Simple LP task solving function */
VECTOR SolveLP( MATRIX *A, VECTOR *b, VECTOR *c, int *iter, double *min_value, int *solution_found );

/* Simple IP task solving function */
void BranchAndBound( MATRIX *A, VECTOR *b, VECTOR *c, double *min_value );

#endif /* _SOLVING_METHODS_H_ */

/* END OF 'SOLVING_METHODS.H' FILE */