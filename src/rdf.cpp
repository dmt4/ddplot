#include <math.h>

#include "ddplot.h"
#include "messages.h"
#include "neighlist.h"
#include "pltwin.h"
#include "darray.h"



DVector rdf_dist;  // radial distribution function (used in qsort)
IVector rdf_num;


/*
  This function compares two distances in the RDF (for the use in
  qsort below.
*/

int RdfCompare(const void *idx1, const void *idx2)
{
  if (rdf_dist(*(int *)idx1) < rdf_dist(*(int *)idx2)) 
    return -1;
  else if (rdf_dist(*(int *)idx1) == rdf_dist(*(int *)idx2)) 
    return 0;
  else
    return 1;
}


/*
  Calculate the radial distribution function - i.e. find the number
  of atoms which have a certain distance from a particular central
  atom.
*/

bool CalcRDF( PltWin *pw )
{
  double dx, dy, dz, dist;
  DMatrix rdf_mat;
  IVector icell(3), icmin(3), icmax(3), idxs;
  int i, d, idx, icen, ineigh, iidx, jidx, kidx;
  bool written;


  pw->RdfNNeigh = 0;

  for (icen=1; icen<=pw->NInit; icen++) {
    // consider only atoms in the selected Z-layers
    if (!pw->zLayerSel( pw->zLayer(icen) ))
      continue;

    // get the indices of the cell in which atom icen resides
    GetCell(pw, icen, icell);

    // we will search also the neighboring cells - get the indices
    for (d=1; d<=3; d++) {
      icmin(d) = (icell(d)-1 >= 1)  ?  icell(d)-1 : 1;  
      icmax(d) = (icell(d)+1 <= pw->ncell(d))  ?  icell(d)+1 : pw->ncell(d);
    }


    // search through all neighboring cells and itself
    for (iidx=icmin(1); iidx<=icmax(1); iidx++) {
      for (jidx=icmin(2); jidx<=icmax(2); jidx++) {
	for (kidx=icmin(3); kidx<=icmax(3); kidx++) {

	  // get the head atom (one of possible neighbors of icen) for this cell
	  ineigh = pw->head(iidx,jidx,kidx);

	  while (ineigh>0) {
	    // skip itself
	    if (icen==ineigh) {
	      ineigh = pw->list(ineigh);
	      continue;
	    }

	    // consider only the neighboring atoms located in selected Z-layers
	    if (!pw->zLayerSel( pw->zLayer(ineigh) )) {
	      ineigh = pw->list(ineigh);
	      continue;
	    }
	    
	    dx = pw->xyzInit(ineigh,1) - pw->xyzInit(icen,1);
	    dy = pw->xyzInit(ineigh,2) - pw->xyzInit(icen,2); 
	    dz = pw->xyzInit(ineigh,3) - pw->xyzInit(icen,3); 
	    
	    // calculate the distance between icen and ineigh
	    dist = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));
	    written = false;
	    
	    if (dist <= pw->rcut) {
	      // add into the array of atomic separations if it fits in some
	      for (i=1; i<=pw->RdfNNeigh; i++) {
		if (dist <= pw->RdfDist(i)+1e-3 && dist >= pw->RdfDist(i)-1e-3) {
		  pw->RdfNum(i)++;
		  written = true;
		  break;
		}
	      }
	      	      
	      // a completely new interatomic distance
	      if (!written) {
		pw->RdfNNeigh++;
		pw->RdfNum( pw->RdfNNeigh ) = 1;
		pw->RdfDist( pw->RdfNNeigh ) = dist;
	      }
	    }

	    // take next atom in the same cell (-1 of no other exists)
	    ineigh = pw->list(ineigh);
	  };
	}
      }
    }
  }

  if (pw->RdfNNeigh==0) {
    printf("No nearest neighbors have been identified. Increase NeighRCut in .ddplot.\n");
    exit(0);
  }

  // sort according to RdfDist in the ascending order -> after
  // sorting the qsort returns indexes <idxs> such that the array
  // RdfDist[idxs[:]] is sorted
  rdf_dist.Allocate(pw->RdfNNeigh);
  rdf_dist = pw->RdfDist;

  rdf_num.Allocate(pw->RdfNNeigh);
  rdf_num = pw->RdfNum;

  // we need a matrix for sorting below...
  rdf_mat.Allocate(pw->RdfNNeigh,1);
  memcpy(rdf_mat.data, rdf_dist.data, pw->RdfNNeigh*sizeof(double));

  // get the indexes into the sorted array
  idxs.Allocate(pw->RdfNNeigh);
  sort(rdf_mat, 1, pw->RdfNNeigh, 1, idxs);

  // save the sorted arrays
  for (i=1; i<=pw->RdfNNeigh; i++) {
    pw->RdfDist(i) = rdf_dist( idxs(i) );
    pw->RdfNum(i) = rdf_num( idxs(i) );
  }

  return true;
}
