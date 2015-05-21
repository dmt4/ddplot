#ifndef DARRAY_H
#define DARRAY_H

#include <iostream>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>


/*------------------------------------------------------------------------------------------------
  constants
------------------------------------------------------------------------------------------------*/

const int MAXDIM = 4;      /* maximum number of dimensions supported by this class */


/*------------------------------------------------------------------------------------------------
  class DArray - declarations
------------------------------------------------------------------------------------------------*/

template <class T, int rank>
class DArray {
public:
  T *data;

  long items;           /* total number of items in the array */
  int dim;              /* dimension of the array (2 for matrix, 3 for tensor Tijk, etc. */
  int len[MAXDIM];      /* number of items along the specific directions of the array */
  int estep[MAXDIM];    /* jumps for "inflating" of the array (auto_enlarge must be true) */
  bool auto_enlarge;    /* is the enlarging of the structure permitted ? */

  DArray() { data = NULL; };      /* constructor without allocation */
  DArray(int ilen, ...);          /* contructor with allocation of space for data */
  DArray(DArray<T,rank> &t);      /* copy constructor */
  ~DArray();                      /* destructor */

  void Allocate(int ilen, ...);   /* allocation of the space for data */
  void Free();                    /* freeing the space occupied by data */

  T &operator()(int i, ...);

  DArray<T,rank> &operator+(DArray<T,rank> &t);
  DArray<T,rank> &operator+=(DArray<T,rank> &t);
  DArray<T,rank> &operator-(DArray<T,rank> &t);
  DArray<T,rank> &operator-=(DArray<T,rank> &t);
  DArray<T,rank> &operator=(T value);
  DArray<T,rank> &operator=(DArray<T,rank> &t);
  DArray<T,rank> &operator*(T value);
  DArray<T,rank> &operator*(DArray<T,rank> &A2);
  DArray<T,rank> &operator/(T value);

  void EnlargeStep(int istep, ...);

  // vyplneni pole hodnotou value
  void Fill(T value);

  // zjisteni delky tenzoru v dir-tem smeru
  int Length(int dir) { return len[dir-1]; }

  // zjisteni dimenze pole
  int Dim(void) { return dim; }

  // maximum item in an array
  double Max(DArray<T,rank> &arr);

  // zvetseni struktury podle skoku estep (auto_enlarge=true)
  void Resize(int i, ...);
};


/*------------------------------------------------------------------------------------------------
  class DArray - derived types
------------------------------------------------------------------------------------------------*/

// arrays with elements of type double
typedef DArray<double,1> DVector;
typedef DArray<double,2> DMatrix;
typedef DArray<double,3> DArray3;
typedef DArray<double,4> DArray4;

// arrays with elements of type int
typedef DArray<int,1> IVector;
typedef DArray<int,2> IMatrix;
typedef DArray<int,3> IArray3;
typedef DArray<int,4> IArray4;

// arrays with elements of type bool
typedef DArray<bool,1> BVector;
typedef DArray<bool,2> BMatrix;
typedef DArray<bool,3> BArray3;
typedef DArray<bool,4> BArray4;


/*------------------------------------------------------------------------------------------------
  class DArray - support functions (definitions in darray.cpp)
------------------------------------------------------------------------------------------------*/

void disp(DVector &A);
void disp(DMatrix &A);
void disp(DArray3 &A);
void disp(DArray4 &A);

// handling of error messages
void ErrMsg(char *msg, char *fun);

// velikost vektoru
double vlength(DVector &vect);

// supremum - nejblizsi vyssi cele cislo:  sup(1.3)=2, sup(5.8)=2, sup(6)=6
int sup(double value);

// trideni podle i-teho sloupce
void sort(DMatrix &mold, int start, int end, int col, IVector &indexes);


/*------------------------------------------------------------------------------------------------
  class DArray - constructors and destructors
------------------------------------------------------------------------------------------------*/

// construktor for an array of any rank
template <class T, int rank>
DArray<T,rank>::DArray(int ilen, ...)
{
  char *fun="DArray<T,rank> (constructor)";
  va_list args;
  int i;

  va_start(args,ilen);
  dim = rank;
  len[0] = items = ilen;
  for (i=1; i<rank; i++) {
    len[i] = va_arg(args,int);
    items *= len[i];
  }
  
  memset(&len[rank], 0, (MAXDIM-rank)*sizeof(int));
  va_end(args);

  auto_enlarge = false;
  memset(estep, 0, MAXDIM*sizeof(int));

  data = (T *) malloc(items*sizeof(T));
  if (data==NULL && items>0) 
    ErrMsg("Allocation of the space for data unsuccessful.", fun);
  Fill(0);
}


