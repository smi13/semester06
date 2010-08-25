/* System solving functions implementation module */

#define _CRT_SECURE_NO_WARNINGS

#include <math.h> /* sqrt */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system_work.h"
#include "solving_methods.h"

#pragma warning(disable : 4244)

extern double minimum_value;
extern VECTOR Solution;

/* Helpful househoulder vector building function */
VECTOR HouseholderVector( VECTOR *x, int n, double *Betha )
{
   int i;
   double sigma = 0, m, p;
   VECTOR v;

   CreateVector(&v, N + 1);

   for (i = N - n + 2; i < N + 1; i++)
      v.B[i] = x->B[i];

   v.B[N + 1 - n] = 1;

   for (i = N - n + 2; i < N + 1; i++)
      sigma += x->B[i] * x->B[i];

   if (sigma < E)
      *Betha = 0;
   else
   {
      m = sqrt(x->B[N + 1 - n] * x->B[N + 1 - n] + sigma);
      if (x->B[N + 1 - n] < 0)
         v.B[N + 1 - n] = x->B[N + 1 - n] - m;
      else
         v.B[N + 1 - n] = -sigma / (x->B[N + 1 - n] + m);

      *Betha = 2 * v.B[N + 1 - n] * v.B[N + 1 - n] / 
         (sigma + v.B[N + 1 - n] * v.B[N + 1 - n]);

      p = v.B[N + 1 - n];

      for (i = 0; i <= N; i++)
         v.B[i] /= p;
   }

   return v;
} /* End of 'HouseholderVector' function */

/* Orthogonalization scheme solving function */
VECTOR OrtogonalizationScheme( SYSTEM *S )
{
   int i, j, n = N + 1, k, m;
   double Betha;
   VECTOR v, x;
   MATRIX NewA;
   MATRIX I, Tmp;
   MATRIX Q;

   CreateVector(&x, n);
   CreateMatrix(&NewA, n);
   CreateMatrix(&I, n);
   CreateMatrix(&Tmp, n);
   CreateMatrix(&Q, n);

   SetInit(&Q);

   for (i = 0; i < N; i++)
   {
      for (j = 0; j < N; j++) 
         NewA.A[i][j] = S->A.A[j][i];
      NewA.A[i][N] = -S->B.B[i];
      NewA.A[N][i] = 0;
   }

   NewA.A[N][N] = 1;

   for (i = 0; i < n - 1; i++)
   {
      for (j = 0; j < n; j++)
         x.B[j] = NewA.A[i][j];

      v = HouseholderVector(&x, n - i - 1, &Betha);

      SetInit(&I);

      for (k = 0; k < n; k++)
         for (m = 0; m < n; m++)
            Tmp.A[k][m] = -Betha * v.B[k] * v.B[m];

      ClearVector(&v);

      Add(&I, &Tmp);

      Mul(&NewA, &I);
      Mul(&Q, &I);
   }

   ClearMatrix(&NewA);
   ClearMatrix(&I);
   ClearMatrix(&Tmp);

   for (i = 0; i < n - 1; i++)
      x.B[i] = Q.A[i][n - 1] / Q.A[n - 1][n - 1];

   x.Size = N;

   ClearMatrix(&Q);

   return x;
} /* End of 'OrtogonalizationScheme' function */ 

