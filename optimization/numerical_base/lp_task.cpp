#include "lp_task.h"

istream & operator>>( istream &Istr, LpTask &T )
{
   Istr >> T.A;

   T.m = T.A.m();
   T.n = T.A.n();

   T.b.setSize(T.m);
   Istr >> T.b;

   T.c.setSize(T.n);
   Istr >> T.c;

   return Istr;
}

ostream & operator<<( ostream &Ostr, LpTask &T )
{
   Ostr << T.m << ' ' << T.n << endl;
   Ostr << "A: " << endl << T.A << endl;
   Ostr << "B: " << T.b << endl;
   Ostr << "C: " << T.c << endl;

   return Ostr;
}

LpTask::LpTask()
{
}

LpTask::LpTask( Matrix &NewA, Vector &NewB, Vector &NewC )
{
   n = NewA.n();
   m = NewA.m();
   A = NewA;
   b = NewB;
   c = NewC;
}