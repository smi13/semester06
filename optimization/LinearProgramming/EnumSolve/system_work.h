#ifndef _SYSTEM_WORK_H_
#define _SYSTEM_WORK_H_

#include <stdio.h> /* FILE */

/* Condition number size */
typedef int CONDSIZE;

#define LARGE_COND 1
#define SMALL_COND 0
/****/

/* Square matrix representation type */
typedef struct tagMATRIX
{
   double **A;
   int Size;
   int m, n;   /* Rectangle matrix case */
} MATRIX;

/* Vector representation type */
typedef struct tagVECTOR
{
   double *B;
   int Size;
} VECTOR;

/* System (linear system) representation type */
typedef struct tagSYSTEM
{
   MATRIX A;    /* System matrix */
   VECTOR B;    /* Free coefficients column */
   int Size;
} SYSTEM;

/* Copy vector function */
void CopyVector( VECTOR *V1, VECTOR *V2 );

/* Copy matrix function */
void CopyMatrix( MATRIX *M1, MATRIX *M2 );

/* Copy system function */
void CopySystem( SYSTEM *S1, SYSTEM *S2 );

/* Clear matrix function */
void ClearMatrix( MATRIX *M );

/* Clear vector function */
void ClearVector( VECTOR *V );

/* Clear system function */
void ClearSystem( SYSTEM *S );

/* Create matrix function */
void CreateMatrix( MATRIX *M, int NewSize );

/* Create rectangle matrix function */
void CreateRectMatrix( MATRIX *M, int m, int n, int force_rectangle );

/* Create vector function */
void CreateVector( VECTOR *V, int NewSize );

/* Create system function */
void CreateSystem( SYSTEM *S, int NewSize );

/* Create 'E' matrix function */
void SetInit( MATRIX *M );

/* Matrix transposing function */
void MatrixTranspose( MATRIX *M );

/* Generate system function */
void Generate( SYSTEM *S, CONDSIZE CondSize, int NewSize );

/* Matrix sum (+=) function */
void Add( MATRIX *M1, MATRIX *M2 );

/* Vector sum (+=) function */
void AddVec( VECTOR *V1, VECTOR *V2 );

/* Matrix sub (-=) function */
void Sub( MATRIX *M1, MATRIX *M2 );

/* Vector sub (-) function */
VECTOR SubVec( VECTOR *V1, VECTOR *V2 );

/* Matrix multiplication (*=) function */
void Mul( MATRIX *M1, MATRIX *M2 );

/* Print system function */
void PrintSystem( SYSTEM *S, FILE *F );

/* Print matrix function */
void PrintMatrix( MATRIX *M, FILE *F );

/* Print vector function */
void PrintVector( VECTOR *V, FILE *F );

/* Generate dA - matrix disturbance function */
void MatrixDisturbance( SYSTEM *S, MATRIX *dA );

/* Generate dB - vector disturbance function */
void VectorDisturbance( SYSTEM *S, VECTOR *dB );

/* Vector norm function */
double VectorNorm( VECTOR *V );

/* Matrix norm function */
double MatrixNorm( MATRIX *M );

/* Vectors dot product function */
double DotProductSpecial( VECTOR *V1, VECTOR *V2, int *perm, int m );

/* Vectors dot product function */
double DotProduct( VECTOR *V1, VECTOR *V2 );

void AddRow( MATRIX *A, VECTOR *b, double value, int index );

int CheckInt( VECTOR *X, int *index );

#endif /* _SYSTEM_WORK_H_ */

/* End of 'SYSTEM_WORK.H */