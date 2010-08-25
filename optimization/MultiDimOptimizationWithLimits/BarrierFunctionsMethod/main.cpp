#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

#include "md_task.h"
#include "barrier_functions_solver.h"
#include "zoutendijk_solver.h"

/************************************************************************/
/*  Task data (bound functions) Prepare task is in *.inc file           */
/************************************************************************/

#include "tests/task1.inc"

/************************************************************************/

void Work( char *OutFileName )
{
   double _eps = 0.1;

   if (OutFileName != 0)
      freopen(OutFileName, "wt", stdout);

   try
   {
      for (int i = 0; i < 6; i++)
      {
         MdTask T;
         PrepareTask(T);

         BarrierFunctionsSolver solver(T, 10.0, 0.1);

         solver.setEps(_eps);
         
         double res_value;
         Vector res_vec;
         
         solver.resetIterCounter();
         /*solver.setDebugOutput();*/

         res_value = solver.solve(res_vec);

         cout << fixed;
         
         printf("Eps: %e\n", _eps);
         cout << "Min: " << res_value << endl;
         cout << "Solution: " << endl << res_vec << endl;
         cout << "Iterations: " << solver.getIterCounter() << endl << endl;

         _eps /= 10;

         cout << endl;
      }
   }
   catch (...)
   {
      puts("Something bad occurred.");
   }
}

int main( void )
{
   //Work(0);
   Work("log.txt");
}