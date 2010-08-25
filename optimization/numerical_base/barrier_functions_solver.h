#ifndef _barrier_functions_solver_h
#define _barrier_functions_solver_h

#include "function.h"
#include "md_task_solver.h"

class Vector;
class MdTask;

class BarrierFunctionsSolver : public MdTaskSolver
{
public:

   class BarrierFunction : public Func
   {
   public:

      BarrierFunction( double new_BarrierParamter, MdTask &new_Task );

      virtual double operator()( Vector &x );

   private:

      double _barrierParameter;
      MdTask &_T;
   };

   class BarrierFunctionGrad : public VectorFunc
   {
   public:

      BarrierFunctionGrad( double new_BarrierParamter, MdTask &new_Task );

      virtual Vector operator()( Vector &x );

   private:

      double _barrierParameter;
      MdTask &_T;
   };

   BarrierFunctionsSolver( MdTask &T, double new_barrierParamter, double new_betha );

   virtual double solve( Vector &res );

   void setDebugOutput();

   bool isFeasible( Vector &x );

private:

   bool _debugOutput;

   void _solve( Vector &res );
   double _barrierParameter, _betha;
};

#endif /* _barrier_functions_solver_h */