#include <assert.h>
#include <math.h>
#include <qpainter.h>
#include <stdlib.h>

#include "appwin.h"
#include "calcs.h"
#include "messages.h"
#include "pltwin.h"
#include "ddplot.h"


// constructor
PltWin::PltWin(QWidget *parent, Qt::WindowFlags f) 
  : QWidget( parent, f )
{
  int d,i,j;

  xBorder = yBorder = 30;
  xOffset = yOffset = 0;
  arrAngle = 60.0;
  ZFact = ZOOM_FACT;
  xPan = yPan = 0;
  isSelected = true;
  DisloCenter = SHOW_DCENTER;
  PlaneTraces = PTRACES;
  PTAngle = PTRACES_ANGLE;

  tail = NULL;
  TailSize = 0;

  arrNeighNum = ARR_NEIGHNUM;
  arrNeighbors.Allocate(arrNeighNum);
  for (i=1; i<=arrNeighNum; i++)
    arrNeighbors(i) = ARR_NEIGHBORS[i-1];

  ProjVector.Allocate(3);
  for (d=1; d<=3; d++) 
    ProjVector(d) = PROJ_VECTOR[d-1];

  blSize.Allocate(3);
  xyzMin.Allocate(3);  xyzMax.Allocate(3);
  period.Allocate(3);
  xyzCen.Allocate(3);
  cellsize.Allocate(3);
  ncell.Allocate(3);

  // auto-enlarge=true
  zLayerSel.Allocate(60);  zLayerSel.EnlargeStep(5);
  zCoordLayer.Allocate(60);  zCoordLayer.EnlargeStep(5);  
  zColorLayer.Allocate(60,2);  zColorLayer.EnlargeStep(5,0);
  zLineThickLayer.Allocate(60);  zLineThickLayer.EnlargeStep(5);
  zDiamLayer.Allocate(60);  zDiamLayer.EnlargeStep(5);
  RdfNum.Allocate(5);  RdfNum.EnlargeStep(5);
  RdfDist.Allocate(5);  RdfDist.EnlargeStep(5);

  csys.Allocate(3,3);
  for (i=1; i<=3; i++) 
    for (j=1; j<=3; j++)
      csys(i,j) = CSYS[i-1][j-1];

  DScaleFact = DSCALE_FACT;
  shortestArrow = ARROW_SHORTEST;     // shortest arrow to be plotted
  thicknessArrow = ARROW_THICKNESS;
  plotType = PLOT_TYPE;
  DispComponent = DISP_COMPONENT;
  AtomPos = ATOMPOS;
  AtomDispScale = ATOMDISP_SCALE;
  PrintMagDisp = PRINT_MAG_DISP;
  AtomNumbers = ATOM_NUMBERS;

  NInert = 0;
  InertAtoms = false;
  isGBfile = false;
  showGB = SHOWGB;
  showZLayers = SHOW_ZLAYERS;
  showCSys = SHOW_CSYS;
  showNeighCells = SHOW_NEIGHCELLS;

  zTolerance = ZTOLERANCE;

  corrEdgeCompBCC = CORR_EDGECOMP_BCC;

  xCore = yCore = 0.0;
  Bvect.Allocate(3);

  rcut = 0;

  paintEPS = false;

  setGeometry( 0, 23, parent->width(), parent->height()-23 );
}


// destructor
PltWin::~PltWin()
{
  free(tail);
}


