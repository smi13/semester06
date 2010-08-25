#include "comutils.h"

#ifdef _LP64 /* Just for testing on PC */
void c_put_pixel( char *data, int x, int y, int bytes_per_line )
{
  *(data + y * bytes_per_line + 4 * x) = -1;
}
#else
void c_put_pixel( char *data, int x, int y, int bytes_per_line )
{
  *(data + y * bytes_per_line + x) = -1;
}
#endif /* _LP64 */

void c_put_line( int x0, int y0, int x1, int y1, char *data, int bytes_per_line )
{
   int steep = abs(y1 - y0) > abs(x1 - x0), tmp;

   if (steep)
   {
      swap(&x0, &y0);
      swap(&x1, &y1);
   }

   int deltax = abs(x1 - x0),
      deltay = abs(y1 - y0),
      error = deltax / 2,
      ystep = y0 < y1 ? 1 : -1,
      xstep = x0 > x1 ? -1 : 1,
      y = y0, x;

   for (x = x0; x0 > x1 ? x >= x1 : x <= x1; x += xstep)
   {
      if (steep)
         c_put_pixel(data, y, x, bytes_per_line); 
      else 
         c_put_pixel(data, x, y, bytes_per_line);

      error -= deltay;

      if (error < 0)
      {
         y += ystep;
         error += deltax;
      }
   }
}	
