#ifndef PLTWIN_H
#define PLTWIN_H

#include <qwidget.h>
#include <qprinter.h>

#include "ddplot.h"
#include "darray.h"


class PltWin : public QWidget
{
  Q_OBJECT

public:
  QString FName;
  DArray<QColor,2> zColorLayer;   // colors of the atomic layers (1=foreground, 2=background)

  // coordinates of atoms and their relative displacements
  DMatrix xyzRel;          // coordinates of atoms in the relaxed configuration
  DMatrix xyzInit;         // coordinates of atoms in the initial (unrelaxed) configuration
  DMatrix xyzInert;        // coordinates of inert atoms
  DMatrix aDisp;           // atomic displacements (real values)
  IVector atomType;        // flag recognizing the types of atoms
  int NRel;                // number of atoms in the relaxed block
  int NInit;               // number of atoms in the initial block
  int NInert;              // number of inert atoms (block in the BOP format)

  // parameters of the atomic block
  DMatrix csys;            // coordinate system of the block
  DVector blSize;          // calculated size of the block in the x,y,z direction
  DVector xyzMin, xyzMax;
  DVector period;          // periodicity in the three directions
  char *tail;              // the tail of the .plt file containing the relaxation info (text)
  int TailSize;            // size of the trailing info stored in <tail>

  // Z-layers
  DVector zCoordLayer;     // z-coordinates of layers (1 to NumZLayers)
  IVector zLayer;          // identifiers of the layers
  int NumZLayers;          // number of layers of atoms
  IVector zLineThickLayer; // line thickness of atoms in a specific layer
  IVector zDiamLayer;      // diameter of atoms in a specific layer in pixels
  BVector zLayerSel;       // true if a particular Z-layer is selected (considered for plotting arrows)
  bool showZLayers;        // true if the active Z-layers should be visualized as a small inset
  double zTolerance;       // tolerance for the Z-coordinates of atoms in the same Z-layer 

  // dislocations
  DVector Bvect;           // Burgers vector
  double xCore,yCore;
  bool showNyeTensor;      // show a contour map corresponding to a component of the Nye tensor
  bool knownNyeTensor;     // true if the Nye tensor was already calculated (initially false)

  // grain boundaries
  double gbYcoor;          // y-position of the grain boundary
  IVector gbLayer;         // grain boundary layer in which the atom lies(upper grain>0, lower grain<0)
  bool isGBfile;           // true if the file contains a grain boundary data
  bool showGB;             // true if the grain boundary is being plotted 

  // appearance of the plot
  DVector ProjVector;      // vector into whose direction the displacements are to be projected
                           // in the system [1-21], [-101], [111]
  double arrAngle;         // angle of arrows
  double ZFact;            // zoom factor
  double xPan,yPan;        // panning
  double DScaleFact;       // scaling factor for displacements (arrow lengths)
  double AtomDispScale;    // scaling of atomic displacements in the relaxed configuration
  double factor;
  IMatrix atomScreen;      // coordinates of atoms on the screen [pixels]
  int xBorder,yBorder,xOffset,yOffset;
  IVector xyzCen;
  int shortestArrow;    // shortest arrow to be plotted (in pixels)
  int thicknessArrow;   // thickness of an arrow (in pixels)
  int plotType;               // type of plot
  int DispComponent;          // component of the displacements plotted
  int AtomPos;             // plotting of atomic positions (RELAXED, UNRELAXED)  
  int PTAngle;             // angle between two neighboring traces of planes plotted
  bool PrintMagDisp;       // print magnitudes of displacements (value/a) ?
  bool AtomNumbers;        // print the numbers of atoms
  bool isSelected;         // true if this plot is selected
  bool DisloCenter;        // show the dislocation center ?
  bool PlaneTraces;        // show the traces of the planes ?
  bool InertAtoms;         // plot inert atoms ?
  bool showCSys;           // is the coordinate system plotted ?
  bool corrEdgeCompBCC;    // correction of the orientations of edge components for screw in BCC

  FILE *feps;              // handle to the EPS file
  bool paintEPS;           // paint in the EPS file ?

  // radial distribution function
  DVector RdfDist;         // distances of the first, second, etc., nearest neighbors
  int arrNeighNum;         // number of different neighbor distances to be considered
  IVector arrNeighbors;    // which neighbor distances (1=first, 2=second, etc.)      
  IVector RdfNum;          // numbers of 1st, 2nd, 3rd, ..., nearest neighbors
  int RdfNNeigh;           // how many nearest neighbors ? (3=first, second and third)

  // linked neighbor list
  double rcut;             // cut-off radius (minimum size of an edge in either direction)
  DVector cellsize;        // size of a cell's edge in x, y and z direction
  IVector ncell;           // number of cells in x, y and z direction
  IVector list;            // linked neighbor list
  IArray3 head;            // index of a head atom in each cell
  IMatrix NeighListInit, NeighListRel;
  DArray3 scaleArr;        // scaling factors for plotting the arrows
  bool showNeighCells;     // true if the division into cells is to be displayed


  explicit PltWin( QWidget *parent=0, Qt::WindowFlags f=0 );
  ~PltWin();
  void copy( PltWin *p );
  void paintEvent( QPaintEvent * );
  void plotEdgeComponent(QPainter *p);
  void plotScrewComponent(QPainter *p);
  void plotDifference(QPainter *p);
  void plotNyeTensor(QPainter *p);

protected:
  virtual void resizeEvent( QResizeEvent * );

public:
  void drawIt(QPainter *p);
  void printIt(QPrinter *printer);

  bool DrawArrow( QPainter *p, double x0, double y0, double x1, double y1 );
  void DrawCircle( QPainter *p, int atom, double x, double y, int dia);
  void DrawLine( QPainter *p, double x0, double y0, double x1, double y1, int addx, int addy);
  void DrawPlaneTraces( QPainter *p, double x0, double y0, double x1, double y1 );
  void DrawText( QPainter *p, double x, double y, QString txt );
  void Pan( double xsteps, double ysteps );
  void SetGeometry();
  void ShowActiveZLayers( QPainter *p );
  void ShowCSys( QPainter *p );
  void Zoom( double zfact );

public:
  void calcEdgeRelDisp();
  void calcScrewRelDisp();
  void calcNyeTensor();
  bool CompareDisp( PltWin *p1, PltWin *p2, int comp );
  int FindNearestAtom( int x, int y );
  bool LoadBOP( QString fname );
  bool LoadDD( QString fname );
  bool LoadGB( QString fname, long int *blstart, PltWin *initConf );
  bool LoadPLT( QString fname );
  bool LoadXYZ( QString fname );
  bool PrepareBlock( bool toggle_inert );
};

#endif
