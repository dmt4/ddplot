#include "appwin.h"
#include "pltwin.h"


// Consider that the Y-axis is a diad (in our bcc block it is the [-101] axis) and apply
// this operation to show the symmetry-related block.
void AppWin::applyYDiad()
{
  reflectBlock(actYZReflect);
  reflectBlock(actXYReflect);
}


void AppWin::correctEdgeCompBCC()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  CORR_EDGECOMP_BCC = !CORR_EDGECOMP_BCC;
  pw->corrEdgeCompBCC = CORR_EDGECOMP_BCC;
  actCorrEdgeBCC->setChecked(CORR_EDGECOMP_BCC);
  pw->calcEdgeRelDisp();
  pw->repaint();
}
