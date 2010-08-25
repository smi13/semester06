#include "md_task.h"

MdTask::MdTask() 
{
}

void MdTask::setMainFunc( Func &phi0 )
{
   _phi0 = &phi0;
}

void MdTask::setMainFuncGrad( VectorFunc &phi0_grad )
{
   _phi0_grad = &phi0_grad;
}

void MdTask::addFunc( Func &phi )
{
   _phi.push_back(&phi);
}

void MdTask::addGradFunc( VectorFunc &phi_grad)
{
   _phi_grad.push_back(&phi_grad);
}

void MdTask::setTaskDimension( int new_dimension )
{
   _taskDimension = new_dimension;
}