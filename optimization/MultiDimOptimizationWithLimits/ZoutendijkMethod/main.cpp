#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

#include "enumerator.h"
#include "function.h"
#include "zoutendijk_solver.h"
#include "vector.h"

#include "codes_enumerator.h"

/************************************************************************/
/*  Task data (bound functions) Prepare task is in *.inc file           */
/************************************************************************/

#include "tests/task6.inc"

/************************************************************************/

void GenerateTest( char *OutFileName, MdTask &T, Vector &x_star )
{
   Matrix RectA(T._phi.size(), x_star.size() + 1), A(x_star.size() + 1);
   Vector b(x_star.size() + 1);

   b[0] = 0.0;

   if (OutFileName != 0)
      freopen(OutFileName, "wt", stdout);

   cout << fixed;

   cout << setw(10) << setprecision(11);

   //cout << "Phi: "; 
   for (int i = 0; i != T._phi.size(); i++)
   {
      RectA[i][0] = T._phi[i]->eval(x_star);
      //cout << RectA[0][i] << " ";
   }

   //cout << 0 << endl;

   //cout << "Matrix: " << endl;

   for (int i = 0; i != x_star.size(); i++)
   {
      for (int j = 0; j != T._phi.size(); j++)
      {
         RectA[j][i + 1] = (T._phi_grad[j]->eval(x_star))[i];
         //cout << RectA[j][i + 1] << " ";
      }

      b[i + 1] = -(T._phi0_grad->eval(x_star))[i];
      //cout << b[i + 1] << " ";

      cout << endl;
   }

   cout << RectA << endl;
   cout << b << endl << endl;

   Enumerator col_enum(RectA.n(), RectA.m());
   bool isOver = false;

   vector<int> cur_perm;
   vector<double> b_vec;

   b_vec.resize(b.size());

   for (int i = 0; i != b_vec.size(); i++)
   {
      b_vec[i] = b[i];
   }

   cur_perm.resize(RectA.m());

   for (int i = 0; i != cur_perm.size(); i++)
      cur_perm[i] = i;

   bool timeToEnd = false;

   while (true) 
   {
      Matrix tmp;

      cout << "Current permutation: " << endl;
      for (int i = 0; i != 3; i++)
      {
         cout << cur_perm[i] << " ";
      }

      cout << endl;

      for (int i = 0; i != RectA.m(); i++)
      {
         tmp << RectA[cur_perm[i]];
      }

      if (!tmp.isDegenerate())
      {
         vector<double> x;

         x.resize(x_star.size() + 1);

         A = tmp;

         A.solveSystem(b_vec, x);

         cout << endl << A << endl;

         Vector Y(T._phi.size());

         for (int i = 0; i != RectA.m(); i++)
            Y[cur_perm[i]] = x[i];

         cout << "Y: " << endl << Y << endl << endl;
      }

      cur_perm = col_enum.next(isOver);

      if (isOver)
         break;
   } 

   cout << endl;
}

void Work( char *OutFileName )
{
   double _eps = 0.1;

   if (OutFileName != 0)
      freopen(OutFileName, "wt", stdout);

   try
   {
      for (int i = 0; i < 3; i++)
      {
         MdTask T;
         PrepareTask(T);

         Vector tmp_vec(2);

         tmp_vec[0] = 0.8228754809496618;
         tmp_vec[1] = 0.911437740474831;

         GenerateTest("test.txt", T, tmp_vec);

         ZoutendijkSolver solver(T);

         solver.setEps(_eps);
         
         double res_value;
         Vector res_vec;
         
         solver.resetIterCounter();
         res_value = solver.solve(res_vec);

//         GenerateTest("test.txt", T, res_vec);

         /*cout << fixed;
         printf("Eps: %e\n", _eps);
         cout << "Min: " << res_value << endl;
         cout << "Solution: " << endl << res_vec << endl;
         cout << "Iterations: " << solver.getIterCounter() << endl << endl;*/

         _eps /= 10;

         /*Additional results output*/

         /*for (int i = 0; i < T._phi.size(); i++)
            cout << T._phi[i]->eval(res_vec) << " ";

         cout << endl;

         cout << T._phi0_grad->eval(res_vec) << endl;
         for (int i = 0; i < T._phi.size(); i++)
            cout << T._phi_grad[i]->eval(res_vec) << endl;*/

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