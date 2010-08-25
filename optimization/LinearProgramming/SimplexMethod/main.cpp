#include <fstream>

#include "simplex_solver.h"
#include "lp_task.h"

using namespace std;

LpTask direct;
Vector x_star, y_star;

int iter;

void Work( char *FileName, char *OutFileName )
{
   ifstream f(FileName);
   LpTask a;

   f >> a;

   f.close();
   
   Vector res;
   SimplexSolver solver(a);


   if (OutFileName != 0)
      freopen(OutFileName, "wt", stdout);

   double result = solver.solve(res);

   if (iter == 0)
   {
      direct = a;
      x_star = res;
   }
   else
   {
      y_star.resize(direct.m);

      for (int i = 0; i != y_star.size(); i++)
      {
         y_star[i] = res[i] - res[i + direct.m];
      }
   }

   iter++;


   
   cout << "min: " << result << endl;
   cout << "vector: " << res << endl;

   cout << endl;
}

int main( int argc, char *argv[] )
{
/*Work("tests/direct3.txt", 0);*/

   //Work("tests/transp_simplex1.txt", "log_transport.txt");
   
   Work("tests/direct1.txt", "log_direct.txt");
   /*Work("tests/dual1.txt", "log_dual.txt");

   cout << direct.c << endl;
   cout << direct.A << endl;
   cout << x_star << endl;
   cout << y_star << endl;

   cout << direct.c - y_star * direct.A << endl;
   cout << (direct.c - y_star * direct.A) * x_star << endl;


   /*Work("tests/direct2.txt", 0);
   Work("tests/dual2.txt", 0);*/

   if (argc < 2)
      return 0;
 
}