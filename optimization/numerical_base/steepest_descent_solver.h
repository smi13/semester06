#ifndef _steepest_descent_solver_h
#define _steepest_descent_solver_h

#include "md_task_solver.h"
#include "function.h"

class Vector;

class AdditionalFunc : public SimpleFunc 
{
public:

   AdditionalFunc( Vector &u, Vector &p, Func &func );

   virtual double operator()( double &x );

private:

   Func &_func;
   Vector &_u, &_p;
};

class SteepestDescentSolver : public MdTaskSolver
{
public:

   SteepestDescentSolver( MdTask &T, Vector &x0, bool feasibilityCheck = false );

   virtual double solve( Vector &res );

   bool _isFeasible( Vector &x );

private:

   bool _feasibilityCheck;

   Vector &_x0;
};

#endif /* _steepest_descent_solver_h */