/* LDR factorization scheme solving function */
VECTOR LDR( SYSTEM *S, int *det_null )
{
   int n = S->Size, i, j, k;
   VECTOR X, Y, Z;
   MATRIX L, D, R;

   FILE *F;

   *det_null = 0;

   CreateMatrix(&L, n);
   CreateMatrix(&D, n);
   CreateMatrix(&R, n);

   CreateVector(&X, n);
   CreateVector(&Y, n);
   CreateVector(&Z, n);

   /* Get LDR factorization*/
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
      {
         D.A[i][j] = 0;
         L.A[i][j] = R.A[i][j] = ((i == j) ? (1) : (0));
      }

      
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
      {
         if (i == j)
         {
            D.A[i][i] = S->A.A[i][i];
            for (k = 0; k < i; k++)
               D.A[i][i] -= L.A[i][k] * D.A[k][k] * R.A[k][i];
         }
         if (i > j)
         {
            L.A[i][j] = S->A.A[i][j];
            for (k = 0; k < j; k++)
               L.A[i][j] -= L.A[i][k] * D.A[k][k] * R.A[k][j];

/*            if (fabs(D.A[j][j]) < E)
            {
               *det_null = 1;
               return X;
            }*/

            L.A[i][j] /= D.A[j][j];
         }
         if (j > i)
         {
            R.A[i][j] = S->A.A[i][j];

            for (k = 0; k < i; k++)
               R.A[i][j] -= L.A[i][k] * D.A[k][k] * R.A[k][j];

            R.A[i][j] /= D.A[i][i];
         }
      }

   /*****/

      //DEBUG output
      F = fopen("L.txt", "wt");
      PrintMatrix(&L, F);
      fclose(F);

      F = fopen("D.txt", "wt");
      PrintMatrix(&D, F);
      fclose(F);

      F = fopen("R.txt", "wt");
      PrintMatrix(&R, F);
      fclose(F);

   /* Lz = b*/
   for (i = 0; i < n; i++)
   {
      Z.B[i] = S->B.B[i];

      for (k = 0; k < i; k++)
         Z.B[i] -= L.A[i][k] * Z.B[k];

      Z.B[i] /= L.A[i][i];
   }

   /* Dy = z */
   for (i = 0; i < n; i++)
   {
      if (fabs(D.A[i][i]) < E)
      {
         *det_null = 1;
         return X;
      }
      Y.B[i] = Z.B[i] / D.A[i][i];
   }

   /* Rx = y */
   for (i = n - 1; i >= 0; i--)
   {
      X.B[i] = Y.B[i];

      for (k = i + 1; k < n; k++)
         X.B[i] -= R.A[i][k] * X.B[k];

      X.B[i] /= R.A[i][i];
   }

   ClearMatrix(&L);
   ClearMatrix(&D);
   ClearMatrix(&R);

   ClearVector(&Y);
   ClearVector(&Z);

   return X;
} /* End of 'LDR' function */

//==============================================================================
// return 1 if system not solving
// nDim - system dimension
// pfMatr - matrix with coefficients
// pfVect - vector with free members
// pfSolution - vector with system solution
// pfMatr becames trianglular after function call
// pfVect changes after function call
//==============================================================================
int LinearEquationsSolving( int nDim, double *pfMatr, double *pfVect, double *pfSolution )
{
   int i, j, k, m;

   double fMaxElem;
   double fAcc;

   for (k = 0; k < nDim - 1; k++) 
   {
      fMaxElem = fabs(pfMatr[k * nDim + k]);
      m = k;
      for (i = k + 1; i < nDim; i++)
      {
         if (fMaxElem < fabs(pfMatr[i * nDim + k]))
         {
            fMaxElem = fabs(pfMatr[i * nDim + k]);
            m = i;
         }
      }

      if (m != k)
      {
         for (i = k; i < nDim; i++)
         {
            fAcc = pfMatr[k * nDim + i];
            pfMatr[k * nDim + i] = pfMatr[m * nDim + i];
            pfMatr[m * nDim + i] = fAcc;
         }

         fAcc = pfVect[k];
         pfVect[k] = pfVect[m];
         pfVect[m] = fAcc;
      }

      if (fabs(pfMatr[k * nDim + k]) < 1e-10) 
         return 1; 

      for (j = k + 1; j < nDim; j++) 
      {
         fAcc = -pfMatr[j * nDim + k] / pfMatr[k * nDim + k];
         
         for (i = k; i < nDim; i++)
         {
            pfMatr[j * nDim + i] = pfMatr[j * nDim + i] + fAcc * pfMatr[k * nDim + i];
         }

         pfVect[j] = pfVect[j] + fAcc * pfVect[k]; 
      }
   }

   for (k = nDim - 1; k >= 0; k--)
   {
      pfSolution[k] = pfVect[k];

      for (i = k + 1; i < nDim; i++)
      {
         pfSolution[k] -= (pfMatr[k * nDim + i] * pfSolution[i]);
      }

      if (fabs(pfMatr[k * nDim + k]) < 1e-10) 
         return 1;

      pfSolution[k] = pfSolution[k] / pfMatr[k * nDim + k];
   }

   return 0;
}

