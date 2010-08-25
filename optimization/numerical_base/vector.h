#ifndef _vector_h
#define _vector_h

#include <vector>

class NumberSet;
class Matrix;

using namespace std;

class Vector
{

   friend istream & operator>>( istream &Istr, Vector &A );
   friend ostream & operator<<( ostream &Ostr, Vector &A );

public:

   Vector();
   Vector( int n );
   Vector( const Vector &v );
   void operator=( const Vector &v );

   bool checkPositive( int &false_index );
   bool checkNegative( int &false_index );
   bool checkNegative();
   bool checkZero( int &false_index );

   void resize( int n );
   void setSize( int n ); 
   int size() const;
   void pushBack( double &a );
   void copy( const Vector &v );

   void initValue( const double v );

   void initUnit( const NumberSet &set );
   void initUnit();

   void initZero( const NumberSet &set );
   void initZero();

   void positive( NumberSet &v );
   void zeroed( NumberSet &v );

   void popBack();

   void threshold();

   void setFor( NumberSet &set, double value );
   void setFor( NumberSet &set, Vector &vec );

   double norm2();

   Vector operator()( const NumberSet &set );
   Vector operator-( const Vector &v );
   Vector operator+( const Vector &v2 );
   

   Vector &operator*=( const double a );
   Vector &operator+=( const Vector &v );
   Vector operator*( double a ) const;
   Vector operator*( Matrix &m );
   double operator*( const Vector &v ) const;

   double operator[]( int i ) const;
   double & operator[]( int i );

private:

   vector<double> _storage;
};

#endif /* _vector_h */