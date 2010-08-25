#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

#include "function.h"
#include "lp_task.h"
#include "zoutendijk_solver.h"
#include "vector.h"
#include "simplex_solver.h"

extern int taskDimension;

ZoutendijkSolver::ZoutendijkSolver( MdTask &T ) : MdTaskSolver(T)
{
   _solvingHelpTask = false;
   _delta = 0.1;
   _alpha = 1.0;
   _lambda = 0.1;
   taskDimension = T._taskDimension;
}

double ZoutendijkSolver::solve( Vector &res )
{
   Vector x0;

   findFirstPoint(x0);

   return _solve(_T, res, x0);
}

double ZoutendijkSolver::_solve( MdTask &T, Vector &res, Vector &xk )
{
   while (true)
   {
      double etha;
      Vector s;

      if (_solvingHelpTask)
         if (xk[xk.size() - 1] < 0)
            break;

  /*    for (int i = 0; i < T._phi.size(); i++)
         cout << T._phi[i]->eval(xk) << " ";
      cout << endl;*/

      _iterCounter++;

      s = _nextDirection(T, xk, etha);

      /*if (!_solvingHelpTask)
      {
         cout << _iterCounter << endl <<
            "X: " << xk << endl <<
            "delta: " << _delta << endl <<
            "etha: " << etha << endl <<
            "phi0(xk): " << T._phi0->eval(xk) << endl;
      }*/

//      cout << etha << endl;

      xk = _nextStep(T, xk, etha, s);

      if (_isOver(T, xk, etha))
         break;
   }

   res = xk;

   return T._phi0->eval(res);
}

void ZoutendijkSolver::_getIdelta( MdTask &T, vector<int> &i_delta, Vector &cur )
{
   for (int i = 0; i != T._phi.size(); i++)
   {
      double tmp = T._phi[i]->eval(cur);

      if (tmp >= -_delta && tmp <= 0)
         i_delta.push_back(i);
   }
}

void ZoutendijkSolver::_prepareLPTask( MdTask &T, LpTask &lp_task, Vector &cur, vector<int> &i_delta )
{
   int n = cur.size(), tmp = 1 + i_delta.size();
   
   Matrix A(n + 1, 1 + i_delta.size() + 2 * n);
   
   for (int i = 0; i < A[0].size(); i++)
   {
      if (i < tmp)
         A[0][i] = 1.0;
      else
         A[0][i] = 0.0;
   }

   for (int i = 0; i < tmp; i++)
   {
      Vector tmp_grad;

      if (i == 0)
         tmp_grad = T._phi0_grad->eval(cur);
      else
         tmp_grad = T._phi_grad[i_delta[i - 1]]->eval(cur);  

      for (int j = 1; j < A.n(); j++)
         A[j][i] = -tmp_grad[j - 1];
   }

   for (int j = 1; j < A.n(); j++)
      for (int i = tmp; i < tmp + n; i++)
      {
         if (i - tmp + 1 == j)
            A[j][i] = 1.0;
         else
            A[j][i] = 0.0;
      }

   for (int j = 1; j < A.n(); j++)
      for (int i = tmp + n; i < tmp + 2 * n; i++)
      {
         if (i - tmp - n + 1 == j)
            A[j][i] = -1.0;
         else
            A[j][i] = 0.0;
      }

   Vector c(A.n() + A.n() + A.m());

   c.initZero();

   c[0] = 1.0;
   c[A.n()] = -1.0;

   A |= A * (-1);

   Matrix E(A.m());

   E.initUnit();
   E *= -1;

   A |= E;

   Vector b(A.m());

   b.initValue(-1.0);

   for (int i = 0; i < tmp; i++)
      b[i] = 0;

   lp_task.m = A.m();
   lp_task.n = A.n();
   lp_task.A = A;
   lp_task.b = b;
   lp_task.c = c;
}

Vector ZoutendijkSolver::_nextDirection( MdTask &T, Vector &xk, double &out_etha )
{
   int n = xk.size();
   vector<int> i_delta;

   LpTask lp_Task;
   _getIdelta(T, i_delta, xk);
   _prepareLPTask(T, lp_Task, xk, i_delta);

   SimplexSolver lp_solver(lp_Task);

   Vector tmp, s(n);

   lp_solver.solve(tmp);

   out_etha = tmp[0] - tmp[n + 1];

   for (int i = 1; i < n + 1; i++)
   {
      s[i - 1] = tmp[i] - tmp[i + n + 1];
   }

   return s;
}

Vector ZoutendijkSolver::_nextStep( MdTask &T, Vector &xk, double etha, Vector &s )
{
   if (etha < -_delta)
   {
      bool feasible = true;
      double cur_alpha = _alpha;
      Vector x_next = xk + s * cur_alpha;

      while (true)
      {
         feasible = true;

         if (T._phi0->eval(x_next) > T._phi0->eval(xk) + 0.5 * etha * cur_alpha)
            feasible = false;

         for (int i = 0; i != T._phi.size(); i++)
            if (T._phi[i]->eval(x_next) > 0)
            {
               feasible = false;
               break;
            }

         if (feasible)
            break;

         cur_alpha *= _lambda;
         x_next = xk + s * cur_alpha;
      }

      return x_next;
   }
   else
   {
      _delta *= _lambda;
      return xk;
   }
}

bool ZoutendijkSolver::_isOver( MdTask &T, Vector &xk, double etha )
{
   double delta0k = -(1 << 24);

   for (int i = 0; i != T._phi.size(); i++)
   {
      double func_value = T._phi[i]->eval(xk);
      if (func_value > delta0k)
         delta0k = func_value; 
   }

   delta0k *= -1;

   if (_delta < delta0k && fabs(etha) < _eps)
      return true;

   return false;
}

void ZoutendijkSolver::_formHelpTask( const MdTask &T, MdTask &out_task, Vector &t0 )
{
   out_task._taskDimension = T._taskDimension + 1;
   out_task.setMainFunc(_help_phi0);
   out_task.setMainFuncGrad(_help_gradphi0);

   _helpfulFuncStorage.resize(T._phi.size());
   _helpGradFuncStorage.resize(T._phi_grad.size());

   for (int i = 0; i != T._phi.size(); i++)
   {
      _helpfulFuncStorage[i].setFunc(T._phi[i]);
      out_task.addFunc(_helpfulFuncStorage[i]);
   }

   for (int i = 0; i != T._phi_grad.size(); i++)
   {
      _helpGradFuncStorage[i].setFunc(T._phi_grad[i]);
      out_task.addGradFunc(_helpGradFuncStorage[i]);
   }

   double _initVal = 2.0;

   t0.initValue(_initVal);
   t0[t0.size() - 1] = -(1 << 24);

   Vector tmp(t0.size() - 1);

   tmp.initValue(_initVal);

   for (int i = 0; i != T._phi.size(); i++)
   {
      double func_value = T._phi[i]->eval(tmp);

      if (func_value > t0[t0.size() - 1])
         t0[t0.size() - 1] = func_value;
   }
}

void ZoutendijkSolver::findFirstPoint( Vector &res )
{
   MdTask helpTask;
   Vector u0(_T._taskDimension + 1), t0(_T._taskDimension + 1);

   _solvingHelpTask = true;
   _formHelpTask(_T, helpTask, t0);
   _solve(helpTask, u0, t0);
   _solvingHelpTask = false;

   u0.popBack();

   //for (int i = 0; i != _T._phi.size(); i++)
   //{
   //   cout << _T._phi[i]->eval(u0) << " ";
   //}

   res = u0;
}