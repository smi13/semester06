#include <cmath>
#include <iomanip>

#include "vector.h"
#include "number_set.h"
#include "comdef.h"
#include "matrix.h"

using namespace std;

istream & operator>>( istream &Istr, Matrix &A )
{
   int n, m;

   Istr >> m;
   Istr >> n;

   A._storage.resize(n);

   for (int i = 0; i < m; i++)
      for (int j = 0; j < n; j++)
      {
         double a;
         Istr >> a;
         A._storage[j].pushBack(a);
      }

   return Istr;
}

ostream & operator<<( ostream &Ostr, const Matrix &A )
{
   int n = A._storage.size(), m = A._storage[0].size();

   for (int i = 0; i < m; i++)
   {
      for (int j = 0; j < n; j++)
      {
         if (A._specialoutput)
         {
            if (A._storage[j][i] == A._val)
               Ostr << setw(7) << "free" << ' ';
            else
               Ostr << setw(7) << A._storage[j][i] << ' ';
         }
         else
            Ostr << setw(7) << A._storage[j][i] << ' ';
      }
      Ostr << endl;
   }
   
   return Ostr;
}

int Matrix::m() const
{
   if (_storage.size() == 0)
      return 0;
   return _storage[0].size();
}

int Matrix::n() const
{
   return _storage.size();
}

Vector & Matrix::operator[]( const int i )
{
   return _storage[i];
}

Vector Matrix::operator[]( const int i ) const
{
   return _storage[i];
}

void Matrix::copy( const Matrix &A )
{
   for (int i = 0; i != _storage.size(); i++)
      _storage[i] = A._storage[i];
}

void Matrix::initUnit()
{
   int n = _storage.size(), m = _storage[0].size();

   for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
      {
         if (i == j)
            _storage[i][j] = 1.0;
         else
            _storage[i][j] = 0;
      }
}

Matrix::Matrix( int m )
{
   _storage.resize(m);

   for (int i = 0; i != _storage.size(); i++)
   {
      _storage[i].resize(m);
   }

   _specialoutput = false;
}

Matrix::Matrix( int n, int m )
{
   _storage.resize(n);

   for (int i = 0; i != _storage.size(); i++)
   {
      _storage[i].resize(m);
   }

   _specialoutput = false;
}

Matrix::Matrix()
{
   _specialoutput = false;
}

Matrix & Matrix::operator|=( const Matrix &A )
{
   for (int i = 0; i != A._storage.size(); i++)
      _storage.push_back(A._storage[i]);

   return *this;
}

Matrix Matrix::operator|( const Matrix &A ) const
{
   Matrix B(*this);

   return B |= A;
}

void Matrix::mulRow( int i, const double a )
{
   for (int j = 0; j != _storage.size(); j++)
      _storage[j][i] *= a;
}

void Matrix::operator<<( Vector &v )
{
   _storage.push_back(v);
}

bool Matrix::isDegenerate()
{
   Matrix B;

   return !this->buildInverse(B);
}

Matrix Matrix::operator*( const double a ) const
{
   Matrix B = *this;

   for (int i = 0; i != B._storage.size(); i++)
   {
      B[i] *= a;
   }

   return B;
}

Vector Matrix::operator*( const Vector &v ) const
{
   Vector res;

   res.resize(this->m());

   for (int i = 0; i < this->m(); i++)
   {
      Vector cur_vec;

      cur_vec.resize(this->n());

      for (int j = 0; j < this->n(); j++)
         cur_vec[j] = _storage[j][i];

      res[i] = cur_vec * v;
   }

   return res;
}

Matrix Matrix::operator*( const Matrix &m ) const
{
   Vector cur_vec(this->n());
   Matrix res(m.n(), this->m());

   for (int i = 0; i < this->m(); i++)
      for (int j = 0; j < m.n(); j++)
      {
         for (int k = 0; k < this->n(); k++)
         {
            cur_vec[k] = _storage[k][i];
         }
         res[j][i] = cur_vec * m[j];
      }

   //DEBUG output
   //cout << (*this) << endl;
   //cout << m << endl;
   //cout << res;

   return res;
}

Matrix & Matrix::operator*=( const double a )
{
   (*this) = (*this) * a;
   return *this;
}

Matrix & Matrix::operator*=( const Vector &v )
{
   return *this;
}

Matrix & Matrix::operator*=( const Matrix &m )
{
   return *this;
}

