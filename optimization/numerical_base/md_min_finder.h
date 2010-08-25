#ifndef _md_min_finder_h
#define _md_min_finder_h

#include <vector>
#include <iostream>

using namespace std;

class Matrix;
class Vector;

ostream & operator<<( ostream &os, vector<double> &v );

class MdMinFinder 
{
public:

   class AdditionalFunc : public SimpleFunc 
   {
   public:

      AdditionalFunc( vector<double> &u, vector<double> &p, double (&func)( vector<double> &x ) );

      virtual double operator()( double &x );

   private:

      double (&_func)( vector<double> &x );
      vector<double> &_u, &_p;
   };

   enum METHOD
   {
      STEEPEST_DESCENT,
      CONJUGATE_GRADIENT,
      NEWTON_METHOD,
      GRADIENT_MODIFICATION
   };
   
   MdMinFinder( int dim, double (&func)( vector<double> &var ), void (&_grad)( vector<double> &var, vector<double> &res ),
      void (&_grad2)( Matrix &m, vector<double> &var ), vector<double> &begin_point );

   double find( vector<double> &x, METHOD m, int &iter_num );

   void setEps( double new_eps );

   ~MdMinFinder();

private:

   METHOD _m;
   int _dim;
   int _renewal_step;
   int _accum;

   double _alpha;
   double _betha;
   double _eps;
   double _newtonEps;

   AdditionalFunc a_f;
   vector<double> _cur_u;
   vector<double> _cur_p;

   bool _next_p();
   bool _next_p_Newton();
   bool _next_u( double alpha );

   double _find( vector<double> &x, int &iter_num );


   void (&_grad2)( Matrix &m, vector<double> &var );
   double (&_func)( vector<double> &var );
   void (&_grad)( vector<double> &var, vector<double> &res );
};

#endif /* _md_min_finder_h_ */