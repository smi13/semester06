#include "enumerator.h"

Enumerator::Enumerator( int m, int n )
{
   _m = m;
   _n = n;
   _p = n;

   _cur.resize(_m);

   for (int i = 0; i != _cur.size(); i++)
   {
      _cur[i] = i;
   }
}

vector<int> & Enumerator::next( bool &isOver )
{
   isOver = false;

   if (_m == _n)
   {
      isOver = true;
      return _cur;
   }

   if (_p < 1)
   {
      isOver = true;
      return _cur;
   }

   if (_cur[_n - 1] == _m - 1)
      _p--;
   else
      _p = _n;

   if (_p >= 1)
   { 
      int i;

      for (i = _n - 1; i >= _p - 1; i--)
         _cur[i] = _cur[_p - 1] + i - (_p - 1) + 1;
   }
   else
   {
      isOver = true;
   }

   return _cur;
}