// copy the structure from p (assignment operator doesn't work)
void PltWin::copy( PltWin *p )
{
  FName = p->FName;

  zColorLayer = p->zColorLayer;

  NRel = p->NRel;
  NInit = p->NInit;
  NInert = p->NInert;
  InertAtoms = p->InertAtoms;

  xyzRel = p->xyzRel;
  xyzInit = p->xyzInit;
  if (p->InertAtoms)
    xyzInert = p->xyzInert;
  aDisp = p->aDisp;
  atomType = p->atomType;

  csys = p->csys;
  blSize = p->blSize;
  xyzMin = p->xyzMin;
  xyzMax = p->xyzMax;
  period = p->period;

  TailSize = p->TailSize;
  if (p->TailSize>0) {
    if (tail!=NULL)
      free(tail); 
    tail = (char*) malloc(p->TailSize);
    memcpy(tail, p->tail, p->TailSize*sizeof(char));
  }

  zCoordLayer = p->zCoordLayer;
  zLayer = p->zLayer;
  zLineThickLayer = p->zLineThickLayer;
  zDiamLayer = p->zDiamLayer;
  zLayerSel = p->zLayerSel;

  NumZLayers = p->NumZLayers;
  showZLayers = p->showZLayers;

  xCore = p->xCore;
  yCore = p->yCore;
  Bvect = p->Bvect;

  isGBfile = p->isGBfile;
  if (p->isGBfile) {
    gbLayer = p->gbLayer;
    gbYcoor = p->gbYcoor;
    showGB = p->showGB;
  }

  ProjVector = p->ProjVector;

  arrAngle = p->arrAngle; 
  ZFact = p->ZFact;
  xPan = p->xPan;
  yPan = p->yPan;
  DScaleFact = p->DScaleFact;
  AtomDispScale = p->AtomDispScale;
  factor = p->factor;
  xBorder = p->xBorder;
  yBorder = p->yBorder;
  xOffset = p->xOffset;
  yOffset = p->yOffset;
  shortestArrow = p->shortestArrow;
  thicknessArrow = p->thicknessArrow;
  plotType = p->plotType;
  DispComponent = p->DispComponent;
  AtomPos = p->AtomPos;
  PTAngle = p->PTAngle;
  PrintMagDisp = p->PrintMagDisp;
  AtomNumbers = p->AtomNumbers;
  isSelected = p->isSelected;
  DisloCenter = p->DisloCenter;
  PlaneTraces = p->PlaneTraces;
  InertAtoms = p->InertAtoms;
  showCSys = p->showCSys;

  atomScreen = p->atomScreen;
  xyzCen = p->xyzCen;

  RdfDist = p->RdfDist;
  arrNeighbors = p->arrNeighbors;
  RdfNum = p->RdfNum;

  arrNeighNum = p->arrNeighNum;
  RdfNNeigh = p->RdfNNeigh;
  rcut = p->rcut;
  showNeighCells = p->showNeighCells;

  cellsize = p->cellsize;
  ncell = p->ncell;
  list = p->list;
  head = p->head;
  NeighListInit = p->NeighListInit;
  scaleArr = p->scaleArr;
}


void PltWin::paintEvent( QPaintEvent *)
{
  QPainter *p = new QPainter(this);
  drawIt(p);
  p->end();
}


void PltWin::resizeEvent( QResizeEvent * )
{
  SetGeometry();
}


