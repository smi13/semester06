#include "od_min_finder.h"
#include "modif_min_finder.h"

#include "vector.h"
#include "matrix.h"

#include "function.h"

ModifMinFinder::ModifMinFinder( double lambda, 
                                Func &func,
                                VectorFunc &grad,
                                MatrixFunc &hesse ) : 
                              _iterCount(0), _lambda(lambda), _func(func), _grad(grad), _hesse(hesse)
{
}

double ModifMinFinder::find( Vector &x0, Vector &res )
{
   double grad_norm2;
   Vector xk = x0, x_star(2), grad;
   Matrix B, A;

   x_star.initValue(0.0);

   B = _hesse(x0) * (1 - _lambda) + _hesse(x_star) * _lambda;
   B.buildInverse(A);

   grad = _grad(xk);
   grad_norm2 = grad.norm2();

   while (grad_norm2 > _eps)
   {
      double alpha;
      Vector pk = A * grad;

      AdditionalFunc helpful_func(xk, pk, _func);
      OdMinFinder od_finder(helpful_func, 0, 1.0);

      od_finder.setEps(1e-11);
      od_finder.find(alpha, OdMinFinder::GOLDEN_RATIO);

      xk = xk - pk * alpha;

      grad = _grad(xk);
      grad_norm2 = grad.norm2();
      _iterCount++;
   }

   res = xk;

   return _func(xk);
}

int ModifMinFinder::getIterCount()
{
   return _iterCount;
}

void ModifMinFinder::reset()
{
   _iterCount = 0;
}

void ModifMinFinder::setEps( double eps )
{
   _eps = eps;
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