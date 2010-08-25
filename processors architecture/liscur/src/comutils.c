#include <time.h>

#include "comutils.h"

void swap( int *a, int *b )
{
  int c = *a;

  *a = *b;
  *b = c;
}

qword getTime()
{
  struct timespec t;
  
  clock_gettime(CLOCK_REALTIME, &t);
  return (t.tv_nsec + t.tv_sec * 1000000000ULL) / 1000ULL; //to secs
}