VECTOR Gauss( SYSTEM *S, int *det_null )
{
   int i, j, n = S->Size;
   double *matr, *vect, *solution;
   VECTOR X;

   solution = malloc(sizeof(double) * n);
   matr = malloc(sizeof(double) * n * n);
   vect = malloc(sizeof(double) * n);

   for (i = 0; i < n; i++)
      vect[i] = S->B.B[i];

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         matr[i * n + j] = S->A.A[i][j];

   if (LinearEquationsSolving(n, matr, vect, solution))
      *det_null = 1;
   else
      *det_null = 0;

   CreateVector(&X, n);

   for (i = 0; i < n; i++)
   {
      X.B[i] = solution[i];
   }

   free(solution);
   free(matr);
   free(vect);

   return X;
}

int n_1, m_1, p;
int *perm;

/* Generate next */
int nextSubset( void )
{
   if (m_1 == n_1)
      return 0;

   if (p < 1)
      return 0;

   if (perm[n_1 - 1] == m_1 - 1)
      p--;
   else
      p = n_1;

   if (p >= 1)
   { 
      int i;

      for (i = n_1 - 1; i >= p - 1; i--)
         perm[i] = perm[p - 1] + i - (p - 1) + 1;
   }
   else
      return 0;

   return 1;
}

/* Simple LP task solving function*/
VECTOR SolveLP( MATRIX *A, VECTOR *b, VECTOR *c, int *iter, double *min_value, int *solution_found )
{
   int count_iter = 0;
   int *main_perm;
   double cur_f = INT_MAX;
   int i, j;
   VECTOR x;
   FILE *F;

   *solution_found = 0;

   if (*iter == 1)
   {
      F = fopen("subproducts.txt", "wt");
      PrintMatrix(A, F);
   }

   n_1 = A->m;
   m_1 = A->n;

   CreateVector(&x, A->n);

   p = n_1;

   perm = malloc(sizeof(int) * m_1);
   main_perm = malloc(sizeof(int) * A->m);

   for (i = 0; i < m_1; i++)
      perm[i] = i;

   do
   {
      int flag = 0;
      int det_null;
      double f;
      SYSTEM cur_sys;
      VECTOR cur_x;

      //DEBUG output
      /*for (i = 0; i < A->m; i++)
         printf("%i ", perm[i]);
      printf("\n");*/
      count_iter++;

      CreateSystem(&cur_sys, A->m);

      for (i = 0; i < A->m; i++)
         for (j = 0; j < A->m; j++)
         {
            cur_sys.A.A[i][j] = A->A[i][perm[j]];
         }

      if (*iter == 1)
      {
         fprintf(F, "%i)\n", count_iter);
         PrintMatrix(&cur_sys.A, F);
      }

      for (i = 0; i < A->m; i++)
         cur_sys.B.B[i] = b->B[i];

      cur_x = Gauss(&cur_sys, &det_null);

      if (det_null)
      {
         ClearSystem(&cur_sys);
         continue;
      }

      if (*iter == 1)
      {
         fprintf(F, "X: ");
         PrintVector(&cur_x, F);
      }
   
      for (i = 0; i < A->m; i++)
      {
         if (fabs(cur_x.B[i]) > 1e-10)
            if (cur_x.B[i] < 0)
            {
               ClearSystem(&cur_sys);
            
               flag = 1;
               break;
            }
      }

      if (flag)
         continue;
      if (!flag)
         *solution_found = 1;

      f = DotProductSpecial(&cur_x, c, perm, A->m);

      if (f < cur_f)
      {
         cur_f = f;
         CopyVector(&x, &cur_x);
         memcpy(main_perm, perm, sizeof(int) * A->m);
      }

      ClearSystem(&cur_sys);
   } while (nextSubset());

   if (*iter == 1)
      fclose(F);

   if (!*solution_found)
      *iter = -1;
   else
   {
      VECTOR cur_x;

      *min_value = cur_f;
      
      CreateVector(&cur_x, A->m);
      CopyVector(&cur_x, &x);
      ClearVector(&x);
      CreateVector(&x, A->n);

      for (i = 0; i < x.Size; i++)
      {
         int flag = 0;

         for (j = 0; j < A->m; j++)
            if (main_perm[j] == i)
            {
               x.B[i] = cur_x.B[j];
               flag = 1;
               break;
            }

         if (flag)
            continue;
         else
            x.B[i] = 0;
      }
   }
   
   free(main_perm);
   free(perm);

   return x;
} /* End of 'SolveLP' function */

