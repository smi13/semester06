#ifndef _number_set_h
#define _number_set_h

#include <vector>
#include <iostream>

using namespace std;

class NumberSet
{
public:

   friend ostream & operator<<( ostream &Ostr, const NumberSet &A );

   NumberSet();
   NumberSet( int n );

   void initOrigin( int n );
   void pushBack( int i );
   void replace( int i, int j);
   int size() const;
   void clear();
   int operator[]( int i ) const;
   int &operator[]( int i );

   int getPlace( int index );

   NumberSet &operator=( const NumberSet &n );
   NumberSet operator-( const NumberSet &n );
   NumberSet operator-( const int index );

   bool operator==( const NumberSet &set );

private:

   vector<int> _storage;

};

#endif /* _number_set_h */