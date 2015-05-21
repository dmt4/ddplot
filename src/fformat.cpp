#include <stdio.h>
#include <stdlib.h>

#include "ddplot.h"
#include "fformat.h"


// Return the format of file recognized from the file name.
int FFormat( QString fn )
{
  QString fname;
  FILE *f;
  double xcoor, ycoor, zcoor;
  int iswhat, nrel, atype, n, endc;
  bool isDD, isBOP, isPLT, isXYZ;

  fname = fn.toLower();

  // JMol's .xyz format
  isXYZ = (fname.indexOf(".xyz") >= 0);

  // internal format of the DDPlot program
  isDD = (fname.indexOf(".dd") >= 0);

  // BOP block file with inert and active atoms
  isBOP = (fname.indexOf(".bl") == fname.length()-3) || 
    (fname.indexOf(".block") == fname.length()-6) ||
    (fname.indexOf("block.init") >=0 ) || 
    (fname.indexOf("block.interm") >=0 ) || 
    (fname.indexOf("block.out") >=0 );

  if (isXYZ)  return FORMAT_XYZ;
  if (isDD)   return FORMAT_DD;
  if (isBOP)  return FORMAT_BOP;

  //
  // We must open the file to determine if it contains the grain boundary data file
  //

  f = fopen(fn.toAscii().data(),"r+");

  if (f==NULL)
    return FORMAT_UNKNOWN;

  fscanf(f, "%d\n", &nrel);
  for (n=0; n<nrel; n++)
    fscanf(f, "%lf\n", &zcoor);

  fscanf(f, "%lf %lf %d", &xcoor, &ycoor, &atype);
  endc = getc(f);
  if (endc!='\r' && endc!='\n' && endc!='\0') {  
    fclose(f);
    return FORMAT_GB;  // it is the grain boundary data file
  }

  fclose(f);
  return FORMAT_PLT;  // it is the plot file
}