void PltWin::drawIt( QPainter *p )
{
  QString txt;
  double x, y, x0, y0, x1, y1;
  int n, layer, prev_layer, circDia, i, j;

  QPen *pen = new QPen();
  QBrush *brush = new QBrush();
  QBrush *brush_unrld = new QBrush();


  // plot the grain boundary line
  if (isGBfile && showGB) {
    x0 = ZFact*xyzMin(1) - xPan - (ZFact-1)*xyzCen(1);
    x1 = ZFact*xyzMax(1) - xPan - (ZFact-1)*xyzCen(1);
    y  = ZFact*gbYcoor - yPan - (ZFact-1)*xyzCen(2);
    DrawLine(p, x0, y, x1, y, 0, 0);
  }

  if (AtomPos==COMPOSITE) {
    brush_unrld->setColor( Qt::lightGray );
    brush_unrld->setStyle( Qt::SolidPattern );
  }

  // plotting of atomic arrangement in the initial configuration
  prev_layer = -1;
  x0 = y0 = x1 = y1 = 0;


  //
  //  Plot the atomic configuration
  //

  for (n=1; n<=NInit; n++) {
    // plot only those atoms which belong to the Z-layers that are selected (active)
    if (!zLayerSel(zLayer(n)))
      continue;

    if (plotType==PLOT_ATOM_LAYERS)
      layer = zLayer(n); 
    else
      layer = atomType(n);

    if (layer!=prev_layer) {
      pen->setColor( zColorLayer(layer,1) );
      pen->setWidth( zLineThickLayer(layer) );
      p->setPen( *pen );

      brush->setColor( zColorLayer(layer,2) );
      brush->setStyle( Qt::SolidPattern );
      p->setBrush( *brush );   

      circDia = zDiamLayer(layer);
      prev_layer = layer;
    }


    // plot the atomic positions

    switch(AtomPos) {
      case UNRELAXED:
        x = ZFact*xyzInit(n,1) - xPan - (ZFact-1)*xyzCen(1);
	y = ZFact*xyzInit(n,2) - yPan - (ZFact-1)*xyzCen(2);
        DrawCircle(p, n, x, y, circDia);
        break;

      case RELAXED:
	x = ZFact*(xyzInit(n,1) + AtomDispScale*aDisp(n,1)) - xPan - (ZFact-1)*xyzCen(1);
        y = ZFact*(xyzInit(n,2) + AtomDispScale*aDisp(n,2)) - yPan - (ZFact-1)*xyzCen(2);
        DrawCircle(p, n, x, y, circDia);
	break;

      case COMPOSITE:
	p->setBrush( *brush_unrld );
	x = ZFact*xyzInit(n,1) - xPan - (ZFact-1)*xyzCen(1);
	y = ZFact*xyzInit(n,2) - yPan - (ZFact-1)*xyzCen(2);
        DrawCircle(p, n, x, y, circDia);
  
        p->setBrush( *brush );
	x = ZFact*(xyzInit(n,1) + AtomDispScale*aDisp(n,1)) - xPan - (ZFact-1)*xyzCen(1);
	y = ZFact*(xyzInit(n,2) + AtomDispScale*aDisp(n,2)) - yPan - (ZFact-1)*xyzCen(2); 
        DrawCircle(p, -1, x, y, circDia);
    }

    if (x<x0) x0 = x;
    if (x>x1) x1 = x;
    if (y<y0) y0 = y;
    if (y>y1) y1 = y;

    // atom numbers
    if (AtomNumbers) {
      txt.sprintf("%d", n);
      DrawText(p, x, y, txt);
    }
  }


  // plot the inert atoms
  if (InertAtoms) {
    pen->setColor( Qt::black );
    pen->setWidth( zLineThickLayer(layer) );
    p->setPen( *pen );

    brush->setColor( Qt::black );
    brush->setStyle( Qt::NoBrush );
    p->setBrush( *brush ); 
            
    for (n=1; n<=NInert; n++) {
      // plot only those atoms which belong to the selected (active) layers
      //      if (!zLayerSel(zLayer(n)))
      //	continue;

      x = ZFact*xyzInert(n,1) - xPan - (ZFact-1)*xyzCen(1);
      y = ZFact*xyzInert(n,2) - yPan - (ZFact-1)*xyzCen(2);
      DrawCircle(p, 0, x, y, circDia);
    }    
  }

  // plotting the coordinate system centered at the initial position of
  // the dislocation line
  if (PlaneTraces) 
    DrawPlaneTraces(p, x0, y0, x1, y1);

  // plotting of arrows
  if (arrNeighNum>0) {
    QPen apen( Qt::black, thicknessArrow );
    p->setPen( apen ); 
    p->setBrush( Qt::black );
    
    switch(DispComponent) {
      case EDGE:
	plotEdgeComponent(p);
	break;
	
      case SCREW:
	plotScrewComponent(p);
	break;
      
      case MIXED:
	plotEdgeComponent(p);
	break;
      
      case DIFF_EDGE:
      case DIFF_SCREW:
	plotDifference(p);
    }
  }

  // position of dislocation center
  if (DisloCenter) {
    x = ZFact*xCore - xPan - (ZFact-1)*xyzCen(1);
    y = ZFact*yCore - yPan - (ZFact-1)*xyzCen(2);
    DrawCircle(p, -1, x, y, circDia);
  }

  // small inset to show the active Z-layers
  if (showZLayers)
    ShowActiveZLayers(p);

  // lines showing the division of the block into cells (for the linked neighbor list)
  if (showNeighCells) {
    for (i=0; i<=ncell(1); i++) {
      x = ZFact*(xyzMin(1)+i*cellsize(1)) - xPan - (ZFact-1)*xyzCen(1);
      y0 = ZFact*xyzMin(2) - yPan - (ZFact-1)*xyzCen(2);
      y1 = ZFact*xyzMax(2) - yPan - (ZFact-1)*xyzCen(2);
      DrawLine(p, x, y0, x, y1, 0, 0);    
      
      for (j=0; j<=ncell(2); j++) {
	y = ZFact*(xyzMin(2)+j*cellsize(2)) - yPan - (ZFact-1)*xyzCen(2);
	x0 = ZFact*xyzMin(1) - xPan - (ZFact-1)*xyzCen(1);
	x1 = ZFact*xyzMax(1) - xPan - (ZFact-1)*xyzCen(1);
	DrawLine(p, x0, y, x1, y, 0, 0);
      }
    }
  }

  // coordinate system of the block
  if (showCSys)
    ShowCSys(p);
}


void PltWin::printIt( QPrinter *printer )
{
  QPainter *prn = new QPainter(printer);
  drawIt(prn);
  prn->end();
}

