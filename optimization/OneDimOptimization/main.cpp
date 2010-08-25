#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdio>
#include <time.h>
#include <iostream>
#include <iomanip>

using namespace std;

#include "od_min_finder.h"

int funccounter;

double a = 0.2, b = 1.0;

SIMPLE_FUNC(Func1, 10 * x * log(x) - x * x * 0.5)

Func1 func1;

void GenerateReport( char *FileName )
{
   double eps = 1e-1;
   OdMinFinder finder(func1, a, b);

   if (FileName != 0)
      freopen(FileName, "wt", stdout);

   for (int i = 0; i < 4; i++)
   {
      int iter;
      double x, res, left, right;

      printf("%i) eps: %e\n\n", i + 1, eps);

      funccounter = 0;
      finder.resetCounters();
      finder.setEps(eps);
      finder.setBorders(a, b);

      res = finder.find(x, OdMinFinder::DICHOTOMY);

      finder.getBorders(left, right);
      iter = finder.getIterCounter();

      cout << fixed;

      cout << "Dichotomy" << endl << "Min value: " << setprecision(i + 1) << res << endl <<
         "Argument: " << setprecision(i + 1) << x << endl << "Function computations: " << 
         finder.getFuncCounter() << endl << "Interval length: " << setprecision(7) << (right - left) << endl 
         << "Theoretical value: " << setprecision(7) << pow(0.5, iter) * (b - a) << endl << endl;

      funccounter = 0;
      finder.resetCounters();
      finder.setEps(eps);
      finder.setBorders(a, b);

      res = finder.find(x, OdMinFinder::GOLDEN_RATIO);

      finder.getBorders(left, right);
      iter = finder.getIterCounter();

      cout << fixed;

      cout << "Golden ratio" << endl << "Min value: " << setprecision(i + 1) << res << endl <<
         "Argument: " << setprecision(i + 1) << x << endl << "Function computations: " << 
         finder.getFuncCounter() << endl << "Interval length: " << setprecision(7) << (right - left) << endl 
         << "Theoretical value: " << setprecision(7) << pow(0.62, iter) * (b - a) << endl << endl;

      printf("\n\n");
      
      eps /= 10;
   }
}

int main()
{
   GenerateReport("report.txt");
}