#include "number_set.h"

NumberSet::NumberSet()
{
}

NumberSet::NumberSet( int n )
{
   _storage.resize(n);
}

void NumberSet::initOrigin( int n )
{
   _storage.resize(n);

   for (int i = 0; i != _storage.size(); i++)
      _storage[i] = i;
}

void NumberSet::pushBack( int i )
{
   _storage.push_back(i);
}

int NumberSet::operator[]( int i ) const
{
   return _storage[i];
}

int & NumberSet::operator[]( int i )
{
   return _storage[i];
}

NumberSet NumberSet::operator-( const NumberSet &n )
{
   NumberSet tmp;

   for (int i = 0; i != _storage.size(); i++)
   {
      bool toAdd = true;

      for (int j = 0; j != n._storage.size(); j++)
      {
         if (_storage[i] == n._storage[j])
         {
            toAdd = false;
            break;
         }
      }

      if (toAdd)
         tmp._storage.push_back(_storage[i]);
   }

   return tmp;
}


NumberSet &NumberSet::operator=( const NumberSet &n )
{
   _storage.clear();
   _storage = n._storage;
   return *this;
}

int NumberSet::size() const
{
   return _storage.size();
}

void NumberSet::replace( int i, int j )
{
   for (int k = 0; k != _storage.size(); k++)
      if (_storage[k] == i)
         _storage[k] = j;
}

void NumberSet::clear()
{
   _storage.clear();
}

NumberSet NumberSet::operator-( const int index )
{
   NumberSet res(_storage.size() - 1);

   for (int i = 0, j = 0; i < size(); i++)
   {
      if (_storage[i] != index)
      {
         res[j] = _storage[i];
         j++;
      }
   }
   return res;
}

bool NumberSet::operator==( const NumberSet &set )
{
   if (_storage.size() != set.size())
      return false;

   for (int i = 0; i != _storage.size(); i++)
      if (_storage[i] != set[i])
         return false;

   return true;
}

int NumberSet::getPlace( int index )
{
   for (int i = 0; i < size(); i++)
      if (_storage[i] == index)
         return i;

   return -1;
}

ostream & operator<<( ostream &Ostr, const NumberSet &A )
{
   for (int i = 0; i != A._storage.size(); i++)
      Ostr << A._storage[i] << " ";

   Ostr << endl;

   return Ostr;
}