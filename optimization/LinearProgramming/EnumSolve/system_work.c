#include <stdio.h>  /* printf */ 
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <math.h>   /* sin, cos, rand */

#include "system_work.h"

#define eps 1e-10

/* Copy vector function */
void CopyVector( VECTOR *V1, VECTOR *V2 )
{
   int i, n = V1->Size;

   for (i = 0; i < n; i++)
      V1->B[i] = V2->B[i];

   V1->Size = V2->Size;

} /* End of 'CopyVector' function */

/* Copy matrix function */
void CopyMatrix( MATRIX *M1, MATRIX *M2 )
{
   int i, j, n = M1->Size;

   if (n == -1)
   {
      for (i = 0; i < M1->m; i++)
         for (j = 0; j < M1->n; j++)
            M1->A[i][j] = M2->A[i][j];

      M1->m = M2->m;
      M1->n = M2->n;

      return;
   }

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         M1->A[i][j] = M2->A[i][j];

   M1->Size = M2->Size;

} /* End of 'CopyMatrix' function */

/* Copy system function */
void CopySystem( SYSTEM *S1, SYSTEM *S2 )
{
   CopyMatrix(&S1->A, &S2->A);
   CopyVector(&S1->B, &S2->B);

   S1->Size = S2->Size;
} /* End of 'CopySystem' function */

/* Clear matrix function */
void ClearMatrix( MATRIX *M )
{
   int i, n = M->Size;

   if (n == -1)
   {
      for (i = 0; i < M->m; i++)
         if (M->A[i] != NULL)
            free(M->A[i]);
   }
   else
   {
      for (i = 0; i < n; i++)
         if (M->A[i] != NULL)
            free(M->A[i]);
   }

   if (M->A != NULL)
      free(M->A);
} /* End of 'ClearMatix' function */

/* Clear vector function */
void ClearVector( VECTOR *V )
{
   if (V->B != NULL)
      free(V->B);
} /* End of 'ClearVector' function */

/* Clear system function */
void ClearSystem( SYSTEM *S )
{
   ClearMatrix(&S->A);

   if (S->B.B != NULL)
      free(S->B.B);
} /* End of 'ClearSystem' function */

/* Create matrix function */
void CreateMatrix( MATRIX *M, int NewSize )
{
   int i, j, n = NewSize;

   M->Size = NewSize;

   M->m = NewSize;
   M->n = NewSize;

   M->A = malloc(sizeof(double *) * n);

   for (i = 0; i < n; i++)
      M->A[i] = malloc(sizeof(double) * n);

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         M->A[i][j] = 0;

} /* End of 'CreateMatrix' function */

/* Create rectangle matrix function */
void CreateRectMatrix( MATRIX *M, int m, int n, int force_rectangle )
{
   int i, j;

   if (m == n && !force_rectangle)
   {
      CreateMatrix(M, n);
      return;
   }

   M->Size = -1; /* Rectangle identifying */

   M->m = m;
   M->n = n;

   M->A = malloc(sizeof(double *) * m);

   for (i = 0; i < m; i++)
      M->A[i] = malloc(sizeof(double) * n);

   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         M->A[i][j] = 0;
} /* End of 'CreateRectMatrix' function */ 

/* Create vector function */
void CreateVector( VECTOR *V, int NewSize )
{
   int i;

   V->Size = NewSize;

   V->B = malloc(sizeof(double) * NewSize);

   assert(V->B != NULL);

   for (i = 0; i < NewSize; i++)
      V->B[i] = 0;

} /* End of 'CreateVector' function */

/* Create system function */
void CreateSystem( SYSTEM *S, int NewSize )
{
   S->Size = NewSize;
   CreateMatrix(&S->A, NewSize);
   CreateVector(&S->B, NewSize);
} /* End of 'CreateSystem' function */

/* Create 'E' matrix function */
void SetInit( MATRIX *M )
{
   int i, j, n = M->Size; 

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
      {
         if (i != j)
            M->A[i][j] = 0;
         else
            M->A[i][j] = 1;
      }

} /* End of 'CreateInit' function */

