#ifndef _md_task_h
#define _md_task_h

#include <vector>

#include "function.h"

using namespace std;

class MdTask
{
public:

   MdTask();

   void setMainFunc( Func &phi0 );
   void setMainFuncGrad( VectorFunc &phi0_grad );

   void addFunc( Func &phi );
   void addGradFunc( VectorFunc &phi_grad);

   void setTaskDimension( int new_dimension );

   int _taskDimension;

   Func *_phi0;
   VectorFunc *_phi0_grad;

   vector<Func *> _phi;
   vector<VectorFunc *> _phi_grad;
};


#endif /* md_task.h */