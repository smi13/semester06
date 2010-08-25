#ifndef transport_task_h__
#define transport_task_h__

#include <iostream>
#include <istream>
#include <ostream>

#include "matrix.h"
#include "vector.h"

using namespace std;

class TransportTask
{
public:

   struct Complication
   {
      int i, j;
      double p;

      int type;

      Complication()
      {
      }

      Complication( int new_i, int new_j, int new_p ) : i(new_i), j(new_j), p(new_p)
      {
      }
   };

   int m, n;

   int _colOrRow;
   
   vector<Complication> _special;

   friend istream &operator>>( istream &s, TransportTask &T );
   friend ostream &operator<<( ostream &s, TransportTask &T );

   Matrix _price;
   Vector _stock, _necessity;

   bool wasReformed( int &ColOrRow );

   void fixSolution( Matrix &Solution, double &min_value );

private:

   void _getComplications();
   void _reformTask();
};


#endif // transport_task_h__