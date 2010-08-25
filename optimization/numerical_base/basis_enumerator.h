#ifndef basis_enumerator_h__
#define basis_enumerator_h__

#include <vector>

#include "codes_enumerator.h"
#include "number_set.h"

using namespace std;

class NumberSet;

class BasisEnumerator
{
public:

   BasisEnumerator();

   void set( NumberSet &Lk, NumberSet &Nk, NumberSet &Pk );

   bool nextBasis( NumberSet &Basis );

private:

   vector<int> _positions;

   NumberSet _Nk, _Lk, _Pk;
   CodesEnumerator _enum;
};

#endif // basis_enumerator_h__