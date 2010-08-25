#ifndef potential_solver_h__
#define potential_solver_h__

#include <vector>

class TransportTask;
class Matrix;

using namespace std;

class PotentialSolver
{
public:

   class IntPair
   {
   public:
      int i, j;

      IntPair( int new_i, int new_j ) : i(new_i), j(new_j)
      {
      }
   };

   enum
   {
      free_cell = -100
   };

   PotentialSolver( TransportTask &T );

   double solve( Matrix &res );

private:

   TransportTask &T;

   vector<IntPair> _cycle;

   Matrix _alphaMatr;

   bool _findCycle( Matrix &visited, Matrix &sol, int i1, int j1, int i2, int j2, bool row_or_col );
   void _findPotentials( Matrix &A, vector<double> &b, vector<double> &alpha, vector<double> &betha );

   void _northWestCornerMethod( Matrix &base );

};

#endif // potential_solver_h__