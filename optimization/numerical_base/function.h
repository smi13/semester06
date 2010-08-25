#ifndef _function_h
#define _function_h

#include "vector.h"

template<class ret, class arg>
class Function
{
public:

   virtual ret operator()( arg x ) = 0;

   ret eval( arg x )
   {
      return operator()(x);
   }

private:

};

/* Helpful typedefs & macro definitions */

class Vector;
class Matrix;

typedef Function<double, double&> SimpleFunc;
typedef Function<double, Vector&> Func;
typedef Function<Vector, Vector&> VectorFunc;
typedef Function<Matrix, Vector&> MatrixFunc;

#define SIMPLE_FUNC(name, expr)             \
class name : public SimpleFunc              \
{                                           \
public:                                     \
   virtual double operator()( double &x )   \
   {                                        \
      return expr;                          \
   }                                        \
};                                          \

#define VECTOR_FUNC(name, expr)             \
class name : public VectorFunc              \
{                                           \
public:                                     \
   virtual Vector operator()( Vector &x )   \
   {                                        \
      Vector res(x.size());                 \
      expr;                                 \
      return res;                           \
   }                                        \
};

#define MATRIX_FUNC(name, expr) \
class name : public MatrixFunc              \
{                                           \
public:                                     \
   virtual Matrix operator()( Vector &x )   \
   {                                        \
      Matrix res(x.size(), x.size());       \
      expr;                                 \
      return res;                           \
   }                                        \
};

#define FUNC(name, expr)                    \
class name : public Func                    \
{                                           \
public:                                     \
   virtual double operator()( Vector &x )   \
   {                                        \
      return expr;                          \
   }                                        \
};

class ComplexFunction : public Func
{
public:

   virtual double operator()( Vector &x )
   {
      Vector tmp(x.size() - 1);

      for (int i = 0; i < tmp.size(); i++)
         tmp[i] = x[i];

      return _phi_i->eval(tmp) - x[tmp.size()];
   }

   void setFunc( Func *new_func )
   {
      _phi_i = new_func;
   }

private:
   Func *_phi_i;
};

class ComplexVectorFunction : public VectorFunc
{
public:

   virtual Vector operator()( Vector &x )
   {
      Vector tmp(x.size() - 1);

      for (int i = 0; i < tmp.size(); i++)
         tmp[i] = x[i];

      tmp = _grad_phi_i->eval(tmp);

      Vector res(x.size());

      for (int i = 0; i < tmp.size(); i++)
         res[i] = tmp[i];

      res[tmp.size()] = -1;

      return res;
   }

   void setFunc( VectorFunc *new_func )
   {
      _grad_phi_i = new_func;
   }

private:
   VectorFunc *_grad_phi_i;
};

#endif /* _function_h_ */