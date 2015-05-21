#include <iostream>
#include <stdlib.h>

#include "darray.h"


char *sformat="%10.4lg";


void ErrMsg(char *msg, char *fun)
{
  printf("[%s] :: %s\n",fun,msg);
  exit(EXIT_FAILURE);
}

//---------------------------------------------------------------------------------------------------

/* output of the array to stdout */
template <class T, int rank>
std::ostream &operator<<(std::ostream &outp, DArray<T,rank> &A)
{
  int i,imax;

  imax = A.Length(1);

  printf("[ ");
  for (i=1; i<=imax; i++) {
    printf(sformat,A(i));          // %6.4le
    if (i<imax)
      printf("  ");
  }
  printf(" ]\n");

  return outp;
}


/*
ostream &operator<<(ostream &outp, DMatrix &A)
{
  int i,j,imax,jmax;

  imax = A.Length(1);
  jmax = A.Length(2);

  for (i=1; i<=imax; i++) {
    printf("[ ");
    for (j=1; j<=jmax; j++) {
      printf(sformat,A(i,j));
      if (j<jmax)
        printf("  ");
    }
    printf(" ]\n");
  }

  return outp;
}


ostream &operator<<(ostream &outp, DArray3 &A)
{
  int i,j,k,imax,jmax,kmax;

  imax = A.Length(1);
  jmax = A.Length(2);
  kmax = A.Length(3);

  for (k=1; k<=kmax; k++) {
    printf("(*,*,%d) = \n",k);
    for (i=1; i<=imax; i++) {
      printf("[ ");
      for (j=1; j<=jmax; j++) {
        printf(sformat,A(i,j,k));
        if (j<jmax)
          printf("  ");
      }
      printf(" ]\n");
    }
  }

  return outp;
}


ostream &operator<<(ostream &outp, DArray4 &A)
{
  int i,j,k,l,imax,jmax,kmax,lmax;

  imax = A.Length(1);
  jmax = A.Length(2);
  kmax = A.Length(3);
  lmax = A.Length(4);

  for (l=1; l<=lmax; l++) {
    for (k=1; k<=kmax; k++) {
      printf("(*,*,%d,%d) = \n",k,l);
      for (i=1; i<=imax; i++) {
        printf("[ ");
        for (j=1; j<=jmax; j++) {
          printf(sformat,A(i,j,k,l));
          if (j<jmax)
            printf("  ");
        }
        printf(" ]\n");
      }
    }
  }

  return outp;
}

*/

//---------------------------------------------------------------------------------------------------

// length of a vector
double vlength(DVector &vect)
{
  double vlength;
  int len,i;

  len = vect.Length(1);
  vlength = 0;
  for (i=1; i<=len; i++) {
    vlength += pow(vect(i),2);
  }
  vlength = sqrt(vlength);

  return vlength;
}


// supremum - nejblizsi vyssi cele cislo:  sup(1.3)=2, sup(5.8)=2, sup(6)=6
int sup(double value)
{
  int fvalue;

  fvalue = (int) floor(value);
  if (value>fvalue)
    return fvalue+1;
  else
    return fvalue;
}


// implementation of the quicksort algorithm
void quicksort(DMatrix &list, int lo, int hi)
{
  int i=lo, j=hi;
  double x=list((lo+hi)/2,1);

  while (i<=j)
  {
    while (list(i,1)<x) i++;
    while (list(j,1)>x) j--;

    if (i<=j) {
      swap(list(i,1),list(j,1));
      swap(list(i,2),list(j,2));
      i++;
      j--;
    }
  }

  // rekurze
  if (lo<j) quicksort(list,lo,j);
  if (i<hi) quicksort(list,i,hi);
}


// trideni podle i-teho sloupce
void sort(DMatrix &mold, int start, int end, int col, IVector &indexes)
{
  DMatrix list;
  int row,nrows;

  nrows = end-start+1;
  list.Allocate(nrows,2);
  for (row=1; row<=nrows; row++) {
    list(start+row-1,1) = mold(row,col);          // value
    list(start+row-1,2) = row;                    // number of row
  }

  quicksort(list,start,end);
  for (row=1; row<=nrows; row++)
    indexes(row) = (int) list(row,2);
}
