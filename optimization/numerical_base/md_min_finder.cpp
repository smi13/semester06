#include <cmath>
#include <iomanip>

#include "function.h"

#include "od_min_finder.h"
#include "md_min_finder.h"

#include "vector.h"
#include "matrix.h"

#define ERROR_MSG "No way, bitch!"

MdMinFinder::MdMinFinder( int dim, double (&func)( vector<double> &var ), 
                         void (&grad)( vector<double> &var, vector<double> &res ), 
                         void (&grad2)( Matrix &m, vector<double> &var ),
                         vector<double> &begin_point ) :
                          _dim(dim), _func(func), _grad(grad), _grad2(grad2), a_f(_cur_u, _cur_p, func)
{
   _accum = 0;
   _cur_u = begin_point;
   _grad(_cur_u, _cur_p);
   _renewal_step = -1;
   _eps = -1;   
}

double MdMinFinder::find( vector<double> &x, METHOD m, int &iter_num )
{
   if (_dim < 0 || _eps < 0)
      throw ERROR_MSG;

   _m = m;

   if (m == NEWTON_METHOD)
   {
      _renewal_step = 1;
      _next_p();
      _alpha = 1.0;
      _newtonEps = 0.25;
   }

   if (m == STEEPEST_DESCENT)
      _renewal_step = 1;

   if (m == GRADIENT_MODIFICATION)
   {
      _renewal_step = 1;
   }

   if (m == CONJUGATE_GRADIENT)
      _renewal_step = _dim;

   if (_renewal_step < -1)
      throw ERROR_MSG;

   return _find(x, iter_num);
}

#include <iostream>

ostream & operator<<( ostream &os, vector<double> &v )
{
   for (int i = 0; i != v.size(); i++)
      os << v[i] << " ";

   return os;
}

double MdMinFinder::_find( vector<double> &x, int &iter_num )
{
   int count = 0;
   double res;

   while (true)
   {
      count++;

      if (_m != NEWTON_METHOD)
      {
         OdMinFinder od_finder(a_f, 0, 1.0);

         //cout << _cur_p << endl;

         od_finder.setEps(1e-11);
         od_finder.find(_alpha, OdMinFinder::GOLDEN_RATIO);

         //cout << alpha << endl;
      }
      else
      {
         double tmp_num = 0;
         vector<double> tmp;

         _grad(_cur_u, tmp);

         for (int i = 0; i != tmp.size(); i++)
            tmp_num += tmp[i] * _cur_p[i];

         double var1 = a_f(_alpha) - _func(_cur_u), var2 = _alpha * _newtonEps * tmp_num;

         while (var1 > var2)
         {
            _alpha /= 4.0;
            var1 = a_f(_alpha) - _func(_cur_u);
            var2 = _alpha * _newtonEps * tmp_num;
         }
      }

      if (!_next_u(_alpha))
         break;

      if (!_next_p())
         break;
   }

   iter_num = count;

   res = _func(_cur_u);
   x = _cur_u;

   return res;
}

MdMinFinder::~MdMinFinder()
{
}

bool MdMinFinder::_next_u( double alpha )
{
   double n1 = 0, n2 = 0;
   vector<double> new_u = _cur_u;

   if (_m != GRADIENT_MODIFICATION)
   {
      for (int i = 0; i != _cur_u.size(); i++)
      {
         new_u[i] -= alpha * _cur_p[i];
      }
   }
   else
   {
      Matrix A(2, 2);

      A[0][0] = -0.013160288145597802438;
      A[1][0] = 0.20841074075010485454;
      A[0][1] = 0.14734187040784522415;
      A[1][1] = -0.013160288145597802438;

      _cur_p[0] = (A[0][0] * _cur_p[0] + A[1][0] * _cur_p[1]);
      _cur_p[1] = (A[1][0] * _cur_p[0] + A[1][1] * _cur_p[1]);

      double norm = _cur_p[0] * _cur_p[0] + _cur_p[1] * _cur_p[1];

      if (norm < _eps)
         return false;

      new_u[0] -= alpha * _cur_p[0];
      new_u[1] -= alpha * _cur_p[1];
   }

   for (int i = 0; i != new_u.size(); i++)
   {
      n1 += new_u[i] * new_u[i];
      n2 += _cur_u[i] * _cur_u[i];
   }

   n1 = sqrt(n1);
   n2 = sqrt(n2);

   double dot_product = 0;
   vector<double> tmp1, tmp2;

   n1 = 0;
   n2 = 0;

   for (int i = 0; i != new_u.size(); i++)
   {
      n1 += new_u[i] * new_u[i];
      n2 += _cur_u[i] * _cur_u[i];
   }

//   if (fabs(n1 - n2) < _eps)
  //    return false;

   //Gradient chain pieces are orthogonal
   //in case of using Steepest Descent method
   //Let`s check it
   if (_renewal_step == 1 && _m != NEWTON_METHOD)
   {
      _grad(new_u, tmp1);
      _grad(_cur_u, tmp2);

      for (int i = 0; i != tmp1.size(); i++)
      {
         dot_product -= tmp1[i] * tmp2[i];
      }

      //cout << setprecision(10) << dot_product << endl;
   }

   _accum++;

   if (_accum == _renewal_step)
   {
      _betha = 0;
      _accum = 0;
   }
   else
      _betha = -n1 / n2;

   _cur_u = new_u;

   return true;
}

bool MdMinFinder::_next_p()
{
   double n1 = 0;
   vector<double> tmp;

   _grad(_cur_u, tmp);

   for (int i = 0; i != tmp.size(); i++)
      n1 += tmp[i] * tmp[i];

   if (n1 < _eps)
      return false;

   if (_m == NEWTON_METHOD)
   {
      Matrix matr_tmp;

      _grad2(matr_tmp, _cur_u);
      matr_tmp.solveSystem(tmp, _cur_p);
   }
   else
   {
      if (_m != GRADIENT_MODIFICATION)
         for (int i = 0; i != tmp.size(); i++)
         {
            _cur_p[i] = tmp[i] - _betha * _cur_p[i];
         }
   }

   return true;
}

void MdMinFinder::setEps( double eps )
{
   _eps = eps;
}

double MdMinFinder::AdditionalFunc::operator()( double &alpha )
{
   vector<double> v;

   v.resize(_u.size());

   for (int i = 0; i != _u.size(); i++)
      v[i] = _u[i] - _p[i] * alpha;

   return _func(v);
}

MdMinFinder::AdditionalFunc::AdditionalFunc( vector<double> &u, vector<double> &p, double (&func)( vector<double> &x ) ) : 
                                _u(u), _p(p), _func(func)
{
}
 