// copy constructor
template <class T, int rank>
DArray<T,rank>::DArray(DArray<T,rank> &t)
{
  char *fun="DArray<T,rank> (copy constructor)";
  dim = t.dim;
  items = t.items;
  auto_enlarge = t.auto_enlarge;

  memcpy(len, t.len, MAXDIM*sizeof(int));
  memcpy(estep, t.estep, MAXDIM*sizeof(int));

  data = (T *) malloc(t.items*sizeof(T));
  if (data==NULL && t.items>0) 
    ErrMsg("Allocation of the space for data unsuccessful.", fun);
  memcpy(data,t.data,t.items*sizeof(T));
}


// destructor
template <class T, int rank>
DArray<T,rank>::~DArray()
{
  Free();
}


// dynamic allocation during the run of the program
// - declare:   DArray<T,rank> A;  (don't know the extent yet)
// - allocate:  A.Allocate(...);
template <class T, int rank>
void DArray<T,rank>::Allocate(int ilen, ...)
{
  char *fun="DArray<T,rank>::Allocate";
  va_list args;
  int i;

  // if the array is already allocated, free it first (this is safe !)
  Free();

  va_start(args,ilen);
  dim = rank;
  len[0] = items = ilen;
  for (i=1; i<rank; i++) {
    len[i] = va_arg(args,int);
    items *= len[i];
  }

  memset(&len[rank], 0, (MAXDIM-rank)*sizeof(int));
  va_end(args);

  auto_enlarge = false;
  memset(estep, 0, MAXDIM*sizeof(int));

  data = (T *) malloc(items*sizeof(T));
  if (data==NULL && items>0) 
    ErrMsg("Allocation of the space for data unsuccessful.", fun);
}


// deallocation during the run of the program
template <class T, int rank>
void DArray<T,rank>::Free()
{
  if (data!=NULL) {
    free(data);
    data = NULL;
  }
}


/*------------------------------------------------------------------------------------------------
  class DArray - operators
------------------------------------------------------------------------------------------------*/

// reading/writing into the structure
template <class T, int rank>
T &DArray<T,rank>::operator()(int i, ...)
{
  char *fun="DArray<T,rank>::operator()";
  va_list args;
  int idx[MAXDIM];
  int ipar,mult,idx_data;

  // reading of the idx into the array
  va_start(args,i);
  idx[0] = i;
  for (ipar=1; ipar<dim; ipar++)
    idx[ipar] = va_arg(args,int);
  va_end(args);

  // checking to see whether we are not writing/reading beyond the boundaries of the array
  for (ipar=0; ipar<dim; ipar++) {
    // do we read/write inside the array ?
    if ((idx[ipar]<1 || idx[ipar]>len[ipar]) && !auto_enlarge)
      ErrMsg("Attempt to read/write outside an array.",fun);      
  }

  // enlarge structure
  if (auto_enlarge) {
    switch(dim) {
      case 1:
	if (idx[0]>len[0])
	  Resize(idx[0]);
	break;

      case 2:
	if (idx[0]>len[0] || idx[1]>len[1])
	  Resize(idx[0],idx[1]);
	break;

      case 3:
	if (idx[0]>len[0] || idx[1]>len[1] || idx[2]>len[2])
	  Resize(idx[0],idx[1],idx[2]);
	break;

      case 4:
	if (idx[0]>len[0] || idx[1]>len[1] || 
	    idx[2]>len[2] || idx[3]>len[3])
	  Resize(idx[0],idx[1],idx[2],idx[3]);
	break;
    }
  }

  // calculation of the index into the stack <data>
  mult = 1;
  idx_data = 0;
  for (ipar=0; ipar<dim; ipar++) {
    idx_data += (idx[ipar]-1)*mult;  // -1 because they are counted from 1
    mult *= len[ipar];
  }
  return data[idx_data];
}


// addition of arrays if they are of the same dimension and extent
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator+(DArray<T,rank> &t)
{
  char *fun="DArray<T,rank>::operator+";
  static DArray<T,rank> result = t;
  unsigned int i;

  if (this->dim!=t.dim || this->items!=t.items)
    ErrMsg("Addition of dimensionally incompatible arrays.",fun);

  // can I make this faster (assembler code) ?
  for (i=0; i<items; i++)
    result.data[i] = data[i]+t.data[i];

  return result;
}


// operator of addition to the target
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator+=(DArray<T,rank> &t)
{
  *this = *this+t;
  return *this;
}


