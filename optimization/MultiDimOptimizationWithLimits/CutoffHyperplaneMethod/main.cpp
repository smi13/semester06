#include "md_task.h"
#include "cutoffplane_solver.h"
#include "matrix.h"
#include "lp_task.h"
#include "simplex_solver.h"

/************************************************************************/
/*  Task data (bound functions) Prepare task is in *.inc file           */
/************************************************************************/

#include "tests/task2.inc"
//#include "tests/task2_S0.inc"

/************************************************************************/

LpTask S0;

LpTask PrepareLpTask( void )
{
   LpTask tmp_lptask;
   Matrix A;

   Vector tmp(6);
   
   tmp[0] = 1.0;
   tmp[1] = -1.0;
   tmp[2] = 0.0;
   tmp[3] = 0.0;
   tmp[4] = 0.0;
   tmp[5] = 0.0;

   A << tmp;

   tmp[0] = 0.0;
   tmp[1] = 0.0;
   tmp[2] = 1.0;
   tmp[3] = -1.0;
   tmp[4] = 0.0;
   tmp[5] = 0.0;

   A << tmp;

   tmp[0] = 0.0;
   tmp[1] = 0.0;
   tmp[2] = 0.0;
   tmp[3] = 0.0;
   tmp[4] = -1.0;
   tmp[5] = 1.0;

   A << tmp;

   tmp[0] = -1.0;
   tmp[1] = 1.0;
   tmp[2] = 0.0;
   tmp[3] = 0.0;
   tmp[4] = 0.0;
   tmp[5] = 0.0;

   A << tmp;

   tmp[0] = 0.0;
   tmp[1] = 0.0;
   tmp[2] = -1.0;
   tmp[3] = 1.0;
   tmp[4] = 0.0;
   tmp[5] = 0.0;

   A << tmp;

   tmp[0] = 0.0;
   tmp[1] = 0.0;
   tmp[2] = 0.0;
   tmp[3] = 0.0;
   tmp[4] = 1.0;
   tmp[5] = -1.0;

   A << tmp;

   Matrix E(6);

   E.initUnit();

   A |= E;

   tmp_lptask.A = A;

   Vector b(6);

   b.initValue(10.0);

   b[b.size() - 2] = 160.0;
   b[b.size() - 1] = -160.0;

   tmp_lptask.b = b;

   Vector c(12);

   c.initZero();

   c[2] = 1;
   c[5] = -1;

   tmp_lptask.c = c;

   tmp_lptask.m = tmp_lptask.A.m();
   tmp_lptask.n = tmp_lptask.A.n();

   return tmp_lptask;
   //cout << S;
}

void Work( char *OutFileName )
{
   double _eps = 1e-1;

   if (OutFileName != 0)
      freopen(OutFileName, "wt", stdout);

   try
   {
      for (int i = 0; i < 3; i++)
      {
         MdTask T;
         LpTask S0;
         PrepareTask(T);
         S0 = PrepareLpTask();

         CutOffPlaneSolver solver(T, S0);

         solver.setEps(_eps);

         double res_value;
         Vector res_vec;

         solver.resetIterCounter();
         res_value = solver.solve(res_vec);

         //GenerateTest("test.txt", T, res_vec);

 /*        cout << fixed;
         printf("Eps: %e\n", _eps);
         cout << "Min: " << res_value << endl;
         cout << "Solution: " << endl << res_vec << endl;
         cout << "Iterations: " << solver.getIterCounter() << endl << endl;*/

         _eps /= 10;
      }
   }
   catch (...)
   {
      puts("Something bad occurred.");
   }
}


int main( void )
{
   Work("report.txt");
   return EXIT_SUCCESS;
}