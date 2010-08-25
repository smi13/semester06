#ifndef _enumerator_h
#define _enumerator_h

#include <vector>

using namespace std;

class Enumerator
{
public:

   Enumerator( int m, int n );

   vector<int> & next( bool &isOver );

private:

   int _m, _n, _p;
   vector<int> _cur;
};

#endif /* _enumerator_h */