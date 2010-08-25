#include <cmath>

#include "comdef.h"

#include "barrier_functions_solver.h"
#include "md_task.h"
#include "zoutendijk_solver.h"
#include "md_min_finder.h"
#include "steepest_descent_solver.h"

double BarrierFunctionsSolver::solve( Vector &res )
{
   Vector x0;
   ZoutendijkSolver solver(_T);

   solver.findFirstPoint(x0);
/*   
   for (int i = 0; i != _T._phi.size(); i++)
      cout << _T._phi[i]->eval(x0) << " ";

   cout << endl;*/

   res = x0;

   _solve(res);

   return _T._phi0->eval(res);
}

BarrierFunctionsSolver::BarrierFunctionsSolver( MdTask &T, double new_barrierParameter, double new_betha ) : 
            MdTaskSolver(T), _barrierParameter(new_barrierParameter), _betha(new_betha), _debugOutput(false)
{
}

void BarrierFunctionsSolver::_solve( Vector &xk )
{
   while (true)
   {
      MdTaskSolver::_iterCounter++;

      MdTask helpful_task;
      BarrierFunction bf(_barrierParameter, _T);
      BarrierFunctionGrad bfg(_barrierParameter, _T);

      helpful_task.setMainFunc(bf);
      helpful_task.setMainFuncGrad(bfg);
      helpful_task._taskDimension = _T._taskDimension;

      for (int i = 0; i != _T._phi.size(); i++)
      {
         helpful_task.addFunc(*(_T._phi[i]));
         helpful_task.addGradFunc(*(_T._phi_grad[i]));
      }

      SteepestDescentSolver min_finder(helpful_task, xk, true);

      Vector a = bfg(xk);

      min_finder.setEps(_eps);
      min_finder.solve(xk);

      double tmp = -_T._phi0->eval(xk) + bf(xk);

      if (_debugOutput)
      {
         cout << "Iter: " << MdTaskSolver::_iterCounter << endl 
              << "Mu_k: " << _barrierParameter << endl
              << "X_k: " << xk << endl
              << "B_xk: " << tmp / _barrierParameter << endl
              << "Etha_Mu_k: " << bf(xk) << endl
              << "Mu_k * B_xk: " << tmp << endl;

         cout << endl;
      }

      if (fabs(tmp) < _eps)
      {
         break;
      }

      _barrierParameter *= _betha;    
   }
}

void BarrierFunctionsSolver::setDebugOutput()
{
   _debugOutput = !_debugOutput;
}

BarrierFunctionsSolver::BarrierFunction::BarrierFunction( double new_BarrierParamter, MdTask &new_Task ) : 
            _barrierParameter(new_BarrierParamter), _T(new_Task)
{
}

double BarrierFunctionsSolver::BarrierFunction::operator()( Vector &x )
{
   double res_value = _T._phi0->eval(x);

   for (int i = 0; i != _T._phi.size(); i++)
   {
      res_value -= _barrierParameter * 1 / _T._phi[i]->eval(x);
   }

   return res_value;
}

BarrierFunctionsSolver::BarrierFunctionGrad::BarrierFunctionGrad( double new_BarrierParamter, MdTask &new_Task ) :
            _barrierParameter(new_BarrierParamter), _T(new_Task)
{
}

Vector operator*( double d, Vector v )
{
   return v * d;
}

Vector BarrierFunctionsSolver::BarrierFunctionGrad::operator()( Vector &x )
{
   Vector res = _T._phi0_grad->eval(x);

   for (int i = 0; i != _T._phi.size(); i++)
   {
      res += _barrierParameter * _T._phi_grad[i]->eval(x) * (1 / (_T._phi[i]->eval(x) * _T._phi[i]->eval(x)));
   }

   return res;
}