#include <vector>
#include <cmath>

#include "comdef.h"
#include "matrix.h"
#include "transport_task.h"

#include "potential_solver.h"

using namespace std;

PotentialSolver::PotentialSolver( TransportTask &T ) : T(T)
{
}

double PotentialSolver::solve( Matrix &res )
{
   double res_value = -1;
   Matrix base(T.n, T.m);

   base.initValue(free_cell);

   _northWestCornerMethod(base);

   base.setSpecialOutput(free_cell);

   while (true)
   {
      int cur_eq = 0;
      vector<int> alpha_numbers, betha_numbers;
      Matrix tmp(T.n + T.m, T.n + T.m);
      vector<double> tmp_b;
      
      tmp_b.resize(T.n + T.m);

      tmp.initZero();

      for (int j = 0; j < base.n(); j++)
         for (int i = 0; i < base.m(); i++)
         {
            if (base[j][i] != free_cell)
            {
               alpha_numbers.push_back(i);
               betha_numbers.push_back(j);

               tmp_b[cur_eq] = T._price[j][i];
               tmp[j][cur_eq] = 1.0;
               tmp[i + T.n][cur_eq++] = -1.0;
            }
         }

      tmp[T.n][cur_eq] = 1.0;
      tmp_b[cur_eq] = 0.0;

      _alphaMatr = base;
      vector<double> alpha, betha;
      bool found = true;

      betha.resize(T.n);
      alpha.resize(T.m);

      _findPotentials(tmp, tmp_b, alpha, betha);

      for (int j = 0; j < _alphaMatr.n(); j++)
         for (int i = 0; i < _alphaMatr.m(); i++)
         {
            if (_alphaMatr[j][i] == free_cell)
            {
               _alphaMatr[j][i] = betha[j] - alpha[i] - T._price[j][i];

               if (_alphaMatr[j][i] > 0 && fabs(_alphaMatr[j][i]) > eps)
               {
                  found = false;
               }
            }
            else
            {
               _alphaMatr[j][i] = free_cell;
            }
         }

      _alphaMatr.setSpecialOutput(free_cell);

      if (found)
         break;
      
      int i_max, j_max;
      double alpha_max = -1;

      for (int j = 0; j < _alphaMatr.n(); j++)
         for (int i = 0; i < _alphaMatr.m(); i++)
         {
            if (_alphaMatr[j][i] > 0 && fabs(_alphaMatr[j][i]) > eps)
            {
               if (_alphaMatr[j][i] > alpha_max)
               {
                  alpha_max = _alphaMatr[j][i];
                  i_max = i;
                  j_max = j;
               }
            }
         }

      Matrix visited(T.n, T.m);
      visited.initZero();
      _cycle.clear();

      _findCycle(visited, base, i_max, j_max, i_max, j_max, true);

      vector<IntPair> positive, negative;

      for (int i = 0; i != _cycle.size(); i++)
      {
         if (i % 2 == 0)
            positive.push_back(_cycle[i]);
         else
            negative.push_back(_cycle[i]);
      }

      int i_min, j_min;
      double negative_min = (double)(1 << 24);

      for (int i = 0; i != negative.size(); i++)
      {
         if (base[negative[i].j][negative[i].i] < negative_min)
         {
            negative_min = base[negative[i].j][negative[i].i];
            i_min = negative[i].i;
            j_min = negative[i].j;
         }
      }

      for (int i = 0; i != negative.size(); i++)
         base[negative[i].j][negative[i].i] -= negative_min;

      for (int i = 0; i != positive.size(); i++)
         base[positive[i].j][positive[i].i] += negative_min;

      base[j_max][i_max] = negative_min;
      base[j_min][i_min] = free_cell;

      //cout << base << endl;
   }

   res_value = 0;

   res = base;

   for (int j = 0; j != T.n; j++)
      for (int i = 0; i != T.m; i++)
      {
         if (res[j][i] != free_cell)
            res_value += T._price[j][i] * res[j][i];
      }

   return res_value;
}

void PotentialSolver::_northWestCornerMethod( Matrix &base )
{
   int i = 0, j = 0;
   TransportTask tmp = T;

   while (true)
   {
      if (tmp._stock[i] <= tmp._necessity[j])
      {
         base[j][i] = tmp._stock[i];
         tmp._necessity[j] -= tmp._stock[i];
         i++;
      }
      else
      {
         base[j][i] = tmp._necessity[j];
         tmp._stock[i] -= tmp._necessity[j];
         j++;
      }

      if (i == T.m || j == T.n)
         break;
   }
}

void PotentialSolver::_findPotentials( Matrix &A, vector<double> &b, vector<double> &alpha, vector<double> &betha )
{
   vector<double> res(A.m());

   A.solveSystem(b, res);

   for (int i = 0; i != betha.size(); i++)
      betha[i] = res[i];

   for (int i = 0; i != alpha.size(); i++)
      alpha[i] = res[i + betha.size()];
}

bool PotentialSolver::_findCycle( Matrix &visited, Matrix &sol, int i1, int j1, int i2, int j2, bool row_or_col )
{
   _cycle.push_back(IntPair(i1, j1));
   visited[j1][i1] = 1;

   if ((i1 == i2 || j1 == j2) && _cycle.size() > 3)
      return true;

   //True if going thorough column
   if (row_or_col)
   {
      for (int i = 0; i < T.m; i++)
      {
         /*if (i == i2 && j1 == j2 && _cycle.size() > 3)
            return true;*/

         if (i != i1)
            if (sol[j1][i] != free_cell && !visited[j1][i])
            {
               if (_findCycle(visited, sol, i, j1, i2, j2, !row_or_col))
                  return true;
            }
      }
   }
   else
   {
      for (int j = 0; j < T.n; j++)
      {
         /*if (j == j2 && i1 == i2 && _cycle.size() > 3)
            return true;*/

         if (j != j1)
            if (sol[j][i1] != free_cell && !visited[j][i1])
            {
               if (_findCycle(visited, sol, i1, j, i2, j2, !row_or_col))
                  return true;
            }
      }
   }

   visited[j1][i1] = 0;

   _cycle.pop_back();

   return false;
}