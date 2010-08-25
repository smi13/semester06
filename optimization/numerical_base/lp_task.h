#ifndef _task_h
#define _task_h

#include <vector>
#include <iostream>

#include "matrix.h"
#include "vector.h"

struct LpTask
{
   friend istream & operator>>( istream &Istr, LpTask &A );
   friend ostream & operator<<( ostream &Ostr, LpTask &A );

   LpTask();
   LpTask( Matrix &A, Vector &b, Vector &c );

   int m, n;
   Matrix A;
   Vector c;
   Vector b;
};

#endif /* _task_h */