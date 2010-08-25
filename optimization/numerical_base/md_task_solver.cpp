#include "md_task_solver.h"

MdTaskSolver::MdTaskSolver( MdTask &T ) : _T(T), _iterCounter(0), _eps(1e-1)
{
}

int MdTaskSolver::getIterCounter()
{
   return _iterCounter;
}

void MdTaskSolver::resetIterCounter()
{
   _iterCounter = 0;
}

void MdTaskSolver::setEps( double new_eps )
{
   _eps = new_eps;
}