extern int GlobalVectorSize;

void TryUpdate( VECTOR *X, VECTOR *c )
{
   double cur_value = DotProduct(X, c);

   if (cur_value < minimum_value)
   {
      if (Solution.Size != 0)
         ClearVector(&Solution);

      CreateVector(&Solution, X->Size);
      CopyVector(&Solution, X);
      minimum_value = cur_value;
   }
}

void AddCondition( MATRIX *A, VECTOR *b, VECTOR *c, int index, double value, int side )
{
   int i, j;
   VECTOR b_tmp, c_tmp;
   MATRIX tmp;

   CreateRectMatrix(&tmp, A->m, A->n, 1);
   CreateVector(&b_tmp, b->Size);
   CreateVector(&c_tmp, c->Size);

   CopyMatrix(&tmp, A);
   CopyVector(&b_tmp, b);
   CopyVector(&c_tmp, c);

   ClearMatrix(A);
   ClearVector(b);
   ClearVector(c);

   CreateVector(b, b_tmp.Size + 1);
   CreateVector(c, c_tmp.Size + 1);
   CreateRectMatrix(A, tmp.m + 1, tmp.n + 1, 1);

   for (i = 0; i < tmp.m; i++)
      for (j = 0; j < tmp.n; j++)
         A->A[i][j] = tmp.A[i][j];

   A->A[tmp.m][index] = 1.0;
   A->A[tmp.m][tmp.n] = side;

   for (i = 0; i < b_tmp.Size; i++)
      b->B[i] = b_tmp.B[i];

   b->B[b_tmp.Size] = value;

   for (i = 0; i < c_tmp.Size; i++)
      c->B[i] = c_tmp.B[i];

   c->B[c_tmp.Size] = 0;

   /*{
      FILE *F = stdout;
      PrintVector(c, F);
      PrintVector(b, F);
      PrintMatrix(A, F);
   }*/

   ClearVector(&c_tmp);
   ClearVector(&b_tmp);
   ClearMatrix(&tmp);
}

void BranchAndBound( MATRIX *A, VECTOR *b, VECTOR *c, double *min_value )
{
   int a = 1, index, solution_found;
   VECTOR X;

   X = SolveLP(A, b, c, &a, min_value, &solution_found);

   {
      FILE *F = stdout;

      //PrintVector(&X, F);
   }

   if (solution_found)
   {
      if (!CheckInt(&X,  &index))
      {
         double min_value1, min_value2;
         MATRIX A1, A2;
         VECTOR b1, b2;
         VECTOR c1, c2;

         CreateVector(&c1, c->Size);
         CreateVector(&c2, c->Size);
         CreateVector(&b1, b->Size);
         CreateVector(&b2, b->Size);
         CreateRectMatrix(&A1, A->m, A->n, 1);
         CreateRectMatrix(&A2, A->m, A->n, 1);

         CopyVector(&c1, c);
         CopyVector(&c2, c);
         CopyVector(&b1, b);
         CopyVector(&b2, b);
         CopyMatrix(&A1, A);
         CopyMatrix(&A2, A);

         AddCondition(&A1, &b1, &c1, index, floor(X.B[index]), 1.0);
         BranchAndBound(&A1, &b1, &c1, &min_value1);

         AddCondition(&A2, &b2, &c2, index, ceil(X.B[index]), -1.0);
         BranchAndBound(&A2, &b2, &c2, &min_value2);

         ClearVector(&b1);
         ClearVector(&b2);

         ClearMatrix(&A1);
         ClearMatrix(&A2);
      }
      else
         TryUpdate(&X, c);
   }
}

/* END OF 'SOLVING_METHODS.C' FILE */