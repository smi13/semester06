
#include "number_set.h"
#include "basis_enumerator.h"

BasisEnumerator::BasisEnumerator()
{
}

bool BasisEnumerator::nextBasis( NumberSet &Basis )
{
   vector<int> cur_code;

   if (!_enum.nextCode(cur_code))
      return false;

   Basis = _Nk;

   for (int i = 0; i != cur_code.size(); i++)
   {
      if (cur_code[i] == 0)
         Basis[_positions[i]] = _Nk[_positions[i]];
      else
         Basis[_positions[i]] = _Lk[cur_code[i] - 1];
   }

   return true;
}

void BasisEnumerator::set( NumberSet &Lk, NumberSet &Nk, NumberSet &Pk )
{
   _Lk = Lk;
   _Nk = Nk;
   _Pk = Pk;
   _enum.reset(Pk.size(), Lk.size() + 1);

   _positions.resize(_Pk.size());

   for (int i = 0; i != _positions.size(); i++)
   {
      _positions[i] = _Nk.getPlace(_Pk[i]);
   }
}