// operation of subtraction
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator-(DArray<T,rank> &t)
{
  char *fun="DArray<T,rank>::operator-";
  static DArray<T,rank> result = t;
  unsigned int i;

  if (this->dim!=t.dim || this->items!=t.items)
    ErrMsg("Subtraction of dimensionally incompatible arrays.",fun);

  // can I make this faster (assembler code) ?
  for (i=0; i<items; i++)
    result.data[i] = data[i]-t.data[i];

  return result;
}


// operator of subtraction from the target 
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator-=(DArray<T,rank> &t)
{
  *this = *this-t;
  return *this;
}


// fill the entire array with a given value
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator=(T value)
{ 
  Fill(value); 
  return *this; 
}


// copy operator (copies t to *this)
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator=(DArray<T,rank> &t)
{
  char *fun="DArray<T,rank>::operator=(DArray)";

  // important - free *this before the new allocation
  Free();

  dim = t.dim;
  items = t.items;
  auto_enlarge = t.auto_enlarge;
  memcpy(len, t.len, MAXDIM*sizeof(int));
  memcpy(estep, t.estep, MAXDIM*sizeof(int));

  data = (T *) malloc(t.items*sizeof(T));
  if (data==NULL && t.items>0) {
    printf("%d | %d %d %d %d | %d\n",dim,len[0],len[1],len[2],len[3],t.items);
    ErrMsg("Allocation of the space for data unsuccessful",fun);
  }
  memcpy(data,t.data,t.items*sizeof(T));

  return *this;
}


// operator of multiplication of the same array by a number (from left !)
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator*(T value)
{
  unsigned int i;

  for (i=0; i<items; i++)
    data[i] *= value;
  return *this;
}


// Operator of multiplication of two arrays (dimensionally compatible !)
// This is supposed to be used for matrices only - not too general yet.
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator*(DArray<T,rank> &A2)
{
  char *fun="DArray<T,rank>::operator*";
  static DArray<T,rank> A1 = *this;
  static DArray<T,rank> target;    // the allocation is below
  double sum;
  int new_len[4];
  int new_dim;
  int rmax1,cmax1,cmax2,d,dmax;
  int row,col,idx;

  // is the multiplication possible ?
  if (A1.Length(A1.dim)!=A2.Length(1))
    ErrMsg("Multiplication of dimensionally incompatible arrays.",fun);

  // the dimension of the resulting array
  new_dim = 0;
  dmax = A1.Dim()-1;
  for (d=1; d<=dmax; d++) {
    new_len[new_dim] = A1.Length(d);
    new_dim++;
  }

  dmax = A2.Dim();
  for (d=2; d<=dmax; d++) {
    new_len[new_dim] = A2.Length(d);
    new_dim++;
  }

  // alokace vysledne matice
  switch (new_dim) {
    case 1:
      target.Allocate(new_len[0]);
      break;

    case 2:
      target.Allocate(new_len[0],new_len[1]);
      break;

    case 3:
      target.Allocate(new_len[0],new_len[1],new_len[2]);
      break;

    case 4:
      target.Allocate(new_len[0],new_len[1],new_len[2],new_len[3]);
  }

  // nasobeni
  if (A1.Dim()==2) {
    rmax1 = A1.Length(1);
    cmax1 = A1.Length(2);
    cmax2 = A2.Length(2);

    for (row=1; row<=rmax1; row++) {
      for (col=1; col<=cmax2; col++) {
        sum = 0;
        for (idx=1; idx<=cmax1; idx++)
          sum += A1(row,idx)*A2(idx,col);
        target(row,col) = sum;
      }
    }
  } else
    ErrMsg("Multiplication of these dimension has not been provided yet.",fun);

  return target;
}


// division of the whole array by a number
template <class T, int rank>
DArray<T,rank> &DArray<T,rank>::operator/(T value)
{
  char *fun="DArray<T,rank>::operator/";
  unsigned int i;

  for (i=0; i<items; i++)
    data[i] /= value;
  return *this;
}


/*------------------------------------------------------------------------------------------------
  class DArray - main functions
------------------------------------------------------------------------------------------------*/

// set the steps for enlarging the array - automatically toggles the auto_enlarge flag to true
template <class T, int rank>
void DArray<T,rank>::EnlargeStep(int istep, ...)
{
  va_list args;
  int i;

  auto_enlarge = true;
  estep[0] = istep;
  va_start(args,istep);
  for (i=1; i<rank; i++)
    estep[i] = va_arg(args,int);
  va_end(args);
}


