#ifndef _cutoffplane_solver_h
#define _cutoffplane_solver_h

#include "md_task_solver.h"
#include "function.h"

struct LpTask;
class Matrix;

class CutOffPlaneSolver : public MdTaskSolver
{
public:

   CutOffPlaneSolver( MdTask &T, LpTask &S0 );

   virtual double solve( Vector &res );

private:

   void _findMax( Vector &x, NumberSet &I );
   bool _isOver();
   bool _belongsToOmega( Vector &x );
   void _buildSubgradient( Vector &x );

   Vector _nextVector();

   LpTask &_Sk;

   ComplexVectorFunction _helpfulFuncGrad;
   ComplexFunction _helpfulFunc;

};

#endif /* _cutoffplane_solver_h */