#include <math.h>

#include "ddplot.h"
#include "messages.h"
#include "neighlist.h"
#include "pltwin.h"
#include "darray.h"

DVector rdf_dist;  // radial distribution function (used in qsort)
IVector rdf_num;


/* 
  This function compares two distances in the RDF (for the use in qsort below. 
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
  Calculate the radial distribution function - i.e. find the number of atoms which have a certain
  distance from a particular central atom. 
*/

bool CalcRDF(PltWin *pw)
{
  double dx, dy, dz, dist2, rcut2, dist;
  DMatrix rdf_mat;
  IVector icell(3), icmin(3), icmax(3), idxs;
  int i, d, idx, icen, ineigh, iidx, jidx, kidx;
  bool written;

  if (VERBOSE) {
    printf("* calculating the radial distribution function:\n");
    printf("    cut-off radius for atomic neighbors:  %0.4lf\n", pw->rcut);
  }
  
  pw->RdfNNeigh = 0;
  pw->RdfNum = 0;
  rcut2 = pow(pw->rcut, 2);

  for (icen=1; icen<=pw->NInit; icen++) {
    // consider only atoms in the selected Z-layers
    if (!pw->zLayerSel(pw->zLayer(icen)))
      continue;

    // get the indices of the cell in which atom icen resides
    GetCell(pw, icen, icell);

    // we will search also the neighboring cells - get the indices
    for (d=1; d<=3; d++) {
      icmin(d) = icell(d) - 1;
      icmax(d) = icell(d) + 1;
      if (d < 3) {
	if (icmin(d) == 0) icmin(d) = 1;
	if (icmax(d) == pw->ncell(d)+1) icmax(d) = pw->ncell(d);
      }
    }

    // search through all neighboring cells and itself
    for (kidx=icmin(3); kidx<=icmax(3); kidx++) {
      for (jidx=icmin(2); jidx<=icmax(2); jidx++) {
	for (iidx=icmin(1); iidx<=icmax(1); iidx++) {
	  // get the head atom (one of possible neighbors of icen) for this cell
	  if (kidx == 0)
	    ineigh =  pw->head(iidx,jidx,pw->ncell(3));
	  else if (kidx == pw->ncell(3)+1)
	    ineigh =  pw->head(iidx,jidx,1);
	  else
	    ineigh = pw->head(iidx,jidx,kidx);

	  while (ineigh > 0) {
	    // skip itself
	    if (ineigh < icen || (ineigh == icen && kidx != 0 && kidx != pw->ncell(3)+1)) {
	      ineigh = pw->list(ineigh);
	      continue;
	    }

	    // consider only the neighboring atoms located in selected Z-layers
	    if (!pw->zLayerSel(pw->zLayer(ineigh))) {
	      ineigh = pw->list(ineigh);
	      continue;
	    }
	    
	    dx = pw->xyzInit(ineigh,1) - pw->xyzInit(icen,1);
	    if (fabs(dx) > pw->rcut) {
	      ineigh = pw->list(ineigh);
	      continue;
	    }
	    
	    dy = pw->xyzInit(ineigh,2) - pw->xyzInit(icen,2); 
	    if (fabs(dy) > pw->rcut) {
	      ineigh = pw->list(ineigh);
	      continue;
	    }

	    if (kidx == 0)
	      dz = pw->xyzInit(ineigh,3) - pw->period(3) - pw->xyzInit(icen,3);
	    else if (kidx == pw->ncell(3)+1)
	      dz = pw->xyzInit(ineigh,3) + pw->period(3) - pw->xyzInit(icen,3);
	    else
	      dz = pw->xyzInit(ineigh,3) - pw->xyzInit(icen,3); 
	    if (fabs(dz) > pw->rcut) {
	      ineigh = pw->list(ineigh);
	      continue;
	    }
	    
	    // calculate the distance between icen and ineigh
	    dist2 = pow(dx,2) + pow(dy,2) + pow(dz,2);
	    
	    if (dist2 <= rcut2) {
	      written = false;
	      dist = sqrt(dist2);      		
	      if (pw->RdfNNeigh > 0) {
		for (i=1; i<=pw->RdfNNeigh; i++) {
		  if (dist <= pw->RdfDist(i)+1e-3 && dist >= pw->RdfDist(i)-1e-3) {
		    pw->RdfNum(i)++;
		    written = true;
		    break;
		  }
		}
	      }
	      	      
	      // a completely new interatomic distance
	      if (!written) {
		pw->RdfNNeigh++;
		pw->RdfNum(pw->RdfNNeigh) = 1;
		pw->RdfDist(pw->RdfNNeigh) = dist;
		if (VERBOSE)
		  printf("%d | icen=%d ineigh=%d | r=%0.4lf\n", pw->RdfNNeigh, icen, ineigh, pw->RdfDist(pw->RdfNNeigh));
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
    printf("No nearest neighbors have been identified:\n");
    printf("Increase NeighRCut in .ddplot.\n");
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


/*
  Calculate the radial distribution function for the structure pointed to by pw and for the range of
  atomic separations given in the arguments.
*/

void RDF(PltWin *pw, int which, double rmin, double rmax, int nbins, QVector<double> &rdfrad, 
	 QVector<double> &rdfg, double &gmax)
{
  QVector<int> rdfnum(nbins,0);
  double rx, ry, rz, r, rsq, rminsq, rmaxsq, rlen, dr, c, val;
  int icen, ineigh, ibin;

  rminsq = pow(rmin,2);
  rmaxsq = pow(rmax,2);
  rlen = rmax-rmin;
  dr = rlen/nbins;

  rdfrad.push_back(rmin + dr/2.0);
  for (ibin=1; ibin<nbins; ibin++) 
    rdfrad.push_back(rdfrad[ibin-1] + dr);

  for (icen=1; icen<=pw->NInit; icen++) {
    if (!pw->zLayerSel(pw->zLayer(icen)))
      continue;
    
    for (ineigh=icen+1; ineigh<=pw->NInit; ineigh++) {
      if (!pw->zLayerSel(pw->zLayer(ineigh))) 
	continue;
      
      if (which == UNRELAXED) {
	rx = pw->xyzInit(ineigh,1) - pw->xyzInit(icen,1);
	ry = pw->xyzInit(ineigh,2) - pw->xyzInit(icen,2); 
	rz = pw->xyzInit(ineigh,3) - pw->xyzInit(icen,3); 	    
      } else {
	rx = pw->xyzRel(ineigh,1) - pw->xyzRel(icen,1);
	ry = pw->xyzRel(ineigh,2) - pw->xyzRel(icen,2); 
	rz = pw->xyzRel(ineigh,3) - pw->xyzRel(icen,3); 	    
      }
      
      rsq = pow(rx,2) + pow(ry,2) + pow(rz,2);
      if (rsq >= rminsq && rsq <= rmaxsq) {
	r = sqrt(rsq);
	ibin = floor((r-rmin)/dr);
	rdfnum[ibin]++;	
      }
    }
  }

  c = 4.0*M_PI*dr;
  gmax = 0.0;  
  for (ibin=0; ibin<nbins; ibin++) {
    rsq = pow(rdfrad[ibin],2);
    val = rdfnum[ibin]/(c*rsq);
    rdfg.push_back(val);   // we calculate g(r)*rho here
    if (val>gmax) gmax = val;
  }
}
