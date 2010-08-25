#ifndef _md_task_solver_h
#define _md_task_solver_h

class MdTask;
class Vector;

class MdTaskSolver
{
public:

   MdTaskSolver( MdTask &T );

   virtual double solve( Vector &res ) = 0;
   
   virtual void setEps( double new_eps );
   virtual int getIterCounter();
   virtual void resetIterCounter();

protected:

   MdTask &_T;
   int _iterCounter;
   double _eps;

};

#endif /* _md_task_solver_h */