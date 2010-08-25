
#include <istream>
#include <ostream>

#include <cstdio>
#include <cmath>

#include "transport_task.h"
#include "vector.h"
#include "matrix.h"

istream &operator>>( istream &Istr, TransportTask &T )
{
   Istr >> T._price;

   T._colOrRow = 0;

   T.m = T._price.m();
   T.n = T._price.n();

   T._stock.setSize(T.m);
   Istr >> T._stock;

   T._necessity.setSize(T.n);
   Istr >> T._necessity;

   T._reformTask();   

   T.m = T._price.m();
   T.n = T._price.n();

   T._getComplications();   

   return Istr;
}

ostream &operator<<( ostream &Ostr, TransportTask &T )
{
   Ostr << T.m << " " << T.n << endl;
   Ostr << T._price << endl << endl;
   Ostr << T._stock << endl << endl;
   Ostr << T._necessity << endl;

   return Ostr;
}

bool TransportTask::wasReformed( int &ColOrRow )
{
   if (_colOrRow == 0)
      return false;

   ColOrRow = _colOrRow;

   return true;

}

void TransportTask::_reformTask()
{
   //Reform open task in close form
   double sa = 0, sb = 0;

   for (int i = 0; i != _stock.size(); i++)
      sa += _stock[i];

   for (int i = 0; i != _necessity.size(); i++)
      sb += _necessity[i];

   double tmp = fabs(sb - sa);

   if (sa > sb)
   {
      Vector a(m);

      _colOrRow = 1;

      a.initZero();

      _necessity.pushBack(tmp);
      _price << a;
   }
   else if (sa < sb)
   {
      _stock.pushBack(tmp);

      _colOrRow = -1;

      for (int i = 0; i != n; i++)
      {
         double a = 0;
         _price[i].pushBack(a);
      }
   }
}

void TransportTask::_getComplications()
{
   char c;

   cout << "Task loaded. Complications? (y/N)" << endl;

   cin >> c;

   if (c == 'Y' || c == 'y')
   {
      while (true)
      {
         int type, i, j, p;
         bool _timeToBreak = false;

         cout << "Complication type (1 or 4 (-1 for end)):" << endl;

         cin >> type;

         Complication tmp;

         switch (type)
         {
         default:
            _timeToBreak = true;
            break;
         case 1:
            cout << "(i, j): " << endl;
            cin >> i >> j;
            
            tmp.type = 1;
            tmp.i = i;
            tmp.j = j;
            tmp.p = _price[j][i];

            _price[j][i] = 1 << 24;

            _special.push_back(tmp);

            break;
         case 4:
            cout << "(i, j): " << endl;
            cin >> i >> j;
            cout << "p: " << endl;
            cin >> p;
            _stock[i] -= p;
            _necessity[j] -= p;

            tmp.type = 4;
            tmp.i = i;
            tmp.j = j;
            tmp.p = p;
            
            _special.push_back(tmp);
            break;
         }

         if (_timeToBreak)
            break;
      }
   }
}

void TransportTask::fixSolution( Matrix &Solution, double &min_value )
{
   for (int i = 0; i != _special.size(); i++)
   {
      switch (_special[i].type)
      {
      case 1:
         _price[_special[i].j][_special[i].i] = _special[i].p;
         break;
      case 4:
         _stock[_special[i].i] += _special[i].p;
         _necessity[_special[i].j] += _special[i].p;

         Solution[_special[i].j][_special[i].i] += _special[i].p;

         min_value += _special[i].p * _price[_special[i].j][_special[i].i];

         break;
      }
   }
}