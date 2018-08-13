#include <qmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>

#include "appwin.h"


/*----------------------------------------------------------------------------------------------------
  Create the main menu and submenu of the application.
----------------------------------------------------------------------------------------------------*/

void AppWin::createMenus()
{
  // menu "File"

  mfile = new QMenu(this);
  Q_CHECK_PTR(mfile);
  mfile = menuBar()->addMenu(tr("File"));

  actOpen = new QAction(tr("Open..."), this);
  actOpen->setShortcut(tr("Ctrl+O"));
  connect(actOpen, SIGNAL(triggered()), this, SLOT(open()));
  mfile->addAction(actOpen);

  actSave = new QAction(tr("Save as .dd..."), this);
  actSave->setShortcut(tr("Ctrl+S"));
  connect(actSave, SIGNAL(triggered()), this, SLOT(saveDD()));
  mfile->addAction(actSave);

  actClose = new QAction(tr("Close"), this);
  actClose->setShortcut(tr("Ctrl+F3"));
  connect(actClose, SIGNAL(triggered()), this, SLOT(close()));
  mfile->addAction(actClose);

  actCloseAll = new QAction(tr("Close all"), this);
  connect(actCloseAll, SIGNAL(triggered()), this, SLOT(closeAll()));
  mfile->addAction(actCloseAll);

  mfile->addSeparator();

  actPrint = new QAction(tr("Print..."), this);
  actPrint->setShortcut(tr("Ctrl+P"));
  connect(actPrint, SIGNAL(triggered()), this, SLOT(print()));
  mfile->addAction(actPrint);

  mfile->addSeparator();

  mexport = new QMenu(mfile);
  Q_CHECK_PTR(mexport);
  mexport = mfile->addMenu(tr("Export"));

    actExpEPS = new QAction(tr("to Postscript..."), this);
    connect(actExpEPS, SIGNAL(triggered()), this, SLOT(exportEPS()));
    mexport->addAction(actExpEPS);

    actExpXYZ = new QAction(tr("to XYZ format (JMol)..."), this);
    connect(actExpXYZ, SIGNAL(triggered()), this, SLOT(exportXYZ()));
    mexport->addAction(actExpXYZ);

    actExpPLT = new QAction(tr("to PLT format (PennBOP)..."), this);
    connect(actExpPLT, SIGNAL(triggered()), this, SLOT(exportPLT()));
    mexport->addAction(actExpPLT);

    actExpCFG = new QAction(tr("to CFG format (AtomEye)..."), this);
    connect(actExpCFG, SIGNAL(triggered()), this, SLOT(exportCFG()));
    mexport->addAction(actExpCFG);

    actExpVTK = new QAction(tr("to VTK format (ParaView)..."), this);
    connect(actExpVTK, SIGNAL(triggered()), this, SLOT(exportVTK()));
    mexport->addAction(actExpVTK);

    mfile->addSeparator();

    actAnimate = new QAction(tr("Animate all frames to .GIF..."), this);
    connect(actAnimate, SIGNAL(triggered()), this, SLOT(animate()));
    mfile->addAction(actAnimate);

  actQuit = new QAction(tr("Quit"), this);
  actQuit->setShortcut(tr("Ctrl+Q"));
  connect(actQuit, SIGNAL(triggered()), this, SLOT(quit()));
  mfile->addAction(actQuit);

  //----------------------------------------------------------------------------
  // menu "View"

  mview = new QMenu( this );
  Q_CHECK_PTR( mview );
  mview = menuBar()->addMenu("View");

  actZoomIn = new QAction(tr("Zoom in"), this);
  actZoomIn->setShortcut(tr("+"));
  connect(actZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
  mview->addAction(actZoomIn);

  actZoomOut = new QAction(tr("Zoom out"), this);
  actZoomOut->setShortcut(tr("-"));
  connect(actZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
  mview->addAction(actZoomOut);

  actFitWin = new QAction(tr("Fit window"), this);
  actFitWin->setShortcut(tr("*"));
  connect(actFitWin, SIGNAL(triggered()), this, SLOT(fitWin()));
  mview->addAction(actFitWin);

  mview->addSeparator();

  actPanRight = new QAction(tr("Pan right"), this);
  actPanRight->setShortcut(Qt::SHIFT+Qt::Key_Right);
  connect(actPanRight, SIGNAL(triggered()), this, SLOT(panRight()));
  mview->addAction(actPanRight);

  actPanLeft = new QAction(tr("Pan left"), this);
  actPanLeft->setShortcut(Qt::SHIFT+Qt::Key_Left);
  connect(actPanLeft, SIGNAL(triggered()), this, SLOT(panLeft()));
  mview->addAction(actPanLeft);

  actPanUp = new QAction(tr("Pan up"), this);
  actPanUp->setShortcut(Qt::SHIFT+Qt::Key_Up);
  connect(actPanUp, SIGNAL(triggered()), this, SLOT(panUp()));
  mview->addAction(actPanUp);

  actPanDown = new QAction(tr("Pan down"), this);
  actPanDown->setShortcut(Qt::SHIFT+Qt::Key_Down);
  connect(actPanDown, SIGNAL(triggered()), this, SLOT(panDown()));
  mview->addAction(actPanDown);

  mview->addSeparator();

  actPanFRight = new QAction(tr("Pan fast right"), this);
  actPanFRight->setShortcut(Qt::CTRL+Qt::Key_Right);
  connect(actPanFRight, SIGNAL(triggered()), this, SLOT(panFastRight()));
  mview->addAction(actPanFRight);

  actPanFLeft = new QAction(tr("Pan fast left"), this);
  actPanFLeft->setShortcut(Qt::CTRL+Qt::Key_Left);
  connect(actPanFLeft, SIGNAL(triggered()), this, SLOT(panFastLeft()));
  mview->addAction(actPanFLeft);

  actPanFUp = new QAction(tr("Pan fast up"), this);
  actPanFUp->setShortcut(Qt::CTRL+Qt::Key_Up);
  connect(actPanFUp, SIGNAL(triggered()), this, SLOT(panFastUp()));
  mview->addAction(actPanFUp);

  actPanFDown = new QAction(tr("Pan fast down"), this);
  actPanFDown->setShortcut(Qt::CTRL+Qt::Key_Down);
  connect(actPanFDown, SIGNAL(triggered()), this, SLOT(panFastDown()));
  mview->addAction(actPanFDown);

  // menu "Plot"

  mplot = new QMenu( this );
  Q_CHECK_PTR( mplot );
  mplot = menuBar()->addMenu("Plot");

  actSelPlots = new QAction(tr("Select plots..."), this);
  connect(actSelPlots, SIGNAL(triggered()), this, SLOT(selectPlots()));
  mplot->addAction(actSelPlots);

  actFirstPlot = new QAction(tr("First plot"), this);
  actFirstPlot->setShortcut(Qt::Key_Home);
  connect(actFirstPlot, SIGNAL(triggered()), this, SLOT(selectFirstPlot()));
  mplot->addAction(actFirstPlot);

  actPrevPlot = new QAction(tr("Previous plot"), this);
  actPrevPlot->setShortcut(Qt::Key_Left);
  connect(actPrevPlot, SIGNAL(triggered()), this, SLOT(selectPrevPlot()));
  mplot->addAction(actPrevPlot);

  actNextPlot = new QAction(tr("Next plot"), this);
  actNextPlot->setShortcut(Qt::Key_Right);
  connect(actNextPlot, SIGNAL(triggered()), this, SLOT(selectNextPlot()));
  mplot->addAction(actNextPlot);

  actLastPlot = new QAction(tr("Last plot"), this);
  actLastPlot->setShortcut(Qt::Key_End);
  connect(actLastPlot, SIGNAL(triggered()), this, SLOT(selectLastPlot()));
  mplot->addAction(actLastPlot);

  mplot->addSeparator();

  actUnrelView = new QAction(tr("Unrelaxed configuration"), this);
  actUnrelView->setShortcut(tr("U"));
  connect(actUnrelView, SIGNAL(triggered()), this, SLOT(plotUnrelaxed()));
  mplot->addAction(actUnrelView);

  actRelView = new QAction(tr("Relaxed configuration"), this);
  actRelView->setShortcut(tr("R"));
  connect(actRelView, SIGNAL(triggered()), this, SLOT(plotRelaxed()));
  mplot->addAction(actRelView);

  actCompView = new QAction(tr("Composite view (relaxed+unrelaxed)"), this);
  actCompView->setShortcut(tr("C"));
  connect(actCompView, SIGNAL(triggered()), this, SLOT(plotComposite()));
  mplot->addAction(actCompView);

  mplot->addSeparator();

  actMagDisp = new QAction(tr("Print magnitudes of displacements"), this);
  actMagDisp->setShortcut(tr("M"));
  connect(actMagDisp, SIGNAL(triggered()), this, SLOT(printMagDisp()));
  mplot->addAction(actMagDisp);

  actNumAtom = new QAction(tr("Print numbers of atoms"), this);
  actNumAtom->setShortcut(tr("N"));
  connect(actNumAtom, SIGNAL(triggered()), this, SLOT(printAtomNum()));
  mplot->addAction(actNumAtom);

  mplot->addSeparator();

  actNoComp = new QAction(tr("No displacement component"), this);
  connect(actNoComp, SIGNAL(triggered()), this, SLOT(plotNoComponent()));
  mplot->addAction(actNoComp);

  actEdgeComp = new QAction(tr("Edge component"), this);
  actEdgeComp->setShortcut(tr("E"));
  connect(actEdgeComp, SIGNAL(triggered()), this, SLOT(plotEdgeComponent()));
  mplot->addAction(actEdgeComp);

  actScrewComp = new QAction(tr("Screw component"), this);
  actScrewComp->setShortcut(tr("S"));
  connect(actScrewComp, SIGNAL(triggered()), this, SLOT(plotScrewComponent()));
  mplot->addAction(actScrewComp);

  mplot->addSeparator();

  actDAtLayers = new QAction(tr("Distinguish atomic layers"), this);
  actDAtLayers->setShortcut(tr("L"));
  connect(actDAtLayers, SIGNAL(triggered()), this, SLOT(plotAtomLayers()));
  mplot->addAction(actDAtLayers);

  actDAtTypes = new QAction(tr("Distinguish atomic types"), this);
  actDAtTypes->setShortcut(tr("T"));
  connect(actDAtTypes, SIGNAL(triggered()), this, SLOT(plotAtomTypes()));
  mplot->addAction(actDAtTypes);

  actDAtNeighbors = new QAction(tr("Distinguish by number of neighbors"), this);
  actDAtNeighbors->setShortcut(tr("G"));
  connect(actDAtNeighbors, SIGNAL(triggered()), this, SLOT(plotAtomNeighbors()));
  mplot->addAction(actDAtNeighbors);

  actInertAtom = new QAction(tr("Show inert atoms"), this);
  actInertAtom->setShortcut(tr("I"));
  connect(actInertAtom, SIGNAL(triggered()), this, SLOT(plotInertAtoms()));
  mplot->addAction(actInertAtom);

  mplot->addSeparator();

  actBurgVect = new QAction(tr("Define the Burgers vector..."), this);
  connect(actBurgVect, SIGNAL(triggered()), this, SLOT(burgVect()));
  mplot->addAction(actBurgVect);

  actDefDPos = new QAction(tr("Define dislocation position..."), this);
  connect(actDefDPos, SIGNAL(triggered()), this, SLOT(defDisloPos()));
  mplot->addAction(actDefDPos);
  
  actDCenter = new QAction(tr("Show dislocation position"), this);
  connect(actDCenter, SIGNAL(triggered()), this, SLOT(disloCenter()));
  mplot->addAction(actDCenter);

  actSPlanes = new QAction(tr("Show traces of planes..."), this);
  connect(actSPlanes, SIGNAL(triggered()), this, SLOT(showPlanes()));
  mplot->addAction(actSPlanes);

  actShowCSys = new QAction(tr("Show coordinate system"), this);
  actShowCSys->setShortcut(tr("Ctrl+C"));
  connect(actShowCSys, SIGNAL(triggered()), this, SLOT(showCSys()));
  mplot->addAction(actShowCSys);

  actShowCells = new QAction(tr("Show cells of the linked neighbor list"), this);
  connect(actShowCells, SIGNAL(triggered()), this, SLOT(showCells()));
  mplot->addAction(actShowCells);

  mplot->addSeparator();

  actShowGb = new QAction(tr("Show grain boundary"), this);
  connect(actShowGb, SIGNAL(triggered()), this, SLOT(showGb()));
  mplot->addAction(actShowGb);

  mplot->addSeparator();

  actRelaxInfo = new QAction(tr("Display relaxation info..."), this);
  connect(actRelaxInfo, SIGNAL(triggered()), this, SLOT(relaxInfo()));
  mplot->addAction(actRelaxInfo);

  actUnrelView->setCheckable(true); actUnrelView->setChecked(ATOMPOS==UNRELAXED);
  actRelView->setCheckable(true); actRelView->setChecked(ATOMPOS==RELAXED);
  actCompView->setCheckable(true); actCompView->setChecked(ATOMPOS==COMPOSITE);
  actShowGb->setCheckable(true); actShowGb->setChecked(SHOWGB);

  actMagDisp->setCheckable(true); actMagDisp->setChecked(PRINT_MAG_DISP);
  actNumAtom->setCheckable(true); actNumAtom->setChecked(PRINT_NUM_ATOM);

  actNoComp->setCheckable(true); actNoComp->setChecked(DISP_COMPONENT==NONE);
  actEdgeComp->setCheckable(true); actEdgeComp->setChecked(DISP_COMPONENT==EDGE);
  actScrewComp->setCheckable(true); actScrewComp->setChecked(DISP_COMPONENT==SCREW);

  actDAtLayers->setCheckable(true); actDAtLayers->setChecked(PLOT_TYPE==PLOT_ATOM_LAYERS);
  actDAtTypes->setCheckable(true); actDAtTypes->setChecked(PLOT_TYPE==PLOT_ATOM_TYPES);
  actDAtNeighbors->setCheckable(true); actDAtNeighbors->setChecked(PLOT_TYPE==PLOT_ATOM_NEIGHBORS);
  actInertAtom->setCheckable(true); actInertAtom->setChecked(false);

  actDCenter->setCheckable(true); actDCenter->setChecked(SHOW_DCENTER);
  actShowCSys->setCheckable(true); actShowCSys->setChecked(SHOW_CSYS);
  actShowCells->setCheckable(true); actShowCells->setChecked(SHOW_NEIGHCELLS);

  //----------------------------------------------------------------------------
  // menu "Layers"

  mlayers = new QMenu( this );
  Q_CHECK_PTR( mlayers );
  mlayers = menuBar()->addMenu(tr("Layers"));

  actCoordAt = new QAction(tr("Coordinates of atoms..."), this);
  connect(actCoordAt, SIGNAL(triggered()), this, SLOT(coordAtoms()));
  mlayers->addAction(actCoordAt);

  mlayers->addSeparator();
    
  actSetZLay = new QAction(tr("Show/hide particular Z-layers..."), this);
  connect(actSetZLay, SIGNAL(triggered()), this, SLOT(selectZLayers()));
  mlayers->addAction(actSetZLay);

  actShowZLay = new QAction(tr("Show the list of Z-layers..."), this);
  actShowZLay->setShortcut(tr("Z"));
  connect(actShowZLay, SIGNAL(triggered()), this, SLOT(showZLayers()));
  mlayers->addAction(actShowZLay);

  actZLayUp = new QAction(tr("Move up in the list of Z-layers"), this);
  actZLayUp->setShortcut(Qt::ALT+Qt::Key_Up);
  connect(actZLayUp, SIGNAL(triggered()), this, SLOT(moveZLayersUp()));
  mlayers->addAction(actZLayUp);

  actZLayDown = new QAction(tr("Move down in the list of Z-layers"), this);
  actZLayDown->setShortcut(Qt::ALT+Qt::Key_Down);
  connect(actZLayDown, SIGNAL(triggered()), this, SLOT(moveZLayersDown()));
  mlayers->addAction(actZLayDown);

  actShowZLay->setCheckable(true);  actShowZLay->setChecked(SHOW_ZLAYERS);

  // menu "Transforms"

  mtran = new QMenu( this );
  Q_CHECK_PTR( mtran );
  mtran = menuBar()->addMenu(tr("Transforms"));

  actCoordSys = new QAction(tr("Give the coordinate system of the block..."), this);
  connect(actCoordSys, SIGNAL(triggered()), this, SLOT(coordSys()));
  mtran->addAction(actCoordSys);

  mtran->addSeparator();

  actXZReflect = new QAction(tr("Reflect through the XZ plane"), this);
  connect(actXZReflect, SIGNAL(triggered()), this, SLOT(slot_actXZReflect()));
  mtran->addAction(actXZReflect);

  actYZReflect = new QAction(tr("Reflect through the YZ plane"), this);
  connect(actYZReflect, SIGNAL(triggered()), this, SLOT(slot_actYZReflect()));
  mtran->addAction(actYZReflect);

  actXYReflect = new QAction(tr("Reflect through the XY plane"), this);
  connect(actXYReflect, SIGNAL(triggered()), this, SLOT(slot_actXYReflect()));
  mtran->addAction(actXYReflect);

  mtran->addSeparator();

  actXRot180 = new QAction(tr("Rotate 180 deg about the X-axis"), this);
  connect(actXRot180, SIGNAL(triggered()), this, SLOT(slot_actXRot180()));
  mtran->addAction(actXRot180);

  actYRot180 = new QAction(tr("Rotate 180 deg about the Y-axis"), this);
  connect(actYRot180, SIGNAL(triggered()), this, SLOT(slot_actYRot180()));
  mtran->addAction(actYRot180);

  actZRot180 = new QAction(tr("Rotate 180 deg about the Z-axis"), this);
  connect(actZRot180, SIGNAL(triggered()), this, SLOT(slot_actZRot180()));
  mtran->addAction(actZRot180);

  actXRot90 = new QAction(tr("Rotate 90 deg about the X-axis"), this);
  connect(actXRot90, SIGNAL(triggered()), this, SLOT(slot_actXRot90()));
  mtran->addAction(actXRot90);

  actYRot90 = new QAction(tr("Rotate 90 deg about the Y-axis"), this);
  connect(actYRot90, SIGNAL(triggered()), this, SLOT(slot_actYRot90()));
  mtran->addAction(actYRot90);

  actZRot90 = new QAction(tr("Rotate 90 deg about the Z-axis"), this);
  connect(actZRot90, SIGNAL(triggered()), this, SLOT(slot_actZRot90()));
  mtran->addAction(actZRot90);

  mtran->addSeparator();

  actTransform = new QAction(tr("User-defined transformation..."), this);
  connect(actTransform, SIGNAL(triggered()), this, SLOT(transform()));
  mtran->addAction(actTransform);

  actProjVect = new QAction(tr("Project displacements in direction..."), this);
  actProjVect->setShortcut(tr("P"));
  connect(actProjVect, SIGNAL(triggered()), this, SLOT(projVector()));
  mtran->addAction(actProjVect);

  // menu "Calculations"

  mcalcs = new QMenu( this );
  Q_CHECK_PTR( mcalcs );
  mcalcs = menuBar()->addMenu(tr("Calculations"));

  actCompDisp = new QAction(tr("Difference between relative displacements in two plots..."), this);
  connect(actCompDisp, SIGNAL(triggered()), this, SLOT(compareDisp()));
  mcalcs->addAction(actCompDisp);

  actCalcRDF = new QAction(tr("Radial distribution function..."), this);
  connect(actCalcRDF, SIGNAL(triggered()), this, SLOT(calcRDF()));
  mcalcs->addAction(actCalcRDF);

  actProbeDispl = new QAction(tr("Probe atomic displacements..."), this);
  connect(actProbeDispl, SIGNAL(triggered()), this, SLOT(probeAtomDispl()));
  mcalcs->addAction(actProbeDispl);

  mcalcs->addSeparator();

#ifdef EXPERIMENTAL  
  actFindDisloSites = new QAction(tr("Find the dislocation sites... [EXPERIMENTAL] "), this);
  connect(actFindDisloSites, SIGNAL(triggered()), this, SLOT(findDisloSites()));
  mcalcs->addAction(actFindDisloSites);
#endif

  actBurgCircuit = new QAction(tr("Calculate the Burgers vector using the Burgers circuit..."), this);
  connect(actBurgCircuit, SIGNAL(triggered()), this, SLOT(defineBurgersCircuit()));
  mcalcs->addAction(actBurgCircuit);

  actDPosPeierlsNabarro = new QAction(tr("Find the dislocation position/pathway..."), this);
  connect(actDPosPeierlsNabarro, SIGNAL(triggered()), this, SLOT(dlgDPosPeierlsNabarro()));
  mcalcs->addAction(actDPosPeierlsNabarro);
  
#ifdef EXPERIMENTAL
  mcalcs->addSeparator();
  
  actDPosBurgCircuit = new QAction(tr("Successive decimation of the Burgers circuit... [EXPERIMENTAL]"), this);
  connect(actDPosBurgCircuit, SIGNAL(triggered()), this, SLOT(dlgDPosBurgCircuit()));
  mcalcs->addAction(actDPosBurgCircuit);
#endif

  // menu "Specials"

  mspecials = new QMenu( this );
  Q_CHECK_PTR( mspecials );
  mspecials = menuBar()->addMenu(tr("Specials"));

  actApplyYDiad = new QAction(tr("Apply the Y-axis diad operation"), this);
  connect(actApplyYDiad, SIGNAL(triggered()), this, SLOT(applyYDiad()));
  mspecials->addAction(actApplyYDiad);

  actCorrEdgeBCC = new QAction(tr("Correct the edge components in BCC due to periodicity"), this);
  connect(actCorrEdgeBCC, SIGNAL(triggered()), this, SLOT(correctEdgeCompBCC()));
  mspecials->addAction(actCorrEdgeBCC);

  actCorrEdgeBCC->setCheckable(true); actCorrEdgeBCC->setChecked(CORR_EDGECOMP_BCC);

  //----------------------------------------------------------------------------
  // menu "Appearance"

  mappear = new QMenu( this );
  Q_CHECK_PTR( mappear );
  mappear = menuBar()->addMenu(tr("Appearance"));

  actLookAndFeel = new QAction(tr("Look and feel..."), this);
  connect(actLookAndFeel, SIGNAL(triggered()), this, SLOT(lookAndFeel()));
  mappear->addAction(actLookAndFeel);

  mappear->addSeparator();

  actCrystPlot = new QAction(tr("Crystal structure plotting..."), this);
  connect(actCrystPlot, SIGNAL(triggered()), this, SLOT(crystalPlot()));
  mappear->addAction(actCrystPlot);

  actScalePos = new QAction(tr("Scale atomic positions..."), this);
  connect(actScalePos, SIGNAL(triggered()), this, SLOT(scalePos()));
  mappear->addAction(actScalePos);

  mappear->addSeparator();

  actArrPlot = new QAction(tr("Plotting of arrows..."), this);
  connect(actArrPlot, SIGNAL(triggered()), this, SLOT(arrowsPlot()));
  mappear->addAction(actArrPlot);

  actScaleArr = new QAction(tr("Scale lengths of arrows/symbols..."), this);
  connect(actScaleArr, SIGNAL(triggered()), this, SLOT(scaleArrow()));
  mappear->addAction(actScaleArr);

  actArrNeigh = new QAction(tr("Plot arrows between which neighbors..."), this);
  connect(actArrNeigh, SIGNAL(triggered()), this, SLOT(arrNeighbors()));
  mappear->addAction(actArrNeigh);

  actSetZTol = new QAction(tr("Tolerance for the Z-coordinates of atoms in the same layer..."), this);
  connect(actSetZTol, SIGNAL(triggered()), this, SLOT(setZTolerance()));
  mappear->addAction(actSetZTol);

  // menu "Help"

  mhelp = new QMenu( this );
  Q_CHECK_PTR( mhelp );
  mhelp = menuBar()->addMenu(tr("Help"));

  actAbout = new QAction(tr("About..."), this);
  actAbout->setShortcut(tr("Ctrl+A"));
  connect(actAbout, SIGNAL(triggered()), this, SLOT(about()));
  mhelp->addAction(actAbout);

  sbar = new QLabel(this);
  statusBar()->addWidget(sbar);
  repaintStatusBar();
}


/*----------------------------------------------------------------------------------------------------
  Set the visibility of the individual items in the main menu and submenus.
----------------------------------------------------------------------------------------------------*/

void AppWin::enableMenuItems( bool flag )
{
  mfile->setEnabled(true);
  mview->setEnabled(flag);
  mplot->setEnabled(flag);
  mlayers->setEnabled(flag);
  mcalcs->setEnabled(flag);
  mappear->setEnabled(flag);
  mtran->setEnabled(flag);
  mspecials->setEnabled(flag);
  mhelp->setEnabled(true);

  actSave->setEnabled(flag);
  actClose->setEnabled(flag);
  actCloseAll->setEnabled(flag);
  actPrint->setEnabled(flag);
  mexport->setEnabled(flag);
  actExpEPS->setEnabled(flag);
  actExpXYZ->setEnabled(flag);
  actExpPLT->setEnabled(flag);
  actExpCFG->setEnabled(flag);
  actExpVTK->setEnabled(flag);
  actAnimate->setEnabled(flag);

  actZoomIn->setEnabled(flag);
  actZoomOut->setEnabled(flag);
  actFitWin->setEnabled(flag);
  actPanRight->setEnabled(flag);
  actPanLeft->setEnabled(flag);
  actPanUp->setEnabled(flag);
  actPanDown->setEnabled(flag);
  actPanFRight->setEnabled(flag);
  actPanFLeft->setEnabled(flag);
  actPanFUp->setEnabled(flag);
  actPanFDown->setEnabled(flag);

  actFirstPlot->setEnabled(flag);
  actPrevPlot->setEnabled(flag);
  actNextPlot->setEnabled(flag);
  actLastPlot->setEnabled(flag);
  actSelPlots->setEnabled(flag);
  actUnrelView->setEnabled(flag);
  actRelView->setEnabled(flag);
  actCompView->setEnabled(flag);
  actMagDisp->setEnabled(flag);
  actNumAtom->setEnabled(flag);
  actNoComp->setEnabled(flag);
  actEdgeComp->setEnabled(flag);
  actScrewComp->setEnabled(flag);
  actDAtLayers->setEnabled(flag);
  actDAtTypes->setEnabled(flag);
  actDAtNeighbors->setEnabled(flag);
  actInertAtom->setEnabled(flag);
  actCalcRDF->setEnabled(flag);
  actProbeDispl->setEnabled(flag);  
  actBurgVect->setEnabled(flag);
  actDCenter->setEnabled(flag);
  actSPlanes->setEnabled(flag);
  actShowCSys->setEnabled(flag);
  actShowCells->setEnabled(flag);
  actShowGb->setEnabled(flag);
  actRelaxInfo->setEnabled(flag); 

  actShowZLay->setEnabled(flag);
  actSetZLay->setEnabled(flag);
  actZLayUp->setEnabled(flag);
  actZLayDown->setEnabled(flag);
  actCoordAt->setEnabled(flag);

  actCoordSys->setEnabled(flag);
  actXZReflect->setEnabled(flag);
  actYZReflect->setEnabled(flag);
  actXYReflect->setEnabled(flag);
  actXRot180->setEnabled(flag);
  actYRot180->setEnabled(flag);
  actZRot180->setEnabled(flag);
  actXRot90->setEnabled(flag);
  actYRot90->setEnabled(flag);
  actZRot90->setEnabled(flag);
  actTransform->setEnabled(flag);  
  actProjVect->setEnabled(flag);
  actCompDisp->setEnabled(flag);

  actCrystPlot->setEnabled(flag);
  actArrPlot->setEnabled(flag);
  actScalePos->setEnabled(flag);
  actScaleArr->setEnabled(flag);
  actArrNeigh->setEnabled(flag);
  actSetZTol->setEnabled(flag);

  actBurgCircuit->setEnabled(flag);
  actDPosPeierlsNabarro->setEnabled(flag);
#ifdef EXPERIMENTAL
  actFindDisloSites->setEnabled(flag);
  actDPosBurgCircuit->setEnabled(flag);
#endif

  actApplyYDiad->setEnabled(flag);
  actCorrEdgeBCC->setEnabled(flag);
}
