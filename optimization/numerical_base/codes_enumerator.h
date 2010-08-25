#ifndef codes_enumerator_h__
#define codes_enumerator_h__

#include <vector>

using namespace std;

class CodesEnumerator
{
public:

   CodesEnumerator();
   CodesEnumerator( int size, int base );

   void reset( int size, int base );
   bool nextCode( vector<int> &code );

private:

   int _size;
   int _base;

   vector<int> _storage;
};

#endif // codes_enumerator_h__