#ifndef APPWIN_H
#define APPWIN_H

#include <qlabel.h>
#include <qmainwindow.h>
#include <qmenu.h>
#include <qtabwidget.h>

#include "pltwin.h"
#include "strings.h"


class AppWin: public QMainWindow
{
  Q_OBJECT

public:
  QPrinter *printer;
  QTabWidget *tabWidget;
  QLabel *sbar;
  PltWin *plotWidget[MAX_PLOTS];

  QMenu *mfile, *mexport, *mview, *mplot, *mlayers, *mtran, *mmacros, *mappear, *mhelp;
  QAction *actOpen, *actSave, *actClose, *actCloseAll, *actPrint, *actExpEPS, *actExpXYZ, *actExpPLT, 
    *actExpCFG, *actAnimate, *actQuit;
  QAction *actZoomIn, *actZoomOut, *actFitWin, *actPanRight, *actPanLeft, *actPanUp, *actPanDown;
  QAction *actFirstPlot, *actPrevPlot, *actNextPlot, *actLastPlot, *actSelPlots, *actPanFRight, 
    *actPanFLeft, *actPanFUp, *actPanFDown;
  QAction *actUnrelView, *actRelView, *actCompView, *actMagDisp, *actNumAtom, *actNoComp, *actEdgeComp;
  QAction *actScrewComp, *actDAtLayers, *actDAtTypes, *actInertAtom, *actBurgVect, *actDCenter, 
    *actSPlanes, *actShowCSys, *actShowCells, *actShowGb, *actRelaxInfo;
  QAction *actShowZLay, *actSetZLay, *actCoordAt;
  QAction *actCoordSys, *actXZReflect, *actYZReflect, *actXYReflect, *actXRot180, *actYRot180, *actZRot180, 
    *actXRot90, *actYRot90, *actZRot90, *actTransform, *actProjVect, *actCompDisp;
  QAction *actScalePos, *actScaleArr, *actArrNeigh, *actSetZTol;
  QAction *actApplyYDiad, *actCorrEdgeBCC;
  QAction *actCrystPlot, *actArrPlot, *actAbout;

  AppWin();
  ~AppWin();

protected:
  void closeEvent( QCloseEvent* );
  virtual void resizeEvent( QResizeEvent * );

private slots:
  void about();
  void animate();
  void applyYDiad();
  void arrNeighbors();
  void arrowsPlot();
  void burgVect();
  void coordAtoms();
  bool coordSys();
  void close();
  void closeAll();
  void compareDisp();
  void correctEdgeCompBCC();
  void createMenus();
  void crystalPlot();
  void disloCenter();
  void enableMenuItems( bool flag );
  void exportCFG();
  void exportEPS();
  void exportPLT();
  void exportXYZ();
  void fitWin();
  void interpretCommand( QString cmd );
  bool loadFile( QString fn );
  void loadSettings();
  void open();
  void panRight();
  void panLeft();
  void panUp();
  void panDown();
  void panFastRight();
  void panFastLeft();
  void panFastUp();
  void panFastDown();
  void plotAtomLayers();
  void plotAtomTypes();
  void plotEdgeComponent();
  void plotNoComponent();
  void plotScrewComponent();
  void plotUnrelaxed();
  void plotRelaxed();
  void plotComposite();
  void plotInertAtoms();
  void print();
  void printMagDisp();
  void printAtomNum();
  void projVector();
  void quit();
  void reflectBlock(QAction *action);
  void relaxInfo();
  void repaintStatusBar();
  void rotateBlock(QAction *action);
  void saveDD(); 
  void scaleArrow();
  void scalePos();
  void selectPlots();
  void selectFirstPlot();
  void selectLastPlot();
  void selectNextPlot();
  void selectPrevPlot();
  void selectZLayers();
  void setZTolerance();
  void showCells();
  void showCSys();
  void showGb();
  void showZLayers();
  void showPlanes();
  void slot_actXZReflect() { reflectBlock(actXZReflect); }
  void slot_actYZReflect() { reflectBlock(actYZReflect); }
  void slot_actXYReflect() { reflectBlock(actXYReflect); }
  void slot_actXRot180() { rotateBlock(actXRot180); }
  void slot_actYRot180() { rotateBlock(actYRot180); }
  void slot_actZRot180() { rotateBlock(actZRot180); }
  void slot_actXRot90() { rotateBlock(actXRot90); }
  void slot_actYRot90() { rotateBlock(actYRot90); }
  void slot_actZRot90() { rotateBlock(actZRot90); }
  void transform();
  void zoomIn();  
  void zoomOut();
};

#endif
