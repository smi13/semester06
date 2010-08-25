#ifndef modif_min_finder_h__
#define modif_min_finder_h__

#include "function.h"

class Vector;
class Matrix;

class AdditionalFunc : public SimpleFunc 
{
public:

   AdditionalFunc( Vector &u, Vector &p, Func &func );

   virtual double operator()( double &x );

private:

   Func &_func;
   Vector &_u, &_p;
};

class ModifMinFinder
{
public:

   ModifMinFinder( double lamda, Func &func, VectorFunc &grad, MatrixFunc &hesse );

   double find( Vector &x0, Vector &res );
   int getIterCount();
   void reset();

   void setEps( double eps );

private:

   int _iterCount;
   double _lambda;
   double _eps;

   Func &_func;
   VectorFunc &_grad;
   MatrixFunc &_hesse;
};

#endif // modif_min_finder_h__