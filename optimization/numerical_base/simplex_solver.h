#ifndef _simplex_solver_h 
#define _simplex_solver_h 

#include "vector.h"
#include "lp_task.h"
#include "basis_enumerator.h"

class SimplexSolver
{
public:

   class IntPair
   {
   public:

      IntPair( int new_i, int new_j ) : i(new_i), j(new_j)
      {
      }

      int i, j;
   };
   
   SimplexSolver( LpTask &T );

   double solve( Vector &res );

private:

   LpTask &T;
   BasisEnumerator _enum;

   void _changeBasis( Matrix &A, LpTask &T, NumberSet &Pk, NumberSet &Lk, NumberSet &Nk, bool NewBasisChangeMode );
   void _fillupMatrix( Matrix &A, LpTask &T, NumberSet &Nk, NumberSet &Nkp, NumberSet &Nk0 );
   double _findTheta( Vector &uk, Vector &x, NumberSet &Nk, int &ik );
   void _formNextBMatrix( Matrix &B, int ik, Vector &u, NumberSet &set );
   void _formFirstSupVector( Vector &v );
   double _solve( LpTask &T, Vector &res );
};

#endif /* _simplex_solver_h */