#ifndef _zoutendijk_solver_h
#define _zoutendijk_solver_h

#include <vector>

using namespace std;

#include "md_task_solver.h"
#include "function.h"
#include "lp_task.h"
#include "md_task.h"
#include "vector.h"

class MdTaskSolver;
class Vector;

static int taskDimension;

FUNC(HelpTaskPhi0, x[taskDimension]);
VECTOR_FUNC(HelpTaskGradPhi0, for (int i = 0; i != taskDimension; i++) res[i] = 0.0; res[taskDimension] = 1.0;);

class ZoutendijkSolver : public MdTaskSolver
{
public:

   ZoutendijkSolver( MdTask &T );

   virtual double solve( Vector &res );

   void findFirstPoint( Vector &res );

private:

   Vector _nextDirection( MdTask &T, Vector &xk, double &out_etha );
   Vector _nextStep(  MdTask &T, Vector &xk, double etha, Vector &s );
   bool _isOver( MdTask &T, Vector &xk, double etha );

   void _formHelpTask( const MdTask &T, MdTask &out_task, Vector &t0 );
   void _getIdelta( MdTask &T, vector<int> &i_delta, Vector &cur );
   void _prepareLPTask( MdTask &T, LpTask &lp_task, Vector &cur, vector<int> &i_delta );

   double _solve( MdTask &T, Vector &res, Vector &base );

   double _alpha, _lambda, _delta;

   bool _solvingHelpTask;

   HelpTaskPhi0 _help_phi0;
   HelpTaskGradPhi0 _help_gradphi0;
   vector<ComplexFunction> _helpfulFuncStorage;
   vector<ComplexVectorFunction> _helpGradFuncStorage;
};

#endif /* _zoutendijk_solver_h_ */