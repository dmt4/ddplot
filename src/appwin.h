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
  int num = 145;
  QPrinter *printer;
  QTabWidget *tabWidget;
  QLabel *sbar;
  QVector<PltWin *> plotWidget;

  QMenu *mfile, *mexport, *mview, *mplot, *mlayers, *mtran, *mcalcs, *mspecials, *mappear, *mhelp;
  QAction *actOpen, *actSave, *actClose, *actCloseAll, *actPrint, *actExpEPS, *actExpXYZ, *actExpPLT, 
    *actExpCFG, *actExpVTK, *actAnimate, *actQuit;
  QAction *actZoomIn, *actZoomOut, *actFitWin, *actPanRight, *actPanLeft, *actPanUp, *actPanDown;
  QAction *actFirstPlot, *actPrevPlot, *actNextPlot, *actLastPlot, *actSelPlots, *actPanFRight, 
    *actPanFLeft, *actPanFUp, *actPanFDown;
  QAction *actUnrelView, *actRelView, *actCompView, *actMagDisp, *actNumAtom, *actNoComp, *actEdgeComp;
  QAction *actScrewComp, *actDAtLayers, *actDAtTypes, *actDAtNeighbors, *actInertAtom, *actBurgVect, *actDefDPos, *actDCenter, 
    *actSPlanes, *actShowCSys, *actShowCells, *actShowGb, *actRelaxInfo;
  QAction *actShowZLay, *actSetZLay, *actZLayUp, *actZLayDown, *actCoordAt;
  QAction *actCoordSys, *actXZReflect, *actYZReflect, *actXYReflect, *actXRot180, *actYRot180, *actZRot180, 
    *actXRot90, *actYRot90, *actZRot90, *actTransform, *actProjVect;
  QAction *actScalePos, *actScaleArr, *actArrNeigh, *actSetZTol;
  QAction *actCompDisp, *actApplyYDiad, *actCorrEdgeBCC, *actFindDisloSites, *actBurgCircuit, *actDPosPeierlsNabarro,
    *actCalcRDF, *actProbeDispl;
  QAction *actLookAndFeel, *actCrystPlot, *actArrPlot, *actAbout;

  AppWin();
  ~AppWin();

protected:
  void closeEvent( QCloseEvent* );
  virtual void resizeEvent( QResizeEvent * );

public slots:
  // declared in appwin-init.cpp
  bool loadFile(QStringList file);
  void loadSettings(QString file);

  // declared in appwin-file.cpp
  void animate();
  void close();
  void closeAll();
  void exportCFG();
  void exportEPS();
  void exportPLT();
  void exportXYZ();
  void exportVTK();
  void open();
  void print();
  void quit();
  void saveDD(); 

  // declared in appwin-view.cpp
  void fitWin();
  void panRight();
  void panLeft();
  void panUp();
  void panDown();
  void panFastRight();
  void panFastLeft();
  void panFastUp();
  void panFastDown();
  void zoomIn();  
  void zoomOut();

  // declared in appwin-plot.cpp
  void burgVect();
  void defineBurgersCircuit_end();
  void defDisloPos();
  void disloCenter();
  void plotAtomLayers();
  void plotAtomTypes();
  void plotAtomNeighbors();
  void plotEdgeComponent();
  void plotNoComponent();
  void plotScrewComponent();
  void plotUnrelaxed();
  void plotRelaxed();
  void plotComposite();
  void plotInertAtoms();
  void printMagDisp();
  void printAtomNum();
  void relaxInfo();
  void selectFirstPlot();
  void selectLastPlot();
  void selectNextPlot();
  void selectPrevPlot();
  void selectPlots();
  void showCells();
  void showCSys();
  void showGb();
  void showPlanes();

  // declared in appwin-layers.cpp
  void coordAtoms();
  void moveZLayersDown();
  void moveZLayersUp();
  void showZLayers();  
  void selectZLayers();

  // declared in appwin-transforms.cpp
  bool coordSys();
  void compareDisp();
  void projVector();
  void reflectBlock(QAction *action);
  void rotateBlock(QAction *action);
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

  // declared in appwin-calculations.cpp
  void calcRDF();
  void calcDPosPeierlsNabarro_interm();
  void calcDPosPeierlsNabarro_end();
  void defineBurgersCircuit();
  void dlgDPosPeierlsNabarro();
  void findDisloSites();
  void probeAtomDispl();
  void defineCalcAtomDispl_end();

  // declared in appwin-specials.cpp
  void applyYDiad();
  void arrowsPlot();
  void correctEdgeCompBCC();
  void createMenus();
  void crystalPlot();

  // declared in appwin-appearance.cpp
  void arrNeighbors();
  void lookAndFeel();
  void scaleArrow();
  void scalePos();
  void setZTolerance();
  
  // declared in appwin-help.cpp
  void about();

  // declared in appwin-misc.cpp
  void enableMenuItems(bool flag);
  void repaintStatusBar();
};

#endif
