#include "codes_enumerator.h"

CodesEnumerator::CodesEnumerator( int size, int base )
{

}

CodesEnumerator::CodesEnumerator()
{
}

bool CodesEnumerator::nextCode( vector<int> &code )
{
   int count = 0;

   for (int i = 0; i != _storage.size(); i++)
   {
      _storage[i]++;
      if (_storage[i] == _base)
      {
         _storage[i] = 0;
         count++;
      }
      else
         break;
   }

   code = _storage;

   if (count == _storage.size())
      return false;

   return true;
}

void CodesEnumerator::reset( int size, int base )
{
   _size = size;
   _base = base;
   _storage.resize(_size);

   for (int i = 0; i != _storage.size(); i++)
      _storage[i] = 0;
}