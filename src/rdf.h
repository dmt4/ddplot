#ifndef RDF_H
#define RDF_H

#include "pltwin.h"

int RdfCompare(const void *idx1, const void *idx2);
bool CalcRDF( PltWin *pw );
void RDF(PltWin *pw, int which, double rmin, double rmax, int nbins, QVector<double> &rdfrad, QVector<double> &rdfg, 
	 double &gmax);

#endif