/* Generate system function */
void Generate( SYSTEM *S, CONDSIZE CondSize, int NewSize )
{
   int i, j, count = 2 * NewSize, RandI, RandJ, n = NewSize;
   double Alpha;
   MATRIX A, Rot, Tmp;
   VECTOR B;

   CreateMatrix(&A, NewSize);
   CreateMatrix(&Tmp, NewSize);
   CreateVector(&B, NewSize);

   A.A[0][0] = 1;
   CondSize == SMALL_COND ? (A.A[n - 1][n - 1] = 50 + rand() % (100 - 50)) :
   (A.A[n - 1][n - 1] = 100000 + rand() % (1000000 - 100000));

   for (i = 1; i < n - 1; i++)
      A.A[i][i] = 1 + rand() % (int)(A.A[n - 1][n - 1] - 1);

   CreateMatrix(&Rot, NewSize);
   SetInit(&Rot);

   while (count-- > 0)
   {
      RandI = rand() % n;
      RandJ = rand() % n;
      Alpha = (double)rand() / RAND_MAX;

      Rot.A[RandI][RandI] = cos(Alpha);
      Rot.A[RandJ][RandJ] = Rot.A[RandI][RandI];

      Rot.A[RandI][RandJ] = -sin(Alpha);
      Rot.A[RandJ][RandI] = -Rot.A[RandI][RandJ];

      Mul(&A, &Rot);

      Rot.A[RandJ][RandI] *= -1;
      Rot.A[RandI][RandJ] *= -1;

      Mul(&Rot, &A);

      for (i = 0; i < n; i++)
         for (j = 0; j < n; j++)
            A.A[i][j] = Rot.A[i][j];

      SetInit(&Rot);
   }

   for (i = 0; i < n; i++)
      B.B[i] = (double) rand() / RAND_MAX * n + rand() % n;

   ClearMatrix(&Tmp);
   ClearMatrix(&Rot);

   S->A = A;
   S->B = B;
   S->Size = A.Size;

} /* End of 'Generate' function */

/* Matrix sum (+=) function */
void Add( MATRIX *M1, MATRIX *M2 )
{
   int i, j, n = M1->Size;

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         M1->A[i][j] += M2->A[i][j];
} /* End of 'Add' function */

/* Vector sum (+=) function */
void AddVec( VECTOR *V1, VECTOR *V2 )
{
   int i, n = V1->Size;

   for (i = 0; i < n; i++)
      V1->B[i] += V2->B[i];

} /* End of 'AddVec' function */

/* Matrix sub (-=) function */
void Sub( MATRIX *M1, MATRIX *M2 )
{
   int i, j, n = M1->Size;

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         M1->A[i][j] -= M2->A[i][j];
} /* End of 'Sub' function */

/* Vector sub (-) function */
VECTOR SubVec( VECTOR *V1, VECTOR *V2 )
{
   int i, n = V1->Size;
   VECTOR X;

   CreateVector(&X, V1->Size);

   for (i = 0; i < n; i++)
      X.B[i] = V1->B[i] - V2->B[i];

   return X;
} /* End of 'SubVec' function */

/* Matrix multiplication (*=) function */
void Mul( MATRIX *M1, MATRIX *M2 )
{
   int i, j, k, n = M1->Size;
   double sum;
   MATRIX Res;

   CreateMatrix(&Res, n);

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
      {
         sum = 0;
         for (k = 0; k < n; k++)
            sum += M1->A[i][k] * M2->A[k][j];
         Res.A[i][j] = sum;
      }

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         M1->A[i][j] = Res.A[i][j];
               
   ClearMatrix(&Res);
} /* End of 'Mul' function */

/* Print system function */
void PrintSystem( SYSTEM *S, FILE *F )
{
   int i, j, n = S->Size;

   for (i = 0; i < n; i++)
   {
      for (j = 0; j < n; j++)
         fprintf(F, "%12.5lf ", S->A.A[i][j]);
      fprintf(F, " | %lf\n", S->B.B[i]);
   }

   fprintf(F, "\n\n");
} /* End of 'PrintSystem' function */

/* Print matrix function */
void PrintMatrix( MATRIX *M, FILE *F )
{
   int i, j, n = M->Size, m;

   if (n == -1)
   {
      n = M->n;
      m = M->m;
   }
   else
   {
      n = M->Size;
      m = M->Size;
   }

   for (i = 0; i < m; i++)
   {
      for (j = 0; j < n; j++)
         fprintf(F, "%12.5lf ", M->A[i][j]);//fprintf(F, "%12.5lf ", M->A[i][j]);
      fprintf(F, "\n");
   }

   fprintf(F, "\n\n");
} /* End of 'PrintSystem' function */

/* Print vector function */
void PrintVector( VECTOR *V, FILE *F )
{
   int i, n = V->Size;

   for (i = 0; i < n; i++)
      fprintf(F, "%7.5lf ", V->B[i]);

   fprintf(F, "\n\n");
} /* End of 'PrintVector' function */

