#include <iostream>
#include <cmath>

using namespace std;

#include "comdef.h"
#include "steepest_descent_solver.h"
#include "md_task.h"
#include "od_min_finder.h"

SteepestDescentSolver::SteepestDescentSolver( MdTask &T, Vector &x0, bool feasibilityCheck ) : 
            MdTaskSolver(T), _x0(x0), _feasibilityCheck(feasibilityCheck)
{
}

double SteepestDescentSolver::solve( Vector &res )
{
   double grad_norm2;
   Vector xk = _x0, grad;

   grad = _T._phi0_grad->eval(xk);
   grad_norm2 = grad.norm2();

   while (grad_norm2 > _eps)
   {
      double alpha;
      Vector pk = grad;

      AdditionalFunc helpful_func(xk, pk, *_T._phi0);
      OdMinFinder od_finder(helpful_func, 0, 1.0);

      od_finder.setEps(1e-11);
      od_finder.find(alpha, OdMinFinder::GOLDEN_RATIO);

      Vector x_next;

      x_next = xk - pk * alpha;

      if (_feasibilityCheck)
      {
         if (!_isFeasible(x_next))
         {
            alpha *= 0.1;
            Vector x_cur = xk - pk * alpha;

            while (!_isFeasible(x_cur))
            {
               alpha *= 0.1;
               x_cur = xk - pk * alpha;
            }

            x_next = x_cur;
         }
      }

      xk = x_next;

      grad = _T._phi0_grad->eval(xk);
      grad_norm2 = grad.norm2();
      MdTaskSolver::_iterCounter++;
   }

   res = xk;

   return _T._phi0->eval(xk);
}

bool SteepestDescentSolver::_isFeasible( Vector &x )
{
   for (int i = 0; i != _T._phi.size(); i++)
   {
      double func_value = _T._phi[i]->eval(x);

      if (func_value > 0 )
      {
         return false;
      }
   }

   return true;
}

double AdditionalFunc::operator()( double &alpha )
{
   Vector v;

   v = _u - _p * alpha;

   return _func(v);
}

AdditionalFunc::AdditionalFunc( Vector &u, Vector &p, Func &func ) : _u(u), _p(p), _func(func)
{
}