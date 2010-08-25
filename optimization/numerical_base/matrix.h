#ifndef _matrix_h
#define _matrix_h

#include <vector>
#include <iostream>

using namespace std;

class NumberSet;
class Vector;

class Matrix
{
   friend istream & operator>>( istream &Istr, Matrix &A );
   friend ostream & operator<<( ostream &Ostr, const Matrix &A );

public:

   Matrix();
   Matrix( int m );
   Matrix( int n, int m );

   int m() const;
   int n() const;

   bool isDegenerate();

   void initValue( const double val );
   void initZero();
   void initUnit();

   void mulRow( int i, const double a );
   void copy( const Matrix &A );

   Matrix operator()( const NumberSet &set );

   Vector & operator[]( const int i );
   Vector operator[]( const int i ) const;

   void operator<<( Vector &v );

   Matrix operator|( const Matrix &A ) const;
   Matrix & operator|=( const Matrix &A );

   Matrix operator*( const double a ) const;
   Vector operator*( const Vector &v ) const;
   Matrix operator*( const Matrix &m ) const;

   Matrix operator+( const Matrix &m ) const;

   Matrix & operator*=( const double a );
   Matrix & operator*=( const Vector &v );
   Matrix & operator*=( const Matrix &m );

   void replaceValue( const double &var1, const double &var2 );

   void setSpecialOutput( int val );
   void resetSpecialOutput();

   bool buildInverse( Matrix &res );
   bool solveSystem( const vector<double> &_b, vector<double> &res );

   void threshold();

private:

   bool _specialoutput;
   double _val;

   vector<Vector> _storage;
};

#endif /* _matrix_h */