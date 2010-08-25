#ifndef od_min_finder_h
#define od_min_finder_h

#include "function.h"

class OdMinFinder
{
public:

   enum METHOD
   {
      DICHOTOMY,
      GOLDEN_RATIO
   };
   
   OdMinFinder( SimpleFunc &func, double l, double r );

   double find( double &x, METHOD m, int iter_number = -1 );

   int getIterCounter();
   int getFuncCounter();
   void setBorders( double &a, double &b );
   void getBorders( double &a, double &b );
   void resetCounters();

   void setEps( double _eps );

private:

   void _iterDichotomy( double &a, double &b );
   double _iterGoldenRatio( double &a, double &b );

   double _findDichotomy( double &x, int iter_number );
   double _findGoldenRatio( double &x, int iter_number );

   int _funcCounter;
   int _iterCounter;
   double _eps;
   double _a, _b;
   SimpleFunc &_func;
};

#endif /* od_min_finder_h */