bool Matrix::buildInverse( Matrix &res )
{
   int n = this->n();
   int hold, I_pivot, J_pivot;
   double pivot, abs_pivot;
   vector<double> temp;
   vector<int> row, col;

   res = *this;

   temp.resize(n);
   row.resize(n);
   col.resize(n);

   for (int k = 0; k < n; k++)
   {
      row[k] = k;
      col[k] = k;
   }
   
   for (int k = 0; k < n; k++)
   {
      pivot = res[row[k]][col[k]];
      I_pivot = k;
      J_pivot = k;

      for (int i = k; i < n; i++)
      {
         for (int j = k; j < n; j++)
         {
            abs_pivot = fabs(pivot);

            if (fabs(res[row[i]][col[j]]) > abs_pivot)
            {
               I_pivot = i;
               J_pivot = j;
               pivot = res[row[i]][col[j]] ;
            }
         }
      }

      if (fabs(pivot) < eps)
      {
         return false;
      }

      hold = row[k];
      row[k] = row[I_pivot];
      row[I_pivot] = hold ;
      hold = col[k];
      col[k]= col[J_pivot];
      col[J_pivot] = hold ;
      
      res[row[k]][col[k]] = 1.0 / pivot;

      for (int j = 0; j < n; j++)
      {
         if (j != k)
         {
            res[row[k]][col[j]] = res[row[k]][col[j]] * res[row[k]][col[k]];
         }
      }

      for (int i = 0; i < n; i++)
      {
         if (k != i)
         {
            for (int j = 0; j < n; j++)
            {
               if (k != j)
               {
                  res[row[i]][col[j]] = res[row[i]][col[j]] - res[row[i]][col[k]] *
                     res[row[k]][col[j]] ;
               }
            }

            res[row[i]][col [k]] = -res[row[i]][col[k]] * res[row[k]][col[k]] ;
         }
      }
   }
   
   for (int j = 0; j < n; j++)
   {
      for (int i = 0; i < n; i++)
      {
         temp[col[i]] = res[row[i]][j];
      }

      for (int i = 0; i < n; i++)
      {
         res[i][j] = temp[i];
      }
   }

   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         temp[row[j]] = res[i][col[j]];
      }
      for(int j = 0; j < n; j++)
      {
         res[i][j] = temp[j];
      }
   }

   res.threshold();

   return true;
}

void Matrix::threshold()
{
   for (int i = 0; i < this->m(); i++)
      for (int j = 0; j < this->n(); j++)
         if (fabs(_storage[j][i]) < eps)
            _storage[j][i] = 0;
}

Matrix Matrix::operator()( const NumberSet &set )
{
   Matrix res;

   for (int i = 0; i < set.size(); i++)
   {
      
         res << ((*this).operator[])(set[i]);
   }

   return res;
}

Matrix Matrix::operator+( const Matrix &m ) const
{
   Matrix res(this->n(), this->m());

   for (int j = 0; j < this->n(); j++)
      for (int i = 0; i < this->m(); i++)
      {
         res[j][i] = _storage[j][i] + m[j][i];
      }

   return res;
}

bool Matrix::solveSystem( const vector<double> &_b, vector<double> &res )
{
   int i, j, k, m;
   double fMaxElem;
   double fAcc;
   vector<double> b = _b;

   Matrix tmp = *this;

   int nDim = _storage.size();

   for (k = 0; k < nDim - 1 ; k++) 
   {
      fMaxElem = fabs(tmp._storage[k][k]);
      m = k;
      for (i = k + 1; i < nDim; i++)
      {
         if (fMaxElem < fabs(tmp._storage[k][i]))
         {
            fMaxElem = fabs(tmp._storage[k][i]);
            m = i;
         }
      }

      if (m != k)
      {
         for (i = k; i < nDim; i++)
         {
            fAcc = tmp._storage[i][k];
            tmp._storage[i][k] = tmp._storage[i][m];
            tmp._storage[i][m] = fAcc;
         }

         fAcc = b[k];
         b[k] = b[m];
         b[m] = fAcc;
      }

      if (fabs(tmp._storage[k][k]) < eps) 
         return false; 

      for (j = k + 1; j < nDim; j++) 
      {
         fAcc = -tmp._storage[k][j] / tmp._storage[k][k];

         for (i = k; i < nDim; i++)
         {
            tmp._storage[i][j] = tmp._storage[i][j] + fAcc * tmp._storage[i][k];
         }

         b[j] = b[j] + fAcc * b[k]; 
      }
   }

   for (k = nDim - 1; k >= 0; k--)
   {
      res[k] = b[k];

      for (i = k + 1; i < nDim; i++)
      {
         res[k] -= (tmp._storage[i][k] * res[i]);
      }

      if (fabs(tmp._storage[k][k]) < eps)
         return false;

      res[k] = res[k] / tmp._storage[k][k];
   }

   return true;
}

void Matrix::initValue( const double val )
{
   for (int j = 0; j < this->n(); j++)
      for (int i = 0; i < this->m(); i++)
      {
         _storage[j][i] = val;
      }
}

void Matrix::initZero()
{
   initValue(0.0);
}

void Matrix::setSpecialOutput( int val )
{
   _specialoutput = true;
   _val = val;
}

void Matrix::resetSpecialOutput()
{
   _specialoutput = false;
}

void Matrix::replaceValue( const double &var1, const double &var2 )
{
   for (int j = 0; j < this->n(); j++)
      for (int i = 0; i < this->m(); i++)
      {
         if (fabs(_storage[j][i] - var1) < eps)
            _storage[j][i] = var2;
      }
}