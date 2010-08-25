#include <iostream>
#include <cmath>

#include "comdef.h"
#include "vector.h"
#include "number_set.h"
#include "matrix.h"

istream & operator>>( istream &Istr, Vector &A )
{
   for (int i = 0; i < A.size(); i++)
   {
      Istr >> A._storage[i];
   }

   return Istr;
}

ostream & operator<<( ostream &Ostr, Vector &A )
{
   int n = A.size();

   for (int i = 0; i < n; i++)
      Ostr << A._storage[i] << ' ';

   return Ostr;
}

Vector::Vector()
{
}

Vector::Vector( int n )
{
   setSize(n);
}

Vector::Vector( const Vector &v )
{
   copy(v);
}

int Vector::size() const
{
   return _storage.size();
}

double & Vector::operator []( int i )
{
   return _storage[i];
}

double Vector::operator[]( int i ) const
{
   return _storage[i];
}

void Vector::setSize( int n )
{
   _storage.resize(n);
}

void Vector::positive( NumberSet &v )
{
   v.clear();

   for (int i = 0; i != _storage.size(); i++)
      if (_storage[i] > 0)
         v.pushBack(i);
}

void Vector::zeroed( NumberSet &v )
{
   v.clear();

   for (int i = 0; i != _storage.size(); i++)
      if (_storage[i] < eps)
         v.pushBack(i);
}

void Vector::pushBack( double &a )
{
   _storage.push_back(a);
}

void Vector::initUnit()
{
   initValue(1);
}

void Vector::initUnit( const NumberSet &set )
{
   for (int i = 0; i != set.size(); i++)
      _storage[set[i]] = 1.0;
}

void Vector::operator=( const Vector &v )
{
   copy(v);
}

void Vector::copy( const Vector &v )
{
   _storage = v._storage;
}

void Vector::resize( int n )
{
   _storage.resize(n);
}

void Vector::initZero( const NumberSet &set )
{
   for (int i = 0; i != set.size(); i++)
      _storage[set[i]] = 1.0;
}

void Vector::initZero()
{
   initValue(0);
}

void Vector::initValue( const double v )
{
   for (int i = 0; i != _storage.size(); i++)
      _storage[i] = v;
}

Vector & Vector::operator*=( double a )
{
   for (int i = 0; i != _storage.size(); i++)
      _storage[i] *= a;

   return *this;
}

Vector Vector::operator*( double a ) const
{
   Vector tmp = *this;

   tmp *= a;

   return tmp;
}

double Vector::operator*( const Vector &v ) const
{
   double res = 0;

   for (int i = 0; i != _storage.size(); i++)
      res += _storage[i] * v._storage[i];

   return res;
}

Vector Vector::operator*( Matrix &m )
{
   Vector res;

   res.resize(m.n());

   for (int i = 0; i < m.n(); i++)
      res[i] = (*this) * m[i];

   //DEBUG output
   //cout << (*this) << endl;
   //cout << m << endl;
   //cout << res << endl;

   return res;
}

Vector Vector::operator()( const NumberSet &set )
{
   Vector res;

   res.resize(set.size());

   for (int i = 0; i < res.size(); i++)
      res[i] = (*this)[set[i]];

   return res;
}

Vector Vector::operator-( const Vector &v )
{
   Vector res;

   res.resize(size());

   for (int i = 0; i < size(); i++)
   {
      res[i] = _storage[i] - v._storage[i];
   }

   return res;
}

bool Vector::checkPositive( int &false_index )
{
   for (int i = _storage.size() - 1; i >= 0; i--)
      if (_storage[i] < 0 && fabs(_storage[i]) > eps)
      {
         false_index = i;
         return false;
      }

   return true;
}

bool Vector::checkNegative( int &false_index )
{
   for (int i = 0; i != _storage.size(); i++)
      if (_storage[i] > 0 && fabs(_storage[i]) > eps)
      {
         false_index = i;
         return false;
      }

   return true;
}

bool Vector::checkNegative()
{
   int tmp;

   return checkNegative(tmp);
}

bool Vector::checkZero( int &false_index )
{
   for (int i = 0; i != _storage.size(); i++)
      if (fabs(_storage[i]) > eps)
      {
         false_index = i;
         return false;
      }
   return true;
}

void Vector::threshold()
{
   for (int i = 0; i != _storage.size(); i++)
      if (fabs(_storage[i]) < eps)
         _storage[i] = 0;
}

Vector Vector::operator+( const Vector &v2 )
{
   Vector res(this->size());

   for (int i = 0; i < res.size(); i++)
      res[i] = this->_storage[i] + v2[i];

   return res;
}

void Vector::setFor( NumberSet &set, double value )
{
   for (int i = 0; i < set.size(); i++)
      this->_storage[set[i]] = value;
}

void Vector::setFor( NumberSet &set, Vector &vec )
{
   for (int i = 0; i < set.size(); i++)
      this->_storage[set[i]] = vec[i];
}

double Vector::norm2()
{
   double res = 0;

   for (int i = 0; i != _storage.size(); i++)
   {
      res += _storage[i] * _storage[i];
   }

   return res;
}

void Vector::popBack()
{
   _storage.pop_back();
}

Vector & Vector::operator+=( const Vector &v )
{
   *this = (*this) + v;
   return *this;
}