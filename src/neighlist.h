#ifndef NEIGHLIST_H
#define NEIGHLIST_H

#include "pltwin.h"
#include "darray.h"


bool InitNeighborList( PltWin *pw, IMatrix &nnlist );
bool RelNeighborList( PltWin *pw, int rmax, IMatrix &nnlist );

void NeighInit( PltWin *pw );
void NeighList( PltWin *pw );
void GetCell( PltWin *pw, int i, IVector &icell );


#endif
