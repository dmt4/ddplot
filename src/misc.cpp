#include <math.h>


// Calculation of the distance between two atoms given by vectors atom1 and atom2
double vDistance( double *v1, double *v2, int nitems=3 )
{
  double dist;
  int d;

  dist = 0;
  for (d=0; d<nitems; d++)
    dist += pow(v1[d]-v2[d],2);
  dist = sqrt(dist);

  return dist;
}


double vDotProduct( double v1[], double v2[], int nitems=3 )
{
  double res;
  int n;

  res = 0;
  for (n=0; n<nitems; n++)
    res += v1[n]*v2[n];

  return res;
}


