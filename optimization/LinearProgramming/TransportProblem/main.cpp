#include <iostream>
#include <fstream>

#include "transport_task.h"
#include "potential_solver.h"

using namespace std;

void Work( const char *InFileName, const char *OutFileName )
{
   ifstream f(InFileName);
   TransportTask T;

   f >> T;
   f.close();

   double min_value;
   Matrix res;
   PotentialSolver solver(T);

   min_value = solver.solve(res);

   res.replaceValue(PotentialSolver::free_cell, 0);
   T.fixSolution(res, min_value);

   if (OutFileName != 0)
      freopen(OutFileName, "wt", stdout);

   cout << T << endl;

   cout << "Min cost: " << min_value << endl << "Solution: " << endl;
   cout << res << endl;

   int col_or_row;

   if (T.wasReformed(col_or_row))
   {
      cout << "Transport task was transformed into close form." << endl;

      if (col_or_row == 1)
      {
         cout << "Right column is fictitious." << endl;
      }
      else
      {
         cout << "Lowest row is fictitious." << endl;
      }
   }
}

int main( int argc, char *argv[] )
{
   /*if (argc < 2)
      exit(0);

   if (argc == 2)*/
   Work("tests/transport1.txt", "res.txt");
}