// fill the array by a number
template <class T, int rank>
void DArray<T,rank>::Fill(T value)
{
  memset(data, value, items*sizeof(T));
}


// find the maximum value in an array
template <class T, int rank>
T Max(DArray<T,rank> &arr)
{
  T max_item;
  long i;

  max_item = arr.data[0];
  for (i=1; i<arr.items; i++) 
    if (arr.data[i]>max_item) 
      max_item = arr.data[i];
  
  return max_item;
}


// enlarging the extent (number of values in each direction) of an array
template <class T, int rank>
void DArray<T,rank>::Resize(int inew, ...)
{
  char *fun="DArray<T,rank>::Resize";
  va_list args;
  T *new_data;
  unsigned int idx_target,idx_source;
  int ilen,jlen,klen,llen,newlen[MAXDIM];
  int i,k,l;

  if (!auto_enlarge) {
    ErrMsg("Cannot resize the fixed-size array (use EnlargeStep)",fun);
    return;
  }

  // read the arguments
  newlen[0] = inew;
  va_start(args,inew);
  for (i=1; i<rank; i++)
    newlen[i] = va_arg(args,int);
  va_end(args);

  // calculate of the new size of the array
  items = 1;
  ilen = jlen = klen = llen = 0;
  switch (dim) {
    case 4:
      if (newlen[3]>len[3] && estep[3]>0) {
        llen = len[3]+sup((newlen[3]-len[3])/double(estep[3]))*estep[3];
        items *= llen;
      } else {
        items *= len[3];
        llen = len[3];
      }

    case 3:
      if (newlen[2]>len[2] && estep[2]>0) {
        klen = len[2]+sup((newlen[2]-len[2])/double(estep[2]))*estep[2];
        items *= klen;
      } else {
        items *= len[2];
        klen = len[2];
      }

    case 2:
      if (newlen[1]>len[1] && estep[1]>0) {
        jlen = len[1]+sup((newlen[1]-len[1])/double(estep[1]))*estep[1];
        items *= jlen;
      } else {
        items *= len[1];
        jlen = len[1];
      }

    case 1:
      if (newlen[0]>len[0] && estep[0]>0) {
        ilen = len[0]+sup((newlen[0]-len[0])/double(estep[0]))*estep[0];
        items *= ilen;
      } else {
        items *= len[0];
        ilen = len[0];
      }
  }

  // increase the structure
  new_data = (T *) malloc(items*sizeof(T));
  if (new_data==NULL && items>0)
    ErrMsg("Allocation of the space for data unsuccessful.",fun);

  // fill by 0
  memset(new_data,0,items*sizeof(T));

  // store the data in the order of j,i,k,l (only i for vectors)
  switch (dim) {
    case 1:
      memcpy(new_data, data, len[0]*sizeof(T));
      len[0] = ilen;
      break;

    case 2:
      idx_source = idx_target = 0;
      for (i=0; i<len[0]; i++) {
	memcpy(new_data+idx_target, data+idx_source, len[1]*sizeof(T));
	idx_target += jlen;
	idx_source += len[1];
      }
      len[0] = ilen;
      len[1] = jlen;
      break;

    case 3:
      idx_source = idx_target = 0;
      for (k=0; k<len[2]; k++) {
        for (i=0; i<len[0]; i++) {
	  memcpy(new_data+idx_target, data+idx_source, len[1]*sizeof(T));
	  idx_target += jlen;
	  idx_source += len[1];
	}
	idx_target += (ilen-len[0])*jlen;  // the empty space left by enlarging
      }
      len[0] = ilen;
      len[1] = jlen;
      len[2] = klen;
      break;

    case 4:
      idx_source = idx_target = 0;
      for (l=0; l<len[3]; l++) {
        for (k=0; k<len[2]; k++) {
          for (i=0; i<len[0]; i++) {
	    memcpy(new_data+idx_target, data+idx_source, len[1]*sizeof(T));
	    idx_target += jlen;
	    idx_source += len[1];
          }
	  idx_target += (ilen-len[0])*jlen;  // the empty space left by enlarging
        }
	idx_target += (klen-len[2])*ilen*jlen;  // the empty space left by enlarging
      }
      len[0] = ilen;
      len[1] = jlen;
      len[2] = klen;
      len[3] = llen;      
      break;
  }

  // copy the data from the temporary storage
  Free();
  data = (T *) malloc(items*sizeof(T));
  memcpy(data, new_data, items*sizeof(T));
  free(new_data);
}

#endif
