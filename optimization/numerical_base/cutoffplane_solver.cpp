
#include <cmath>
#include <cstdio>

#include "simplex_solver.h"
#include "cutoffplane_solver.h"
#include "md_task.h"
#include "lp_task.h"

CutOffPlaneSolver::CutOffPlaneSolver( MdTask &T, LpTask &S0 )  : MdTaskSolver(T), _Sk(S0)
{
   _helpfulFunc.setFunc(T._phi0);
   _helpfulFuncGrad.setFunc(T._phi0_grad);
}

double CutOffPlaneSolver::solve( Vector &res )
{
   Vector xk, x_next;

   xk = _nextVector();

   while (true)
   {
      if (_belongsToOmega(xk))
         break;

      _buildSubgradient(xk);

      x_next = _nextVector();

      if ((x_next - xk).norm2() < _eps * _eps)
         break;

      _iterCounter++;

      xk = x_next;
   }

   res = xk;
   res.popBack();

   return _T._phi0->eval(xk);
}

bool CutOffPlaneSolver::_isOver()
{
   return true;
}

Vector CutOffPlaneSolver::_nextVector()
{
   cout << _Sk << endl;
   SimplexSolver lp_solver(_Sk);
   Vector tmp, res(_T._taskDimension + 1);

   lp_solver.solve(tmp);

   for (int i = 0; i != res.size(); i ++)
      res[i] = tmp[i] - tmp[res.size() + i];

   return res;
}

bool CutOffPlaneSolver::_belongsToOmega( Vector &x )
{
   for (int i = 0; i != _T._phi.size(); i++)
   {
      if (_T._phi[i]->eval(x) > 0)
      {
         return false;
      }
   }

   if (_helpfulFunc(x) > 0)
      return false;

   return true;
}

void CutOffPlaneSolver::_buildSubgradient( Vector &x )
{
   NumberSet I;

   _findMax(x, I);

   Vector subgradient;

   int i_max = I[0];

   if (i_max == -1)
   {
      subgradient = _helpfulFuncGrad(x);
   }
   else
   {
      subgradient = _T._phi_grad[i_max]->eval(x);
   }

   double b;
   
   if (i_max == -1)
      b = -_helpfulFunc(x) + subgradient * x;
   else
      b = -_T._phi[i_max]->eval(x) + subgradient * x;

   _Sk.b.pushBack(b);

   Vector new_vec(_Sk.m + 1);

   new_vec[new_vec.size() - 1] = 1.0;

   _Sk.A << new_vec;

   for (int i = 0; i != _Sk.n; i++)
   {
      double b = 0;
      _Sk.A[i].pushBack(b);
   }

   for (int i = 0; i != subgradient.size(); i++)
   {
      _Sk.A[i][_Sk.A.m() - 1] = subgradient[i];
      _Sk.A[i + subgradient.size()][_Sk.A.m() - 1] = -subgradient[i];
   }

   b = 0;
   _Sk.c.pushBack(b);

   _Sk.n = _Sk.A.n();
   _Sk.m = _Sk.A.m();

//   cout << _Sk;
}

void CutOffPlaneSolver::_findMax( Vector &x, NumberSet &I )
{
   Vector tmp(x.size() - 1);

   for (int i = 0; i != tmp.size(); i++)
      tmp[i] = x[i];

   int i_max = -1;
   double phi_max = _helpfulFunc(x);

   for (int i = 0; i != _T._phi.size(); i++)
   {
      double tmp_val = _T._phi[i]->eval(tmp);

      if (tmp_val > phi_max)
      {
         phi_max = tmp_val;
         i_max = i;
      }
   }

   double tmp_val = _helpfulFunc(x);

   if (fabs(tmp_val - phi_max) < 1e-13)
   {
      I.pushBack(-1);
   }

   for (int i = 0; i != _T._phi.size(); i++)
   {
      double tmp_val = _T._phi[i]->eval(tmp);

      if (fabs(tmp_val - phi_max) < 1e-13)
      {
         I.pushBack(i);
      }
   }
}