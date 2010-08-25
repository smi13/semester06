#include <cmath>
#include <cfloat>

#include "number_set.h"
#include "enumerator.h"
#include "comdef.h"
#include "simplex_solver.h"
#include "codes_enumerator.h"

SimplexSolver::SimplexSolver( LpTask &T ) : T(T)
{
}

double SimplexSolver::solve( Vector &res )
{
   Vector tmp, sup_vec(T.n);
   _formFirstSupVector(tmp);

   //cout << tmp;

   //Not absolutely right: consider degenerate vector case
   for (int i = 0; i < sup_vec.size(); i++)
      sup_vec[i] = tmp[i]; 

   res = sup_vec;

   return _solve(T, res); 
}

//Artificial basis method
void SimplexSolver::_formFirstSupVector( Vector &v )
{
   Matrix A;
   Vector c(T.n + T.m),
          b(T.b),
          x(T.n + T.m),
          sup_vec(T.n + T.m);

   //New minimizing function: min(i from M) x[i]
   c.initUnit();

   for (int i = 0; i < T.n; i++)
      c[i] = 0;

   //New task matrix: A | E
   A = T.A;

   //Check for negative b components
   for (int i = 0; i < b.size(); i++)
      if (b[i] < 0 && fabs(b[i]) > eps)
      {
         b[i] = -b[i];
         A.mulRow(i, -1);
      }

   Matrix E(T.m);

   E.initUnit();
   A |= E;

   //Trivial first support vector
   for (int i = 0; i < T.n; i++)
      sup_vec[i] = 0;

   for (int i = T.n; i != sup_vec.size(); i++)
      sup_vec[i] = b[i - T.n];

   //DEBUG output
   
   LpTask TmpTask(A, b, c);

   //cout << TmpTask << endl;

   v = sup_vec;   

   _solve(TmpTask, v);

   /*cout << TmpTask << endl;
   cout << "first vector: " << sup_vec << endl;
   cout << "vector: " << v << endl;
   cout << "min: " << TmpTask.c * v << endl;*/
}

double SimplexSolver::_solve( LpTask &T, Vector &xk )
{
   //Base number sets: original set, 
   //N - original sets
   //Nkp - where x[i] is positive
   //Nk0 - where x[i] is zero
   //Nk - main set, equal to Nkp firstly
   NumberSet Nk0, Nkp, Nk, N;

   xk.positive(Nkp);
   xk.zeroed(Nk0);
   N.initOrigin(xk.size());
   Nk = Nkp;

   Matrix A = T.A(Nk);

   if (A.m() == 0)
   {
      _fillupMatrix(A, T, Nk, Nkp, Nk0);
   }

   //Check if A is rectangular - if not fill up Nkp with columns from Nk0
   if (A.m() != A.n())
      _fillupMatrix(A, T, Nk, Nkp, Nk0);

   Matrix B;

   A.buildInverse(B);

   bool BasisChangeMode = true;

   //Main method cycle
   while (true)
   {
      // Lk = N \ Nk
      NumberSet Lk = N - Nk; 

      //Calculate d[Lk]
      Vector d = T.c(Lk) - T.c(Nk) * (B * T.A(Lk));

      int jk;
      if (d.checkPositive(jk))
         break; //If d[Lk] is completely positive - end work, solution found

      d.threshold();

      //d[jk] < 0
      jk = Lk[jk];

      //Calculate uk vector
      Vector uk(xk.size());

      uk.setFor(Nk, B * T.A[jk]);
      uk.setFor(Lk - jk, 0);
      uk[jk] = -1;

      //If uk is completely negative - 
      //end work, no solution found, target function is unbounded
      if (uk.checkNegative())
         throw "Function is unbounded.";

      int ik;
      double thetha;

      //x is not degenerate
      if (Nk == Nkp)
      {
         thetha = _findTheta(uk, xk, Nk, ik);
      }
      //x is degenerate
      else
      {
         NumberSet Pk = Nk - Nkp; // Nk \ Nkp

         //Although x is degenerate, thetha can be found now
         if (uk(Pk).checkNegative())
            thetha = _findTheta(uk, xk, Nk, ik);
         //Need to change basis in that case 
         //and return to cycle beginning 
         else
         {
            _changeBasis(A, T, Pk, Lk, Nk, BasisChangeMode);
            A = T.A(Nk);
            A.buildInverse(B);
            BasisChangeMode = false;
            continue;
         }
      }

      //Build next support vector
      xk = xk - uk * thetha;

      BasisChangeMode = true;

      //cout << xk * T.c;

      //Build next A, B matrices
      A[Nk.getPlace(ik)] = T.A[jk];
      Nk.replace(ik, jk);
      A.buildInverse(B);

      //Modify Nkp & Nk0 sets
      xk.positive(Nkp);
      xk.zeroed(Nk0);
   }

   return xk * T.c;
}

void SimplexSolver::_fillupMatrix( Matrix &A, LpTask &T, NumberSet &Nk, NumberSet &Nkp, NumberSet &Nk0 )
{
   int diff = A.m() - A.n();

   if (A.m() == 0)
      diff = T.m;
   
   Enumerator col_enum(Nk0.size(), diff);
   bool isOver = false;

   vector<int> cur_perm;

   cur_perm.resize(diff);

   for (int i = 0; i != cur_perm.size(); i++)
      cur_perm[i] = i;

   while (!isOver)
   {
      NumberSet tmpset = Nk;
      Matrix tmp = A;

      for (int i = 0; i != diff; i++)
      {
         tmp << T.A[Nk0[cur_perm[i]]];
         tmpset.pushBack(Nk0[cur_perm[i]]);
      }

      if (!tmp.isDegenerate())
      {
         Nk = tmpset;
         A = tmp;
         break;
      }

      cur_perm = col_enum.next(isOver);
   }
}

void SimplexSolver::_formNextBMatrix( Matrix &B, int ik, Vector &u, NumberSet &set )
{
   int p;
   Matrix tmp(B.n(), B.m());

   for (int i = 0; i < set.size(); i++)
      if (set[i] == ik)
         p = i;

   for (int i = 0; i < tmp.m(); i++)
      for (int j = 0; j < tmp.n(); j++)
      {
         if (i == p)
            tmp[j][i] = B[j][p] / u[ik];
         else
            tmp[j][i] = B[j][i] - u[set[i]] * B[j][p] / u[ik];
      }

   B = tmp;
}

double SimplexSolver::_findTheta( Vector &uk, Vector &xk, NumberSet &Nk, int &ik )
{
   double minimum = 1 << 24;

   for (int i = 0; i < Nk.size(); i++)
   {
      if (uk[Nk[i]] > eps)
      {
         double temp = xk[Nk[i]] / uk[Nk[i]];

         if (temp < minimum)
         {
            minimum = temp;
            ik = Nk[i];
         }
      }
   }

   return minimum;
}

void SimplexSolver::_changeBasis( Matrix &A, LpTask &T, NumberSet &Pk, NumberSet &Lk, NumberSet &Nk, bool NewBasisChangeMode )
{
   if (NewBasisChangeMode)
      _enum.set(Lk, Nk, Pk);

   while (true)
   {
      NumberSet basis;

      if (!_enum.nextBasis(basis))
         throw "No appropriate basis found. Simplex method going into infinite cycle.";

      if (!T.A(basis).isDegenerate())
      {
         Nk = basis;
         return;
      }
   }
}