/* Generate dA - matrix disturbance function */
void MatrixDisturbance( SYSTEM *S, MATRIX *dA )
{
   int n = S->Size, i, j;
   MATRIX K;

   CreateMatrix(&K, n);

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
      {
         K.A[i][j] = ((double)rand() / RAND_MAX * 2 - 1) * 0.01;
         K.A[j][i] = K.A[i][j];
      }

   *dA = K;

   Add(&S->A, &K);

} /* End of 'MatrixDisturbance' function */

/* Helpful element swapping function */
void Swap( double *a, double *b )
{
   double c = *a;

   *a = *b;
   *b = c;
} /* End of 'Swap' function */

/* Matrix transposing function */
void MatrixTranspose( MATRIX *M )
{
   int i, j, n = M->Size;

   for (i = 0; i < n; i++)
      for (j = i; j < n; j++)
         Swap(&M->A[i][j], &M->A[j][i]);

} /* End of 'MatrixTranspose' function */

/* Generate dB - vector disturbance function */
void VectorDisturbance( SYSTEM *S, VECTOR *dB )
{
   int n = S->Size, i;
   VECTOR K;

   CreateVector(&K, n);

   for (i = 0; i < n; i++)
      K.B[i] = ((double)rand() / RAND_MAX * 2 - 1) * 0.01;

   *dB = K;

   for (i = 0; i < n; i++)
      S->B.B[i] += K.B[i];
} /* End of */

/* Vector norm function */
double VectorNorm( VECTOR *V )
{
   int i, n = V->Size;
   double res = 0;

   for (i = 0; i < n; i++)
      res += V->B[i] * V->B[i];

   res = sqrt(res);

   return res;
} /* End of 'VectorNorm' function */

/* Matrix norm function */
double MatrixNorm( MATRIX *M )
{
   int i, j, n = M->Size;
   double res = 0;
   
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         res += M->A[i][j] * M->A[i][j];

   res = sqrt(res);

   return res;
} /* End of 'MatrixNorm' function */

/* Vectors dot product special function */
double DotProductSpecial( VECTOR *V1, VECTOR *V2, int *perm, int m )
{
   int i, n = V1->Size;
   double sum = 0;

   for (i = 0; i < m; i++)
      sum += V1->B[i] * V2->B[perm[i]];

   return sum;
} /* End of 'DotProductSpecial' function */

/* Vectors dot product function */
double DotProduct( VECTOR *V1, VECTOR *V2 )
{
   int i, n = V1->Size;
   double sum = 0;

   for (i = 0; i < n; i++)
      sum += V1->B[i] * V2->B[i];

   return sum;
} /* End of 'DotProduct' function */


void AddRow( MATRIX *A, VECTOR *b, double Value, int index )
{
   int i, j;
   MATRIX A1;
   VECTOR b1;

   CreateRectMatrix(&A1, A->m, A->n, 1);
   CreateVector(&b1, b->Size);

   CopyVector(&b1, b);

   for (j = 0; j < A->n; j++)
      for (i = 0; i < A->m; i++)
      {
         A1.A[i][j] = A->A[i][j];
      }

   ClearMatrix(A);
   ClearVector(b);

   CreateRectMatrix(A, A1.m + 1, A1.n, 1);
   CreateVector(b, b1.Size + 1);

   for (i = 0; i < b1.Size; i++)
      b->B[i] = b1.B[i];

   b->B[b->Size - 1] = Value;

   for (j = 0; j < A->n; j++)
      for (i = 0; i < A->m - 1; i++)
      {
         A->A[i][j] = A1.A[i][j];
      }

   for (i = 0; i < A->n; i++)
   {
     if (i == index)
     {
        A->A[A->m - 1][i] = 1;
        continue;
     }
     A->A[A->m - 1][i] = 0;
   }

   ClearMatrix(&A1);
   ClearVector(&b1);
}

int GlobalVectorSize;

int CheckInt( VECTOR *X, int *index )
{
   int i;
   double diff;

   for (i = 0; i < GlobalVectorSize; i++)
   {
      if (fabs(X->B[i]) > eps)
      {
         diff = X->B[i] - floor(X->B[i]); 

         if (diff > eps)
         {
            *index = i;
            return 0;
         }
      }
   }

   return 1;
}

/* END OF 'SYSTEM_WORK.C' FILE */