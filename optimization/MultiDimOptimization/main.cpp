#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdio>

#include "vector.h"
#include "matrix.h"
#include "function.h"
#include "od_min_finder.h"
#include "modif_min_finder.h"

FUNC(Func1, x[0] * x[0] + 2 * x[1] * x[1] + exp(x[0] * x[0] + x[1] * x[1]));
VECTOR_FUNC(Grad1, res[0] = 2 * x[0] + 2 * x[0] * exp(x[0] * x[0] + x[1] * x[1]);
                   res[1] = 4 * x[1] + 2 * x[1] * exp(x[0] * x[0] + x[1] * x[1]))
MATRIX_FUNC(Hesse1, double tmp = exp(x[0] * x[0] + x[1] * x[1]); 
                    res[0][0] = 2 + 2 * tmp + 4 * x[0] * x[0] * tmp;
                    res[1][1] = 4 + 2 * tmp + 4 * x[1] * x[1] * tmp;

                    res[0][1] = 4 * x[0] * x[1] * tmp;
                    res[1][0] = res[0][1];)

Func1 func1;
Grad1 grad1;
Hesse1 hesse1;

void Work()
{
   double _eps;

   //If lambda is equal to 0 - we use only hesse(x0)
   //if lambda is equal to 1 - we use unly hesse(x_star)
   for (double lambda = 0; lambda <= 1.0; lambda += 0.1)
   {
      char FileName[100];

      sprintf(FileName, "report%i.txt", (int)(lambda * 10));
      freopen(FileName, "wt", stdout);
      _eps = 0.1;

      cout << "lambda = " << setprecision(1) << 0.9 << endl << endl << fixed;

      for (int i = 0; i < 4; i++)
      {
         int iterations;
         double res_value;
         ModifMinFinder finder(lambda, func1, grad1, hesse1);
         Vector x0(2), res;

         x0.initValue(0.3);

         finder.setEps(_eps);

         res_value = finder.find(x0, res);

         iterations = finder.getIterCount();

         printf("%e\n", _eps);
         cout << fixed;
         cout << "Descent modification" << endl << setprecision(2 * i + 1) << res_value 
            << endl << res << endl << "Iterations: " << iterations << endl << endl;

         finder.reset();

         _eps /= 100;
      }
   }
}

int main( int argc, char *argv[] )
{
   Work();
}