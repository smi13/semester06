#include <cstring>
#include <cstdio>
#include <cmath>

#include "function.h"
#include "od_min_finder.h"

OdMinFinder::OdMinFinder( SimpleFunc &func , double a, double b ) : _funcCounter(0), _iterCounter(0), 
            _eps(-1), _a(a), _b(b), _func(func) 
{
}

double OdMinFinder::find( double &x, METHOD m, int iter_number )
{
   if (_eps < 0)
      throw "No way, bitch!";

   if (m == DICHOTOMY)
      return _findDichotomy(x, iter_number);

   if (m == GOLDEN_RATIO)
      return _findGoldenRatio(x, iter_number);

   throw "No way, bitch!";
}

int OdMinFinder::getIterCounter()
{
   return _iterCounter;
}

int OdMinFinder::getFuncCounter()
{
   return _funcCounter;
}

void OdMinFinder::_iterDichotomy( double &a, double &b )
{
   double x1, x2, delta = _eps / 3;
   
   x1 = (a + b) * 0.5 - delta;
   x2 = (a + b) * 0.5 + delta;

   double fx1 = _func(x1), fx2 = _func(x2);

   _funcCounter += 2;

   if (fx1 > fx2)
      a = x1;
   else
      b = x2;

   _iterCounter++;
}

double OdMinFinder::_findDichotomy( double &x, int iter_number )
{
   double a = _a, b = _b;

   if (iter_number == -1)
   {
      do 
      {
         double tmp = (_b - _a);

         if (fabs(tmp) < 2 * _eps)
            break;

         _iterDichotomy(_a, _b);

      } while (true);
   }
   else
   {
      while (iter_number-- > 0) 
      {
         _iterDichotomy(_a, _b);
      } 
   }

   x = (_a + _b) * 0.5;

   return _func(x);
}

double OdMinFinder::_iterGoldenRatio( double &a, double &b )
{
   int ret;
   static double u1, u2, y1, y2, tmp1, tmp2;

   if (_iterCounter == 0)
   {
      u1 = a + (3 - sqrt(5.0)) * 0.5 * (b - a);
      u2 = a + (sqrt(5.0) - 1) * 0.5 * (b - a);

      y1 = _func(u1);
      _funcCounter++;
      y2 = _func(u2);
      _funcCounter++;
   }

   if (y1 > y2)
   {
      ret = 1;
      a = u1;

      u1 = u2;
      y1 = y2;

      tmp1 = a + (3 - sqrt(5.0)) * 0.5 * (b - a);
      tmp2 = a + (sqrt(5.0) - 1) * 0.5 * (b - a);

      if (fabs(tmp1 - u1) > fabs(tmp2 - u1))
         u2 = tmp1;
      else
         u2 = tmp2;

      y2 = _func(u2);
      _funcCounter++;      
   }
   else
   {
      ret = 2;
      b = u2;

      u2 = u1;
      y2 = y1;

      tmp1 = a + (3 - sqrt(5.0)) * 0.5 * (b - a);
      tmp2 = a + (sqrt(5.0) - 1) * 0.5 * (b - a);

      if (fabs(tmp1 - u2) > fabs(tmp2 - u2))
         u1 = tmp1;
      else
         u1 = tmp2;

      y1 = _func(u1);
      _funcCounter++;
   }

   _iterCounter++;

   if (ret == 1)
      return u1;
   else
      return u2;
}

double OdMinFinder::_findGoldenRatio( double &x, int iter_number )
{
   double a = _a, b = _b;
   
   if (iter_number == -1)
   {
      do
      {
         double tmp = _b - _a;

         if (fabs(tmp) < 2 * _eps)
            break;

         x = _iterGoldenRatio(_a, _b);

      } while (true);
   }
   else
   {
      while (iter_number-- > 0) 
      {
         x = _iterGoldenRatio(a, b);
      } 
   }

   return _func(x);
}

void OdMinFinder::getBorders( double &a, double &b )
{
   a = _a;
   b = _b;
}

void OdMinFinder::setEps( double eps )
{
   this->_eps = eps;
}

void OdMinFinder::resetCounters()
{
   _funcCounter = 0;
   _iterCounter = 0;
}

void OdMinFinder::setBorders( double &a, double &b )
{
   _a = a;
   _b = b;
}
