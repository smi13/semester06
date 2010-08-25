#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "system_work.h"
#include "solving_methods.h"

/* Global parameters for LP task*/
MATRIX A;
VECTOR c;
VECTOR b;

double minimum_value;
VECTOR Solution;

extern int GlobalVectorSize;

/* Load matrix & vector from file for LP task function*/
void LoadLPTask( FILE *F )
{
   int m, n, i, j;
   
   fscanf(F, "%i %i", &m, &n);

   CreateRectMatrix(&A, m, n, 1);
   CreateVector(&b, m);

   for (i = 0; i < m; i++)
      for (j = 0; j < n + 1; j++)
      {
         if (j == n)
            fscanf(F, "%lf", &b.B[i]);
         else
            fscanf(F, "%lf", &A.A[i][j]);
      }

   CreateVector(&c, n);

   for (i = 0; i < n; i++)
      fscanf(F, "%lf", &c.B[i]);
}

/* Do not display subproducts function */
int WorkSimple( char *FileName, char *OutFileName )
{
   int a = 0, a_tmp = 0;
   double min_value;
   FILE *F;
   VECTOR x;

   F = fopen(FileName, "rt");

   if (F == NULL)
      return 0;

   /* Load data */
   LoadLPTask(F);
   fclose(F);

   x = SolveLP(&A, &b, &c, &a_tmp, &min_value, &a);

   if (OutFileName == 0)
      F = stdout;
   else
      F = fopen(OutFileName, "wt");

   if (a == -1)
      fprintf(F, "No solution found.");
   else
   {
      fprintf(F, "min: %lf\nX:", min_value);
      PrintVector(&x, F);
   }

   if (OutFileName != 0)
     fclose(F);

   /* Free memory */
   ClearVector(&b);
   ClearVector(&c);
   ClearMatrix(&A);
   ClearVector(&x);

   return 1;
}

/* Display subproducts function */
int WorkIterative( char *FileName, int isdual )
{
   int a = 1;
   double min_value;
   FILE *F;
   VECTOR x;

   F = fopen(FileName, "rt");

   if (F == NULL)
      return 0;

   /* Load data */
   LoadLPTask(F);
   fclose(F);

   x = SolveLP(&A, &b, &c, &a, &min_value, &isdual);

   F = fopen("res.txt", "wt");

   if (a == -1)
      fprintf(F, "No solution found.");
   else
   {
      fprintf(F, "min: %lf\nX:", min_value);
      PrintVector(&x, F);
   }

   fclose(F);

   /* Free memory */
   ClearVector(&b);
   ClearVector(&c);
   ClearMatrix(&A);
   ClearVector(&x);

   return 1;
}

int WorkInteger( char *FileName, char *OutFileName )
{
   int a = 0;
   double min_value;
   FILE *F;
   VECTOR x;

   minimum_value = INT_MAX;

   F = fopen(FileName, "rt");

   if (F == NULL)
      return 0;

   /* Load data */
   LoadLPTask(F);
   fclose(F);

   GlobalVectorSize = A.n;

   BranchAndBound(&A, &b, &c, &min_value);

   Solution.Size = GlobalVectorSize;

   if (OutFileName == 0)
      F = stdout;
   else
      F = fopen(OutFileName, "wt");

   if (a == -1)
      fprintf(F, "No solution found.");
   else
   {
      fprintf(F, "min: %lf\nX:", minimum_value);
      PrintVector(&Solution, F);
   }

   if (OutFileName != 0)
      fclose(F);

   /* Free memory */
   ClearVector(&Solution);
   ClearVector(&b);
   ClearVector(&c);
   ClearMatrix(&A);
//   ClearVector(&x);

   return 1;
}

int main( int argc, char *argv[] )
{
   WorkSimple("tests/direct1.txt", 0);
   WorkSimple("tests/dual1.txt", 0);

   WorkSimple("tests/direct2.txt", 0);
   WorkSimple("tests/dual2.txt", 0);
   
   WorkInteger("tests/direct1.txt", 0);
   WorkInteger("tests/direct2.txt", 0);

   //WorkSimple("tests/dual2.txt", 0);

   //WorkIterative("dual1.txt", 0);
/*
   if (argc < 3)
      return EXIT_FAILURE;

   if (argc == 3)
   {
      if (strcmp(argv[1], "d") == 0)
         isdual = 1;
      else if (strcmp(argv[1], "s") == 0)
         isdual = 0;

      if (strcmp(argv[1], "i") == 0)
         if (WorkInteger(argv[2]))
            return EXIT_SUCCESS;

      if (WorkSimple(argv[2], isdual))
         return EXIT_SUCCESS;
   }

   if (argc == 4)
   {
      if (strcmp(argv[1], "d") == 0)
         isdual = 1;
      if (strcmp(argv[1], "s") == 0)
         isdual = 0;

      

      if (strcmp(argv[3], "-i") == 0)
      {
         if (WorkIterative(argv[2], isdual))
            return EXIT_SUCCESS;
      }

      return EXIT_FAILURE;
   }*/

   return EXIT_FAILURE;
}