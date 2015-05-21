#include <assert.h>
#include <math.h>

#include <qcheckbox.h>
#include <qdialog.h>
#include <qgroupbox.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qlistwidget.h>
#include <qpixmap.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qprogressdialog.h>
#include <qradiobutton.h>
#include <qtablewidget.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qprintdialog.h>
#include <qprinter.h>
#include <qspinbox.h>
#include <qtextedit.h>

#include "ui_dabout.h"
#include "ui_danimate.h"
#include "ui_darrneighbors.h"
#include "ui_darrowstyle.h"
#include "ui_datomstyle.h"
#include "ui_dburgvect.h"
#include "ui_dcomparedisp.h"
#include "ui_dcoordatoms.h"
#include "ui_dcoordsys.h"
#include "ui_dprojvector.h"
#include "ui_dscalearrow.h"
#include "ui_dselectplots.h"
#include "ui_dselectzlayers.h"
#include "ui_drelaxinfo.h"
#include "ui_dscalepos.h"
#include "ui_dshowplanes.h"
#include "ui_dtransform.h"
#include "ui_dztolerance.h"

#include "appwin.h"
#include "animate.h"
#include "atomstyle.h"
#include "calcs.h"
#include "fformat.h"
#include "messages.h"
#include "misc.h"
#include "neighlist.h"
#include "scalepos.h"
#include "selectplots.h"
#include "selectzlayers.h"
#include "pltwin.h"
#include "ddplot.h"


void AppWin::about()
{
  Ui::DAbout ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);
  dialog->exec();
  delete( dialog );
}


void AppWin::animate()
{
  PltWin *pw;
  FILE *feps;
  QString outf, cmd, args, animfile, item;
  int n, nbefore, i, pos;
  int density, delay, nloops;
  bool exit, selFlag, rmFiles, anim;

  // checking to see if ImageMagick is present
  exit = system("convert -version > dd_log")==-1;
  if (exit) {
    msgError(err_NoImageMagick);    
    return;
  }
  system("rm -f dd_*");

  Animate *dialog = new Animate;

  for (i=0; i<tabWidget->count(); i++) {
    item = plotWidget[i]->FName;
    
    // only the file name is printed
    pos = item.indexOf('/');  
    if (pos>=0)
      item.remove(0,pos+1);
    
    selFlag = plotWidget[i]->isSelected;

    dialog->plotFiles->addItem(item);
    dialog->plotFiles->setItemSelected(dialog->plotFiles->item(i), selFlag);
  }

  dialog->exec();  
  if (dialog->result()!=QDialog::Accepted)
    return;

  density = dialog->density->value();
  delay = dialog->delay->value();
  nloops = dialog->nloops->value();
  args = dialog->args->text();
  rmFiles = !dialog->rmFiles->isChecked();

  nbefore = tabWidget->currentIndex();

  QProgressDialog pdlg("Collecting plots...", "&Abort", 0, tabWidget->count()-1, this);
  exit = false;

  anim = false;
  for (n=0; n<tabWidget->count(); n++) {
    pdlg.setValue(n);

    selFlag = dialog->plotFiles->isItemSelected(dialog->plotFiles->item(n));
    if (!selFlag)
      continue;

    anim = true;
    pw = plotWidget[n];
    tabWidget->setCurrentIndex(n);

    outf.sprintf("dd_%03d.ps", n);
    feps = fopen(outf.toAscii().data(), "w+");
    fprintf(feps, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(feps, "%%%%BoundingBox: %d %d %d %d\n", 0, 0, pw->width(), pw->height());
    pw->paintEPS = true;
    pw->feps = feps;
    pw->repaint();
    fprintf(feps,"%%%%Trailer\n");
    fclose(feps);

    pw->paintEPS = false;

    if (pdlg.wasCanceled()) {  
      exit = true;
      break;
    }
  }

  pdlg.cancel();
  if (exit || !anim) return;

  tabWidget->setCurrentIndex(nbefore);
  plotWidget[nbefore]->repaint();

  animfile = CreateFName(pw->FName, ".gif");
  QString types("GIF (*.gif)");
  animfile = QFileDialog::getSaveFileName(
    this,
    "Export to a dynamic .GIF file",
    animfile,
    types);
  if (animfile=="") return;

  // calling the ImageMagick
  cmd.sprintf("convert -density %d -delay %d %s dd_*.ps %s",
	      density,delay,args.toAscii().data(),animfile.toAscii().data());
  system(cmd.toAscii().data());
  if (rmFiles) system("rm -f dd_*.ps");
}


// Consider that the Y-axis is a diad (in our bcc block it is the [-101] axis) and apply
// this operation to show the symmetry-related block.
void AppWin::applyYDiad()
{
  reflectBlock(actYZReflect);
  reflectBlock(actXYReflect);
}


void AppWin::arrNeighbors()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString txt;
  int i, j, istart, iend;
  bool ok;

  Ui::DArrNeighbors ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  // set the neighbor distances
  if (pw->arrNeighNum>0 && pw->RdfNNeigh>0) {
    txt.sprintf("R = %0.4f", pw->RdfDist(1));
    ui.Rdist1->setText(txt); 
  }
  if (pw->arrNeighNum>0 && pw->RdfNNeigh>1) {
    txt.sprintf("R = %0.4f", pw->RdfDist(2));
    ui.Rdist2->setText(txt); 
  }
  if (pw->arrNeighNum>0 && pw->RdfNNeigh>2) {
    txt.sprintf("R = %0.4f", pw->RdfDist(3));
    ui.Rdist3->setText(txt); 
  }
  if (pw->arrNeighNum>0 && pw->RdfNNeigh>3) {
    txt.sprintf("R = %0.4f", pw->RdfDist(4));
    ui.Rdist4->setText(txt); 
  }
  if (pw->arrNeighNum>0 && pw->RdfNNeigh>4) {
    txt.sprintf("R = %0.4f", pw->RdfDist(5));
    ui.Rdist5->setText(txt); 
  }

  // set the checkboxes
  for (i=1; i<=pw->arrNeighNum; i++) {
    if (pw->arrNeighbors(i)==1)  ui.Rcheck1->setChecked(1);
    if (pw->arrNeighbors(i)==2)  ui.Rcheck2->setChecked(1);
    if (pw->arrNeighbors(i)==3)  ui.Rcheck3->setChecked(1);
    if (pw->arrNeighbors(i)==4)  ui.Rcheck4->setChecked(1);
    if (pw->arrNeighbors(i)==5)  ui.Rcheck5->setChecked(1);
  }

  // execute the dialog
  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;

  // change settings, if needed
  if (ok) {
    if (ui.applyHow->currentIndex()==0)
      istart = iend = tabWidget->currentIndex();
    else {
      istart = 0;
      iend = tabWidget->count()-1;
    }

    for (i=istart; i<=iend; i++) {
      pw = plotWidget[i];
      if (!pw->isSelected)
	continue;
      
      pw->arrNeighNum = 0;
      if (ui.Rcheck1->isChecked()) {
	pw->arrNeighNum++;
	pw->arrNeighbors(pw->arrNeighNum) = 1;
      }
      if (ui.Rcheck2->isChecked()) {
	pw->arrNeighNum++;
	pw->arrNeighbors(pw->arrNeighNum) = 2;
      }
      if (ui.Rcheck3->isChecked()) {
	pw->arrNeighNum++;
	pw->arrNeighbors(pw->arrNeighNum) = 3;
      }
      if (ui.Rcheck4->isChecked()) {
	pw->arrNeighNum++;
	pw->arrNeighbors(pw->arrNeighNum) = 4;
      }
      if (ui.Rcheck5->isChecked()) {
	pw->arrNeighNum++;
	pw->arrNeighbors(pw->arrNeighNum) = 5;
      }

      if (VERBOSE) {
	if (pw->arrNeighNum>0) {
	  printf("* the arrows will be plotted between these nearest neighbors:\n");
	  printf("    ");
	  for (j=1; j<=pw->arrNeighNum; j++)
	    printf("%d ", pw->arrNeighbors(j));
	  printf("\n"); 
	}
      }

      // re-calculate the linked neighbor list
      InitNeighborList( pw, pw->NeighListInit );

      // must re-calculate the relative displacements - arrows plotted between different neighbors
      if (pw->DispComponent==EDGE)
	pw->calcEdgeRelDisp();
      else if (pw->DispComponent==SCREW)
	pw->calcScrewRelDisp();
    }

    // repaint only the visible widget
    plotWidget[ tabWidget->currentIndex() ]->repaint();
  }
}


void AppWin::arrowsPlot()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  int NPlots = tabWidget->count();
  int i,istart,iend;
  bool ok;

  Ui::DArrowStyle ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  ui.ithickness->setValue( pw->thicknessArrow );
  ui.ithickness->selectAll();
  ui.ishortest->setValue( pw->shortestArrow );

  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;

  if (ok) {
    // to which plots should the settings be applied
    if (ui.applyHow->currentIndex()==0)
      istart = iend = tabWidget->currentIndex();
    else {
      istart = 0;
      iend = tabWidget->count()-1;
    }

    // apply the new arrow thickness
    for (i=istart; i<=iend; i++) {
      pw = plotWidget[i];
      if (!pw->isSelected)
	continue;

      pw->thicknessArrow = ui.ithickness->text().toInt();
      pw->shortestArrow = ui.ishortest->text().toInt();
      pw->repaint();
    }
  }
}


void AppWin::burgVect()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString str;
  int i, istart, iend;
  bool ok;

  Ui::DBurgVect ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  str.sprintf("%0.4lf", pw->Bvect(1));
  ui.xcoord->setText(str);
  ui.xcoord->selectAll();
  str.sprintf("%0.4lf", pw->Bvect(2));
  ui.ycoord->setText(str);
  str.sprintf("%0.4lf", pw->Bvect(3));
  ui.zcoord->setText(str);

  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;

  if (ok) {
    if (ui.applyHow->currentIndex()==0)
      istart = iend = tabWidget->currentIndex();
    else {
      istart = 0;
      iend = tabWidget->count()-1;
    }
    
    for (i=istart; i<=iend; i++) {
      pw = plotWidget[i];
      if (!pw->isSelected)
	continue;

      pw->Bvect(1) = ui.xcoord->text().toDouble();
      pw->Bvect(2) = ui.ycoord->text().toDouble();
      pw->Bvect(3) = ui.zcoord->text().toDouble();

      // must re-calculate the relative displacements - arrows plotted between different neighbors
      if (pw->DispComponent==EDGE)
	pw->calcEdgeRelDisp();
      else if (pw->DispComponent==SCREW)
	pw->calcScrewRelDisp();

      pw->repaint();
    }
  }
}


void AppWin::close()
{
  int NCurrentPlot = tabWidget->currentIndex();
  int NPlots = tabWidget->count();
  int i;


  tabWidget->removeTab( NCurrentPlot );
  delete( plotWidget[NCurrentPlot] );

  for (i=NCurrentPlot; i<NPlots; i++)
    plotWidget[i] = plotWidget[i+1];

  // the deleted plot was the last one
  if (NCurrentPlot>=tabWidget->count())
    NCurrentPlot = tabWidget->count()-1;

  if (tabWidget->count()==0) {
    delete( tabWidget );
    tabWidget = NULL;
    enableMenuItems(false);
  } else {
    tabWidget->setCurrentIndex( NCurrentPlot );
    repaintStatusBar();
  }
}


void AppWin::closeAll()
{
  int n;
  int NPlots = tabWidget->count();

  for (n=0; n<NPlots; n++) {
    tabWidget->removeTab( n );
    delete( plotWidget[n] );
  }

  delete( tabWidget );
  tabWidget = NULL;

  repaintStatusBar();
  enableMenuItems(false);
}


void AppWin::compareDisp()
{
  QString item,cstr;
  int i, pos, p1, p2, NCurrentPlot, n, d, comp, idx;
  bool ok, comparable;

  Ui::DCompareDisp ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  // set up the data
  for (i=0; i<tabWidget->count(); i++) {
    item = plotWidget[i]->FName;
    
    // only the file name is printed
    pos = item.indexOf('/');  
    if (pos>=0)
      item.remove(0,pos+1);
    
    ui.plot1->addItem(item);
    ui.plot2->addItem(item);
  }

  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;

  if (ok) {
    NCurrentPlot = tabWidget->count();
    if (NCurrentPlot>=MAX_PLOTS) {
      msgError(err_TooManyPlots);
      return;
    }

    p1 = ui.plot1->currentRow();
    p2 = ui.plot2->currentRow();
    if (p1==p2) {
      msgInfo(info_CompareSamePlots);
      return;
    }

    comp = EDGE * ui.compEdge->isChecked() + 
      SCREW * ui.compScrew->isChecked();

    if (comp==EDGE) 
      cstr = QString(" (edge)");
    else 
      cstr = QString(" (screw)");

    // create a new widget
    plotWidget[NCurrentPlot] = new PltWin( tabWidget );
    plotWidget[NCurrentPlot]->copy( plotWidget[p1] );
    plotWidget[NCurrentPlot]->xyzRel = plotWidget[NCurrentPlot]->xyzInit;

    // set the plot-specific parameters
    if (comp==EDGE)
      plotWidget[NCurrentPlot]->DispComponent = DIFF_EDGE;  
    else if (comp==SCREW)
      plotWidget[NCurrentPlot]->DispComponent = DIFF_SCREW;     
    plotWidget[NCurrentPlot]->FName = tabWidget->tabText(p1) + " (-) " + 
      tabWidget->tabText(p2) + cstr.toAscii().data();

    comparable = plotWidget[NCurrentPlot]->CompareDisp( plotWidget[p1], plotWidget[p2], comp );
    if (!comparable) {
      delete(plotWidget[NCurrentPlot]);
      msgError(err_CannotComparePlots);
      return;
    }

    tabWidget->addTab( plotWidget[NCurrentPlot], plotWidget[NCurrentPlot]->FName );
    tabWidget->setCurrentIndex( NCurrentPlot );

    idx = tabWidget->currentIndex();
    actFirstPlot->setEnabled(idx > 0);
    actPrevPlot->setEnabled(idx > 0);
    actNextPlot->setEnabled(idx < tabWidget->count()-1);
    actLastPlot->setEnabled(idx < tabWidget->count()-1);

    repaintStatusBar();
  }
}


void AppWin::coordAtoms()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QTableWidgetItem witem;
  QString str;
  int i, d;
  bool ok;

  Ui::DCoordAtoms ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);
  ui.coordTable->setRowCount(pw->NInit);

  for (i=1; i<=pw->NInit; i++) {
    for (d=1; d<=3; d++) {
      str.sprintf("%0.4f", pw->xyzInit(i,d));
      QTableWidgetItem *witem1 = new QTableWidgetItem(str);
      ui.coordTable->setItem(i-1, d-1, witem1);
      str.sprintf("%0.4f", pw->xyzRel(i,d));
      QTableWidgetItem *witem2 = new QTableWidgetItem(str);
      ui.coordTable->setItem(i-1, d+2, witem2);
    }
  }

  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;
}


bool AppWin::coordSys()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  int istart, iend, i;
  bool ok;

  Ui::DCoordSys ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  ui.x1->insert( QString::number( pw->csys(1,1) ));
  ui.x1->selectAll();
  ui.x2->insert( QString::number( pw->csys(1,2) ));
  ui.x3->insert( QString::number( pw->csys(1,3) ));
  ui.y1->insert( QString::number( pw->csys(2,1) ));
  ui.y2->insert( QString::number( pw->csys(2,2) ));
  ui.y3->insert( QString::number( pw->csys(2,3) ));
  ui.z1->insert( QString::number( pw->csys(3,1) ));
  ui.z2->insert( QString::number( pw->csys(3,2) ));
  ui.z3->insert( QString::number( pw->csys(3,3) ));

  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;

  if (ok) {
    if (ui.applyHow->currentIndex()==0)
      istart = iend = tabWidget->currentIndex();
    else {
      istart = 0;
      iend = tabWidget->count()-1;
    }
    
    // apply the new magnification ratio
    for (i=istart; i<=iend; i++) {
      pw = plotWidget[i];
      if (!pw->isSelected)
	continue;

      pw->csys(1,1) = ui.x1->text().toDouble();
      pw->csys(1,2) = ui.x2->text().toDouble();
      pw->csys(1,3) = ui.x3->text().toDouble();
      pw->csys(2,1) = ui.y1->text().toDouble();
      pw->csys(2,2) = ui.y2->text().toDouble();
      pw->csys(2,3) = ui.y3->text().toDouble();
      pw->csys(3,1) = ui.z1->text().toDouble();
      pw->csys(3,2) = ui.z2->text().toDouble();
      pw->csys(3,3) = ui.z3->text().toDouble();
      pw->repaint();
    }
  }

  return ok;
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


void AppWin::crystalPlot()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  int i,n,nrows;
  bool ok;

  // declaration
  AtomStyle *dialog = new AtomStyle(pw);
  dialog->setData();

  // execution
  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;

  // change of settings
  if (ok) {
    for (i=1; i<=pw->NumZLayers; i++) {
      pw->zDiamLayer(i) = dialog->atomTable->item(i-1,1)->text().toInt();
      pw->zLineThickLayer(i) = dialog->atomTable->item(i-1,2)->text().toInt();
      
      pw->zColorLayer(i,1) = dialog->zColor(i,1);
      pw->zColorLayer(i,2) = dialog->zColor(i,2);
    }
    pw->repaint();    
  }
}


void AppWin::disloCenter()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  SHOW_DCENTER = !SHOW_DCENTER;
  actDCenter->setChecked(SHOW_DCENTER);
  pw->DisloCenter = SHOW_DCENTER; 
  pw->repaint();
  repaintStatusBar();
}


void AppWin::enableMenuItems( bool flag )
{
  actSave->setEnabled(flag);
  actClose->setEnabled(flag);
  actCloseAll->setEnabled(flag);
  actPrint->setEnabled(flag);
  mexport->setEnabled(flag);
  actExpEPS->setEnabled(flag);
  actExpXYZ->setEnabled(flag);
  actExpPLT->setEnabled(flag);
  actExpCFG->setEnabled(flag);
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
  actInertAtom->setEnabled(flag);  
  actBurgVect->setEnabled(flag);
  actDCenter->setEnabled(flag);
  actSPlanes->setEnabled(flag);
  actShowCSys->setEnabled(flag);
  actShowCells->setEnabled(flag);
  actShowGb->setEnabled(flag);
  actRelaxInfo->setEnabled(flag); 

  actShowZLay->setEnabled(flag);
  actSetZLay->setEnabled(flag);
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

  actApplyYDiad->setEnabled(flag);
  actCorrEdgeBCC->setEnabled(flag);
}


void AppWin::exportCFG()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString aname, cfgfile, msg;
  DVector s(3);
  double amass;
  int i, j, d, atype;
  FILE *fcfg;

  cfgfile = CreateFName(pw->FName, ".cfg");
  QString types("CFG file (*.cfg)");
  cfgfile = QFileDialog::getSaveFileName(
    this,
    "Export to CFG format (AtomEye)",
    cfgfile,
    types);
  if (cfgfile=="") return;

  fcfg = fopen(cfgfile.toAscii().data(), "w+");

  fprintf(fcfg, "Number of particles = %d\n\n", pw->NInit+pw->NInert);
  fprintf(fcfg, "A = 10 Angstrom (basic length scale)\n\n");

  // supercell's first edge (x-size of the block)
  fprintf(fcfg, "H0(1,1) = %0.4f A\n", pw->blSize(1));
  fprintf(fcfg, "H0(1,2) = 0.0 A\n");
  fprintf(fcfg, "H0(1,3) = 0.0 A\n\n");

  // supercell's second edge (y-size of the block)
  fprintf(fcfg, "H0(2,1) = 0.0 A\n");
  fprintf(fcfg, "H0(2,2) = %0.4f A\n", pw->blSize(2));
  fprintf(fcfg, "H0(2,3) = 0.0 A\n\n");

  // supercell's third edge (z-size of the block)
  fprintf(fcfg, "H0(3,1) = 0.0 A\n");
  fprintf(fcfg, "H0(3,2) = 0.0 A\n");
  fprintf(fcfg, "H0(3,3) = %0.4f A\n\n", pw->blSize(3));

  // coordinates of atoms - initial configuration
  for (i=1; i<=pw->NInit; i++) {
    atype = pw->atomType(i);
    aname = ATOM_NAME[atype];
    amass = AtomicMass(aname.toUpper());

    // reduced coordinates (between 0 and 1)
    for (d=1; d<=3; d++)
      s(d) = (pw->xyzInit(i,d) - pw->xyzMin(d)) / pw->blSize(d);

    fprintf(fcfg,"%0.4f  %2s  %6.4f  %6.4f  %6.4f  0  0  0\n",
	    amass, aname.toAscii().data(), s(1), s(2), s(3));
  }

  for (i=1; i<=pw->NInert; i++) {
    aname = "H";
    amass = 1.0;

    // reduced coordinates (between 0 and 1)                                                               
    for (d=1; d<=3; d++)
      s(d) = (pw->xyzInert(i,d) - pw->xyzMin(d)) / pw->blSize(d);

    fprintf(fcfg,"%0.4f  %2s  %6.4f  %6.4f  %6.4f  0  0  0\n",
            amass, aname.toAscii().data(), s(1), s(2), s(3));
  }
  
  fclose(fcfg);
}


void AppWin::exportEPS()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString epsfile, msg;
  QColor col;
  FILE *feps;
  int n, xpos, ypos, dia, rad, lw;

  epsfile = CreateFName(pw->FName, ".eps");
  QString types("Postscript (*.ps *.eps)");
  epsfile = QFileDialog::getSaveFileName(
    this,
    "Export to Postscript",
    epsfile,
    types);
  if (epsfile=="") return;

  feps = fopen(epsfile.toAscii().data(), "w+");
  fprintf(feps, "%%!PS-Adobe-3.0 EPSF-3.0\n");
  fprintf(feps, "%%%%BoundingBox: %d %d %d %d\n", 0, 0, pw->width(), pw->height());
  pw->paintEPS = true;
  pw->feps = feps;
  pw->repaint();
  fprintf(feps,"%%%%Trailer\n");
  fclose(feps);

  pw->paintEPS = false;
  pw->repaint();
}


void AppWin::exportPLT()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString aname, pltfile, msg;
  FILE *f;
  int n,atype;

  pltfile = CreateFName(pw->FName,".plt");
  QString types( "PLT file (*.plt)");
  pltfile = QFileDialog::getSaveFileName(
    this,
    "Export to PLT format (PennBOP)",
    pltfile,
    types);
  if (pltfile=="") return;

  f = fopen(pltfile.toAscii().data(), "w+");

  fprintf(f,"%d\n",pw->NRel);
  for (n=1; n<=pw->NRel; n++) 
    fprintf(f,"%12.6lf\n", pw->xyzRel(n,3));
  for (n=1; n<=pw->NRel; n++)
    fprintf(f,"%12.6lf  %12.6lf  %d\n", pw->xyzRel(n,1), pw->xyzRel(n,2), pw->atomType(n));

  fprintf(f,"%d\n",pw->NInit);
  for (n=1; n<=pw->NInit; n++) 
    fprintf(f,"%12.6lf\n", pw->xyzInit(n,3));
  for (n=1; n<=pw->NInit; n++)
    fprintf(f,"%12.6lf  %12.6lf\n", pw->xyzInit(n,1), pw->xyzInit(n,2));

  fprintf(f,"%d\n",0);
  fprintf(f,"%12.6f\n", pw->period(1));
  fprintf(f,"%12.6f\n", pw->period(2));
  fprintf(f,"%12.6f\n", pw->period(3));
  fprintf(f,"CORE { %lf;%lf }\n", pw->xCore, pw->yCore);

  fclose(f);
}


void AppWin::exportXYZ()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString aname, xyzfile, msg;
  FILE *f;
  int n,atype;

  xyzfile = CreateFName(pw->FName,".xyz");
  QString types( "XYZ file (*.xyz)");
  xyzfile = QFileDialog::getSaveFileName(
    this,
    "Export to XYZ format (JMol/RasMol)",
    xyzfile,
    types);
  if (xyzfile=="") return;

  f = fopen(xyzfile.toAscii().data(), "w+");

  fprintf(f,"%d\n",pw->NRel+pw->NInert);
  fprintf(f,"Place a comment here\n");
  for (n=1; n<=pw->NRel; n++) {
    atype = pw->atomType(n);
    aname = ATOM_NAME[atype];
    fprintf(f,"%s  %12.6lf  %12.6lf  %12.6lf\n",
	    aname.toAscii().data(), pw->xyzRel(n,1), pw->xyzRel(n,2), pw->xyzRel(n,3));
  }
  for (n=1; n<=pw->NInert; n++) {
    aname = "H";
    fprintf(f,"%s  %12.6lf  %12.6lf  %12.6lf\n",
            aname.toAscii().data(), pw->xyzInert(n,1), pw->xyzInert(n,2), pw->xyzInert(n,3));
  }

  fclose(f);
}


void AppWin::fitWin()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->xPan = 0;
  pw->yPan = 0;
  pw->ZFact = 1;
  pw->repaint();

  repaintStatusBar();
}


void AppWin::interpretCommand(QString cmd)
{
  QString params[100];
  int npar,idx,i,j,id;
  int angle;
  bool alldone=false;

  // get rid of comments
  idx = cmd.indexOf('#');   if (idx>=0) cmd.truncate(idx);
  idx = cmd.indexOf('\n');  if (idx>=0) cmd.truncate(idx);
  idx = cmd.indexOf('\r');  if (idx>=0) cmd.truncate(idx);  // Linux terminates with \r\n

  if (cmd.isEmpty())
    return;

  for (npar=0; npar<100; npar++) {
    params[npar] = cmd.section(',',npar,npar);
    if (params[npar].isEmpty())
      break;
    params[npar] = params[npar].trimmed();    // get rid of spaces
  }

  if (npar<1)
    return;

  params[0] = params[0].toUpper();

  //-------------------------------------------
  // interpretation of the commands read above
  //-------------------------------------------

  if (params[0]=="ATOM.DIAMETER") {
    for (i=1; i<=npar-1; i++)
      ATOM_DIAM[i-1] = params[i].toInt();
    NUM_DIAM = npar-1;
  }

  if (params[0]=="ATOM.THICKNESS") {
    for (i=1; i<=npar-1; i++)
      ATOM_THICKNESS[i-1] = params[i].toInt();
    NUM_THICKNESS = npar-1;
  }

  if (params[0]=="ATOM.FGCOLOR") {
    for (i=1; i<=npar-1; i++)
      ATOM_FGCOLOR[i-1] = params[i];
    NUM_FGCOLORS = npar-1;
  }

  if (params[0]=="ATOM.BGCOLOR") {
    for (i=1; i<=npar-1; i++)
      ATOM_BGCOLOR[i-1] = params[i];
    NUM_BGCOLORS = npar-1;
  }

  if (params[0]=="ANAME") {
    id = params[1].toInt();
    ATOM_NAME[id] = params[2];
  }

  if (params[0]=="ATOMNUMBERS") {
    ATOM_NUMBERS = (params[1]=="on");
  }
      
  if (params[0]=="ARRNEIGHBORS") {
    ARR_NEIGHNUM = npar-1;
    for (i=1; i<=npar-1; i++)   
      ARR_NEIGHBORS[i-1] = params[i].toInt();

    if (ARR_NEIGHNUM==1 && ARR_NEIGHBORS[0]==0)
      ARR_NEIGHNUM = 0;
  }

  if (params[0]=="ADISTINGUISH") {
    if (params[1]=="layer")
      PLOT_TYPE = PLOT_ATOM_LAYERS;

    if (params[1]=="type")
      PLOT_TYPE = PLOT_ATOM_TYPES;
  }

  if (params[0]=="ARROW.SHORTEST")          
    ARROW_SHORTEST = params[1].toInt();

  if (params[0]=="ARROW.THICKNESS")          
    ARROW_THICKNESS = params[1].toInt();

  if (params[0]=="ATOMPOS") {
    if (params[1]=="relaxed")
      ATOMPOS = RELAXED;

    if (params[1]=="unrelaxed")
      ATOMPOS = UNRELAXED;

    if (params[1]=="composite")
      ATOMPOS = COMPOSITE;

    if (params[1]=="relaxed" or params[1]=="composite") {
      if (npar>2)
        ATOMDISP_SCALE = params[2].toInt();
      else
        ATOMDISP_SCALE = 1.0;
    }
  }

  if (params[0]=="CORREDGECOMPBCC") {
    CORR_EDGECOMP_BCC = (params[1]=="on");
  }
   
  if (params[0]=="CSYS") {
    for (i=0; i<3; i++) {
      for (j=0; j<3; j++) {
        idx = 3*i+j+1;
        CSYS[i][j] = params[idx].toDouble();
      }
    }
  }

  if (params[0]=="DISPCOMPONENT") {
    if (params[1]=="edge")  
      DISP_COMPONENT = EDGE;
    else if (params[1]=="screw")
      DISP_COMPONENT = SCREW;
    else
      DISP_COMPONENT = NONE;
  }

  if (params[0]=="DSCALEFACT") {
    if (params[1]=="auto")
      DSCALE_FACT = AUTO;
    else
      DSCALE_FACT = params[1].toDouble();
  }

  if (params[0]=="NEIGHRCUT") {
    NEIGHRCUT = params[1].toDouble();
  }

  if (params[0]=="PLANETRACES") {
    angle = params[1].toDouble(); 
    if (angle>0) {
      PTRACES = true; 
      PTRACES_ANGLE = angle;
    }
  }

  if (params[0]=="PRINTMAGDISP") {
    if (params[1]=="on")
      PRINT_MAG_DISP = true;

    else
      PRINT_MAG_DISP = false;
  }

  if (params[0]=="PROJVECTOR") {
    PROJ_VECTOR[0] = params[1].toDouble();
    PROJ_VECTOR[1] = params[2].toDouble();
    PROJ_VECTOR[2] = params[3].toDouble(); 
  }

  if (params[0]=="WINSIZE") {
    WINWIDTH = params[1].toInt();
    WINHEIGHT = params[2].toInt();
  }

  if (params[0]=="ZOOMFACT") {
    ZOOM_FACT = params[1].toDouble();
  }

  if (params[0]=="ZTOLERANCE") {
    ZTOLERANCE = params[1].toDouble();
  }
}


bool AppWin::loadFile( QString fn )
{
  QString fname;
  PltWin *initConf=NULL, *pw;
  long int blstart=0;
  int pos, NCurrentPlot, fmt, blnum=0, idx;
  bool ok;

  if (tabWidget==NULL) {
    tabWidget = new QTabWidget( this );
    tabWidget->setGeometry(0, 23, width(), height()-46);  
  }

  // what is the format of the input file
  fmt = FFormat(fn);

  while (fmt!=FORMAT_GB || (fmt==FORMAT_GB && blstart>=0)) {   
    NCurrentPlot = tabWidget->count();
    if (NCurrentPlot>=MAX_PLOTS) {
      msgError(err_TooManyPlots);
      return false;
    }

    plotWidget[NCurrentPlot] = new PltWin( tabWidget );
    pw = plotWidget[NCurrentPlot];

    if (plotWidget[NCurrentPlot]==NULL) {
      msgCritical(crit_OutOfMemory);
      exit;
    }

    switch(fmt) {
    case FORMAT_DD:  // ddplot internal format
      if (VERBOSE)
	printf("* the chosen file has a .dd format of ddplot\n");
      ok = plotWidget[NCurrentPlot]->LoadDD( fn );
      break;
      
    case FORMAT_BOP:  // format of BOP (see block.out)
      if (VERBOSE)
	printf("* the chosen file has a .bl format of PennBOP\n");
      ok = plotWidget[NCurrentPlot]->LoadBOP( fn );	   
      break;
      
    case FORMAT_PLT:  // plot format of BOP
      if (VERBOSE)
	printf("* the chosen file has a .plt format of PennBOP\n");
        ok = plotWidget[NCurrentPlot]->LoadPLT( fn );
      break;
      
    case FORMAT_GB:  // grain boundary data file (multiple blocks in one file)
      if (VERBOSE)
	printf("* the chosen file contains grain boundary data\n");
      blnum++;
      ok = plotWidget[NCurrentPlot]->LoadGB( fn, &blstart, initConf );
      if (blnum==1)  initConf = plotWidget[NCurrentPlot];
      break;

    case FORMAT_XYZ: // format of JMol
      if (VERBOSE)
	printf("* the chosen file has a .xyz format of JMol\n");
      ok = plotWidget[NCurrentPlot]->LoadXYZ( fn );
      break;
  
    case FORMAT_UNKNOWN:
      msgError(err_InputFmtUnknown);
    }

    // an error during reading the plot file has occurred
    if (!ok) {
      delete plotWidget[NCurrentPlot];
      return false;
    }

    // catch an error that may occur by constructing the neighbor list
    ok = plotWidget[NCurrentPlot]->PrepareBlock(false);
    if (!ok) {
      delete plotWidget[NCurrentPlot];
      return false;
    }
    plotWidget[NCurrentPlot]->SetGeometry();

    // allocate the space for data
    plotWidget[NCurrentPlot]->scaleArr.Allocate(plotWidget[NCurrentPlot]->NInit,10,3);  
    plotWidget[NCurrentPlot]->scaleArr.EnlargeStep(0,10,0);
    plotWidget[NCurrentPlot]->atomScreen.Allocate(plotWidget[NCurrentPlot]->NInit,2);
    
    // calculate the relative displacements between atoms
    switch(plotWidget[NCurrentPlot]->DispComponent) {
      case EDGE:
        plotWidget[NCurrentPlot]->calcEdgeRelDisp();
        break;

      case SCREW:
        plotWidget[NCurrentPlot]->calcScrewRelDisp();
    }

    // only the file name is printed
    fname = fn;
    pos = fname.indexOf('/');  
    if (pos>=0)
      fname.remove(0,pos+1);

    if (fmt==FORMAT_GB)
      fname.sprintf("%s (#%d)", fname.toAscii().data(), blnum);
    
    if (VERBOSE)
      printf("* inserting a widget with name: %s\n", fname.toAscii().data());

    tabWidget->addTab( plotWidget[NCurrentPlot], fname );
    tabWidget->setCurrentIndex( NCurrentPlot );
    tabWidget->show();

    if (fmt!=FORMAT_GB)  break;
  }

  repaintStatusBar();
  enableMenuItems(true);

  idx = tabWidget->currentIndex();
  actFirstPlot->setEnabled(idx > 0);
  actPrevPlot->setEnabled(idx > 0);
  actNextPlot->setEnabled(idx < tabWidget->count()-1);
  actLastPlot->setEnabled(idx < tabWidget->count()-1);

  return true;
}


void AppWin::loadSettings()
{
  QString tbuf;
  FILE *f;
  char *buf;
  bool eof;

  f = fopen( SETTINGS_FILE.toAscii().data(), "r+" );
  if (f!=NULL) {
    if (VERBOSE)
      printf("* reading settings from file %s\n",SETTINGS_FILE.toAscii().data());
  } else {
    if (VERBOSE)
      printf("* no configuration file ~/.ddplot found => default settings will be used\n");
    return;
  }

  buf = (char *) malloc(128);

  while (!feof(f)) {
    // read whole line
    eof = fgets(buf,128,f)==NULL;
    if (eof)
      break;

    tbuf = buf;
    interpretCommand(tbuf);
  }

  free(buf);
  fclose(f);
}


void AppWin::open()
{
  QString types( "usual plot files (*.plt *.bl *.dd *.xyz);;"
		 "ddplot's internal (*.dd);;"
		 "JMol's .xyz file (*.xyz);;"
		 "plot files (*.plt *.dat);;"
		 "BOP block files (*.bl *.block block.out block.interm st.interm.*);;"
		 "any file (*)");
  int n;
  bool ok;
  QString fname;

  QStringList files = QFileDialog::getOpenFileNames(
    this,
    "Select one or more files to open",
    "./",
    types);

  if (files.isEmpty()) return;

  QStringList list = files;
  QStringList::Iterator fn = list.begin();  
  QProgressDialog pdlg( "Loading plot files...", "&Abort", 0, files.count(),
			this );

  ok = true;
  n = 0;
  while( fn!=list.end() && ok ) {
    ok = loadFile(*fn);

    if (ok) {
      fn++;
      n++;
      pdlg.setValue(n);
      pdlg.repaint();
      
      if (pdlg.wasCanceled())
	return;
    }
  }
}


void AppWin::panRight()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(PAN_NORMAL,0);
  repaintStatusBar();
}


void AppWin::panLeft()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(-PAN_NORMAL,0);
  repaintStatusBar();
}


void AppWin::panUp()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(0,PAN_NORMAL);
  repaintStatusBar();
}


void AppWin::panDown()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(0,-PAN_NORMAL);
  repaintStatusBar();
}


void AppWin::panFastRight()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(PAN_FAST,0);
  repaintStatusBar();
}


void AppWin::panFastLeft()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(-PAN_FAST,0);
  repaintStatusBar();
}


void AppWin::panFastUp()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(0,PAN_FAST);
  repaintStatusBar();
}


void AppWin::panFastDown()
{
  plotWidget[ tabWidget->currentIndex() ]->Pan(0,-PAN_FAST);
  repaintStatusBar();
}


void AppWin::plotAtomLayers()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->plotType = PLOT_ATOM_LAYERS;
  pw->repaint();

  actDAtLayers->setChecked(true);
  actDAtTypes->setChecked(false);
}


void AppWin::plotAtomTypes()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  
  pw->plotType = PLOT_ATOM_TYPES;
  pw->repaint();

  actDAtLayers->setChecked(false);
  actDAtTypes->setChecked(true);
}


void AppWin::plotEdgeComponent()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->DispComponent = EDGE;
  pw->calcEdgeRelDisp();
  pw->repaint();
  repaintStatusBar();

  actNoComp->setChecked(false);
  actEdgeComp->setChecked(true);
  actScrewComp->setChecked(false);
}


void AppWin::plotNoComponent()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->DispComponent = NONE;
  pw->repaint();
  repaintStatusBar();

  actNoComp->setChecked(true);
  actEdgeComp->setChecked(false);
  actScrewComp->setChecked(false);
}

void AppWin::plotScrewComponent()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->DispComponent = SCREW;
  pw->calcScrewRelDisp();
  pw->repaint();
  repaintStatusBar();

  actNoComp->setChecked(false);
  actEdgeComp->setChecked(false);
  actScrewComp->setChecked(true);
}


void AppWin::plotUnrelaxed()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->AtomPos = UNRELAXED;
  pw->repaint();
  repaintStatusBar();

  actUnrelView->setChecked(true);
  actRelView->setChecked(false);
  actCompView->setChecked(false);
}


void AppWin::plotRelaxed()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->AtomPos = RELAXED;
  pw->repaint();
  repaintStatusBar();

  actUnrelView->setChecked(false);
  actRelView->setChecked(true);
  actCompView->setChecked(false);
}


void AppWin::plotComposite()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->AtomPos = COMPOSITE;
  pw->repaint();
  repaintStatusBar();

  actUnrelView->setChecked(false);
  actRelView->setChecked(false);
  actCompView->setChecked(true);
}


void AppWin::plotInertAtoms()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->InertAtoms = !pw->InertAtoms;
  pw->PrepareBlock(true);
  pw->SetGeometry();
  pw->repaint();

  actInertAtom->setChecked(pw->InertAtoms);
}


void AppWin::print()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  QPrintDialog printDialog(printer, this);
  if (printDialog.exec() == QDialog::Accepted)
    pw->printIt(printer);
}


void AppWin::printMagDisp()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->PrintMagDisp = abs(pw->PrintMagDisp-1); 
  pw->repaint();

  actMagDisp->setChecked(pw->PrintMagDisp);
}


void AppWin::printAtomNum()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  pw->AtomNumbers = abs(pw->AtomNumbers-1); 
  pw->repaint();

  actNumAtom->setChecked(pw->AtomNumbers);
}


void AppWin::projVector()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  DVector coord(3);
  int NPlots = tabWidget->count();
  int i,n;
  bool ok,okx,oky,okz;

  do {
    // dialog may not be WDestructiveClose, because it would delete all data inside exec()
    Ui::DProjVector ui;
    QDialog *dialog = new QDialog;
    ui.setupUi(dialog);

    ui.xcoord->insert( QString::number( pw->ProjVector(1) ));
    ui.xcoord->selectAll();
    ui.ycoord->insert( QString::number( pw->ProjVector(2) ));
    ui.zcoord->insert( QString::number( pw->ProjVector(3) ));

    dialog->exec();
    ok = dialog->result()==QDialog::Accepted;

    if (ok) {
      coord(1) = ui.xcoord->text().toDouble( &okx );
      coord(2) = ui.ycoord->text().toDouble( &oky );
      coord(3) = ui.zcoord->text().toDouble( &okz );

      if (okx && oky && okz) {
	for (n=0; n<NPlots; n++) {
	  pw = plotWidget[n];
	  if (!pw->isSelected) 
	    continue;

	  for (i=1; i<=3; i++)
	    pw->ProjVector(i) = coord(i);

	  pw->DispComponent = MIXED;
	  repaintStatusBar();
	  pw->repaint();
	}
      } else
        msgError(err_ProjVectIsString);
    }
  } while ( ok && (!okx || !oky || !okz) );
}


void AppWin::quit()
{
  //  if (msgWarning( warn_ReallyQuit ))
  exit(0);
}


// Reflect the block through the given plane
void AppWin::reflectBlock(QAction *action)
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  int maxis;
  int i, d, n;

  if (action==actXZReflect) {
    maxis = 2;
    if (VERBOSE)
      printf("* the plot has been reflected through the XZ plane\n");
  } else if (action==actYZReflect) {
    maxis = 1;
    if (VERBOSE)
      printf("* the plot has been reflected through the YZ plane\n");
  } else if (action==actXYReflect) {
    maxis = 3;
    for (i=1; i<=pw->NumZLayers; i++)
      pw->zCoordLayer(i) *= -1;
    if (VERBOSE)
      printf("* the plot has been reflected through the XY plane\n");
  }

  for (n=1; n<=pw->NInit; n++)
    pw->xyzInit(n,maxis) *= -1; 

  for (n=1; n<=pw->NInert; n++)
    pw->xyzInert(n,maxis) *= -1; 
    
  for (n=1; n<=pw->NRel; n++)
    pw->xyzRel(n,maxis) *= -1;
    
  for (n=1; n<=pw->NRel; n++) {
    for (d=1; d<=3; d++)
      pw->aDisp(n,d) = pw->xyzRel(n,d) - pw->xyzInit(n,d);  
  }

  for (i=1; i<=3; i++)
    pw->csys(maxis,i) *= -1;

  // must re-calculate the scaling of arrows
  if (pw->DispComponent==EDGE)
    pw->calcEdgeRelDisp();
  else if (pw->DispComponent==SCREW)
    pw->calcScrewRelDisp();

  pw->PrepareBlock(false);
  pw->SetGeometry();
  pw->repaint();
}


void AppWin::relaxInfo()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];

  Ui::DRelaxInfo ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  if (pw->tail!=NULL) {
    ui.textArea->append( pw->tail );
  }

  dialog->exec();
}


void AppWin::repaintStatusBar()
{
  if (tabWidget==NULL)
    return;

  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  char *dcomp;
  char msg[128];
  int NPlots = tabWidget->count();

  switch(pw->DispComponent) {
    case EDGE:
      dcomp = "EDGE";
      break;

    case SCREW:
      dcomp = "SCREW";
      break;

    case NONE:
      dcomp = "NONE";
      break;

    case DIFF_EDGE:
    case DIFF_SCREW:
      dcomp = "DIFF";
      break;

    default:
      dcomp = "MIXED";
  }

  sprintf(msg, "%d plot(s) | MAG=%0.2lf | DSCALE=%0.2lf | PANX=%0.2lf, PANY=%0.2lf" \
    " | PROJVECT=[%0.1lf,%0.1lf,%0.1lf] | DCOMP=%s | ATOMS=%s", 
    NPlots, 
    pw->ZFact,
    pw->DScaleFact,
    pw->xPan,
    pw->yPan,
    pw->ProjVector(1),
    pw->ProjVector(2),
    pw->ProjVector(3),
    dcomp,
    ATOMPOS_NAME[pw->AtomPos]);

  sbar->setText(msg);
}


// Rotate the block 180 deg about a specific axis
void AppWin::rotateBlock(QAction *action)
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  DMatrix mat(3,3), prod(3,3);
  DVector coor(3);
  int i, j, k, d, n;

  if (action==actXRot180) {
      mat(1,1) =  1;  mat(1,2) =  0;  mat(1,3) =  0;
      mat(2,1) =  0;  mat(2,2) = -1;  mat(2,3) =  0;
      mat(3,1) =  0;  mat(3,2) =  0;  mat(3,3) = -1;
      for (i=1; i<=pw->NumZLayers; i++)
	pw->zCoordLayer(i) *= -1;
      if (VERBOSE)
	printf("* the plot has been rotated 180 deg about the X axis\n");
  } else if (action==actYRot180) {
      mat(1,1) = -1;  mat(1,2) =  0;  mat(1,3) =  0;
      mat(2,1) =  0;  mat(2,2) =  1;  mat(2,3) =  0;
      mat(3,1) =  0;  mat(3,2) =  0;  mat(3,3) = -1;
      for (i=1; i<=pw->NumZLayers; i++)
	pw->zCoordLayer(i) *= -1;
      if (VERBOSE)
	printf("* the plot has been rotated 180 deg about the Y axis\n");
  } else if (action==actZRot180) {
      mat(1,1) = -1;  mat(1,2) =  0;  mat(1,3) =  0;
      mat(2,1) =  0;  mat(2,2) = -1;  mat(2,3) =  0;
      mat(3,1) =  0;  mat(3,2) =  0;  mat(3,3) =  1;
      if (VERBOSE)
	printf("* the plot has been rotated 180 deg about the Z axis\n");
  } else if (action==actXRot90) {
      mat(1,1) =  1;  mat(1,2) =  0;  mat(1,3) =  0;
      mat(2,1) =  0;  mat(2,2) =  0;  mat(2,3) = -1;
      mat(3,1) =  0;  mat(3,2) =  1;  mat(3,3) =  0;
      for (i=1; i<=pw->NumZLayers; i++)
	pw->zCoordLayer(i) *= -1;
      if (VERBOSE)
	printf("* the plot has been rotated 90 deg about the X axis\n");
  } else if (action==actYRot90) {
      mat(1,1) =  0;  mat(1,2) =  0;  mat(1,3) =  1;
      mat(2,1) =  0;  mat(2,2) =  1;  mat(2,3) =  0;
      mat(3,1) = -1;  mat(3,2) =  0;  mat(3,3) =  0;
      for (i=1; i<=pw->NumZLayers; i++)
	pw->zCoordLayer(i) *= -1;
      if (VERBOSE)
	printf("* the plot has been rotated 90 deg about the Y axis\n");
  } else if (action==actZRot90) {
      mat(1,1) =  0;  mat(1,2) = -1;  mat(1,3) =  0;
      mat(2,1) =  1;  mat(2,2) =  0;  mat(2,3) =  0;
      mat(3,1) =  0;  mat(3,2) =  0;  mat(3,3) =  1;
      if (VERBOSE)
	printf("* the plot has been rotated 90 deg about the Z axis\n");
  }

  // transformation of coordinates of atoms - unrelaxed
  for (n=1; n<=pw->NInit; n++) {
    for (i=1; i<=3; i++) {
      coor(i) = 0;
      for (j=1; j<=3; j++) {
	coor(i) += mat(i,j)*pw->xyzInit(n,j);
      }
    }
    
    for (i=1; i<=3; i++)
      pw->xyzInit(n,i) = coor(i);
  }

  // transformation of coordinates of atoms - inert
  for (n=1; n<=pw->NInert; n++) {
    for (i=1; i<=3; i++) {
      coor(i) = 0;
      for (j=1; j<=3; j++) {
	coor(i) += mat(i,j)*pw->xyzInert(n,j);
      }
    }
    
    for (i=1; i<=3; i++)
      pw->xyzInert(n,i) = coor(i);
  }
  
  // transformation of coordinates of atoms - relaxed
  for (n=1; n<=pw->NRel; n++) {
    for (i=1; i<=3; i++) {
      coor(i) = 0;
      for (j=1; j<=3; j++) {
	coor(i) += mat(i,j)*pw->xyzRel(n,j);
      }
    }
    
    for (i=1; i<=3; i++)
      pw->xyzRel(n,i) = coor(i);    
  }
  
  // recalculate the relative displacements
  for (n=1; n<=pw->NRel; n++) {
    for (d=1; d<=3; d++)
      pw->aDisp(n,d) = pw->xyzRel(n,d) - pw->xyzInit(n,d);  
  }

  // transformation of the coordinate system
  prod = 0;
  for (i=1; i<=3; i++)
    for (j=1; j<=3; j++)
      for (k=1; k<=3; k++)
	prod(i,j) += mat(i,k) * pw->csys(k,j);
  pw->csys = prod;

  // must re-calculate the scaling of arrows
  if (pw->DispComponent==EDGE)
    pw->calcEdgeRelDisp();
  else if (pw->DispComponent==SCREW)
    pw->calcScrewRelDisp();

  pw->PrepareBlock(false);
  pw->SetGeometry();
  pw->repaint();
}


// Saves the current plot in the .dd format of ddplot. If we are
// saving a file which has been read in the .dd format, the coordinate
// system is not requested because it was read from the .dd file. If
// the original file is .plt, the system asks for the coordinate
// system of the block.
void AppWin::saveDD()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString fname, msg;
  FILE *fdd;
  double sum;
  int i, j, pos;
  bool ok, needcsys;

  fname = CreateFName(pw->FName,".dd");
  QString types("ddplot's internal (*.dd)");
  fname = QFileDialog::getSaveFileName(
    this,
    "Save the plot as .dd file",
    fname,
    types);
  if (fname=="") return;

  fdd = fopen(fname.toAscii().data(),"w+");

  // header
  fprintf(fdd,"#\n");
  fprintf(fdd,"#  To view the atomic structure stored in this file, use ddplot\n");
  fprintf(fdd,"#  which is downloadable from http://groger.ipm.cz\n");
  fprintf(fdd,"#\n\n");

  // coordinate system of the block
  fprintf(fdd,"CSYS\n");
  for (i=1; i<=3; i++) {
    fprintf(fdd,"  %10.6lf  %10.6lf  %10.6lf\n",
	    pw->csys(i,1), pw->csys(i,2), pw->csys(i,3)); 
  }
  fprintf(fdd,"\n");

  // periodicity along x,y,z
  fprintf(fdd,"PERIOD\n");
  fprintf(fdd,"  %10.6lf  %10.6lf  %10.6lf\n\n",
	  pw->period(1), pw->period(2), pw->period(3));

  // dislocation center
  fprintf(fdd,"DISLO_CENTER\n");
  fprintf(fdd,"  %10.6lf  %10.6lf\n\n",
	  pw->xCore, pw->yCore);

  // unrelaxed coordinates
  fprintf(fdd,"NUM_UNREL\n  %d\n\n",pw->NInit);
  fprintf(fdd,"COOR_UNREL\n");
  for (i=1; i<=pw->NInit; i++) {
    fprintf(fdd,"  %10.6lf  %10.6lf  %10.6lf  %2d\n",
	    pw->xyzInit(i,1), pw->xyzInit(i,2), pw->xyzInit(i,3), pw->atomType(i));
  }
  fprintf(fdd,"\n");

  // relaxed coordinates
  fprintf(fdd,"NUM_REL\n  %d\n\n",pw->NRel);
  fprintf(fdd,"COOR_REL\n");
  for (i=1; i<=pw->NRel; i++) {
    fprintf(fdd,"  %10.6lf  %10.6lf  %10.6lf\n",
	    pw->xyzRel(i,1), pw->xyzRel(i,2), pw->xyzRel(i,3));
  }
  fprintf(fdd,"\n");

  // details of the relaxation
  if (pw->tail!=NULL) {
    fprintf(fdd,"RELAX_DETAILS\n");
    fwrite(pw->tail, 1, pw->TailSize-1, fdd);
  }
  
  fclose(fdd);

  // change the name of the widget
  pos = fname.indexOf('/');  
  if (pos>=0)
    fname.remove(0,pos+1);
  tabWidget->setTabText(tabWidget->currentIndex(), fname);
}


void AppWin::scaleArrow()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  double sfact;
  int NPlots = tabWidget->count();
  int i,istart,iend;
  bool ok,okk;

  Ui::DScaleArrow ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  ui.sfactor->setText( QString::number( pw->DScaleFact ));
  ui.sfactor->selectAll();
  do {
    dialog->exec();
    ok = dialog->result()==QDialog::Accepted;

    if (ok) {
      sfact = ui.sfactor->text().toDouble( &okk );
      if (okk) {
	// get the indexes for widgets to which this should be applied
	if (ui.applyHow->currentIndex()==0)
	  istart = iend = tabWidget->currentIndex();
	else {
	  istart = 0;
	  iend = tabWidget->count()-1;
	}
    
	// apply the new magnification ratio
	for (i=istart; i<=iend; i++) {
	  pw = plotWidget[i];
	  if (!pw->isSelected)
	    continue;

	  pw->DScaleFact = sfact; 
	  pw->repaint();
	}
      } else
        msgError(err_ScaleArrowIsString);   
    }
  } while (ok && !okk);

  repaintStatusBar();
}


void AppWin::scalePos()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  double fact;
  int sigAuto=100;
  int res, i, istart, iend;
  bool ok, okk;

  // create the dialog window
  ScalePos *dialog = new ScalePos(pw);
  dialog->magRatio->setText( QString::number( pw->factor ));
  dialog->magRatio->selectAll();

  dialog->exec();
  
  // which button has been clicked ?
  ok = dialog->result()==QDialog::Accepted;
  if (ok) {
    fact = dialog->magRatio->text().toDouble( &ok );
    if (!ok) {
      msgError(err_ScalePosIsString);   
      return;
    }

    // check if the magnification ratio should be applied to the current widget
    // or to all selected plots
    if (dialog->applyHow->currentIndex()==0)
      istart = iend = tabWidget->currentIndex();
    else {
      istart = 0;
      iend = tabWidget->count()-1;
    }
    
    // apply the new magnification ratio
    for (i=istart; i<=iend; i++) {
      pw = plotWidget[i];
      if (!pw->isSelected)
	continue;
      
      pw->factor = fact;
      pw->xOffset = (int) floor( (pw->width() - 2*pw->xBorder - pw->factor*pw->blSize(1))/2.0 );
      pw->yOffset = (int) floor( (pw->height() - 2*pw->yBorder - pw->factor*pw->blSize(2))/2.0 );
    
      pw->repaint();
    }
  }
}


void AppWin::selectFirstPlot()
{
  int idx;

  idx = tabWidget->currentIndex();

  if (idx > 0) {
    idx = 0;
    tabWidget->setCurrentIndex(idx);
    actFirstPlot->setEnabled(idx > 0);
    actPrevPlot->setEnabled(idx > 0);
    actNextPlot->setEnabled(idx < tabWidget->count()-1);
    actLastPlot->setEnabled(idx < tabWidget->count()-1);
  }
}


void AppWin::selectLastPlot()
{
  int idx;

  idx = tabWidget->currentIndex();

  if (idx < tabWidget->count()-1) {
    idx = tabWidget->count()-1;
    tabWidget->setCurrentIndex(idx);
    actFirstPlot->setEnabled(idx > 0);
    actPrevPlot->setEnabled(idx > 0);
    actNextPlot->setEnabled(idx < tabWidget->count()-1);
    actLastPlot->setEnabled(idx < tabWidget->count()-1);
  }
}


void AppWin::selectNextPlot()
{
  int idx;

  idx = tabWidget->currentIndex();

  if (idx < tabWidget->count()-1) {
    idx++;
    tabWidget->setCurrentIndex(idx);
    actFirstPlot->setEnabled(idx > 0);
    actPrevPlot->setEnabled(idx > 0);
    actNextPlot->setEnabled(idx < tabWidget->count()-1);
    actLastPlot->setEnabled(idx < tabWidget->count()-1);
  }
}


void AppWin::selectPrevPlot()
{
  int idx;

  idx = tabWidget->currentIndex();

  if (idx > 0) {
    idx--;
    tabWidget->setCurrentIndex(idx);
    actFirstPlot->setEnabled(idx > 0);
    actPrevPlot->setEnabled(idx > 0);
    actNextPlot->setEnabled(idx < tabWidget->count()-1);
    actLastPlot->setEnabled(idx < tabWidget->count()-1);
  }
}


void AppWin::setZTolerance()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  double zTol;
  int istart, iend, i;
  bool ok;
  
  Ui::DZTolerance ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  ui.zTolerance->setText( QString::number( pw->zTolerance ));

  dialog->exec();
  
  ok = dialog->result()==QDialog::Accepted;
  if (ok) {
    zTol = ui.zTolerance->text().toDouble( &ok );
    if (!ok) {
      msgError(err_ZToleranceIsString);   
      return;
    }

    if (ui.applyHow->currentIndex()==0)
      istart = iend = tabWidget->currentIndex();
    else {
      istart = 0;
      iend = tabWidget->count()-1;
    }
    
    // apply the new magnification ratio
    for (i=istart; i<=iend; i++) {
      pw = plotWidget[i];
      if (!pw->isSelected)
	continue;
      
      pw->zTolerance = zTol;
      pw->PrepareBlock(false);
      pw->repaint();
    }
  }
}


void AppWin::showGb()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  
  pw->showGB = !pw->showGB;
  actShowGb->setChecked(pw->showGB);
  pw->repaint();
}


void AppWin::showCells()
{
  int i;

  SHOW_NEIGHCELLS = !SHOW_NEIGHCELLS;
  actShowCells->setChecked(SHOW_NEIGHCELLS);

  // apply to all selected plots
  for (i=0; i<tabWidget->count(); i++) {
    if (plotWidget[i]->isSelected)
      plotWidget[i]->showNeighCells = !plotWidget[i]->showNeighCells;
  }
  
  plotWidget[ tabWidget->currentIndex() ]->repaint();
}


void AppWin::showCSys()
{
  int i;

  SHOW_CSYS = !SHOW_CSYS;
  actShowCSys->setChecked(SHOW_CSYS);

  // apply to all selected plots
  for (i=0; i<tabWidget->count(); i++) {
    if (plotWidget[i]->isSelected)
      plotWidget[i]->showCSys = !plotWidget[i]->showCSys;
  }
  
  plotWidget[ tabWidget->currentIndex() ]->repaint();
}


void AppWin::showPlanes()
{
  int i,astep;
  bool ok;

  Ui::DShowPlanes ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  ui.angle->setValue( PTRACES_ANGLE );
  ui.angle->selectAll();
  dialog->exec();
  ok = dialog->result()==QDialog::Accepted;

  if (!ok)
    return;

  astep = ui.angle->text().toDouble( &ok );
  PTRACES_ANGLE = astep;

  for (i=0; i<tabWidget->count(); i++) {
    if (!plotWidget[i]->isSelected) 
      continue;  
    
    if (astep>0) {
      plotWidget[i]->PlaneTraces = true;
      plotWidget[i]->PTAngle = astep;
    } else {
      plotWidget[i]->PlaneTraces = false;
    }
  }

  // only the current widget needs to be repainted
  plotWidget[ tabWidget->currentIndex() ]->repaint();
}


void AppWin::showZLayers()
{
  int i;

  SHOW_ZLAYERS = !SHOW_ZLAYERS;
  actShowZLay->setChecked(SHOW_ZLAYERS);

  // apply to all selected plots
  for (i=0; i<tabWidget->count(); i++) {
    if (plotWidget[i]->isSelected)
      plotWidget[i]->showZLayers = !plotWidget[i]->showZLayers;
  }
  
  plotWidget[ tabWidget->currentIndex() ]->repaint();
}


void AppWin::selectPlots()
{
  QString item;
  int res, pos;
  unsigned int i;
  bool selFlag;

  SelectPlots *dialog = new SelectPlots();

  for (i=0; i<tabWidget->count(); i++) {
    item = plotWidget[i]->FName;

    // only the file name is printed
    pos = item.indexOf('/');  
    if (pos>=0)
      item.remove(0,pos+1);
    
    selFlag = plotWidget[i]->isSelected;

    dialog->plotFiles->addItem(item);
    dialog->plotFiles->setItemSelected(dialog->plotFiles->item(i), selFlag);
  }

  // get the user's response
  dialog->exec();
  res = dialog->result();
  
  // clicked OK
  if (res==QDialog::Accepted) {
    for (i=0; i<dialog->plotFiles->count(); i++) {
      selFlag = dialog->plotFiles->isItemSelected(dialog->plotFiles->item(i));
      plotWidget[i]->isSelected = selFlag;
      tabWidget->setTabEnabled(i, selFlag);
    }
	   
    tabWidget->repaint();
    repaintStatusBar();
  }
}


void AppWin::selectZLayers()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  QString item, stat;
  int sigSelAll=100, sigUnselAll=101;
  int pos, res, istart, iend, iplt;
  unsigned int i;
  bool selFlag;

  SelectZLayers *dialog = new SelectZLayers;

  // set up the data
  for (i=0; i<pw->NumZLayers; i++) {
    item = QString::number( pw->zCoordLayer(i+1), 'f', 4 );
    selFlag = pw->zLayerSel(i+1);

    dialog->zLayers->addItem(item);
    dialog->zLayers->setItemSelected(dialog->zLayers->item(i), selFlag);
  }

  // get the user's response
  dialog->exec();
  res = dialog->result();

  // clicked OK
  if (res==QDialog::Accepted) {
    if (dialog->applyHow->currentIndex()==0)
      istart = iend = tabWidget->currentIndex();
    else {
      istart = 0;
      iend = tabWidget->count()-1;
    }
    
    for (iplt=istart; iplt<=iend; iplt++) {
      PltWin *pw = plotWidget[iplt];
      
      for (i=0; i<pw->NumZLayers; i++) {
	selFlag = dialog->zLayers->isItemSelected(dialog->zLayers->item(i));
	pw->zLayerSel(i+1) = selFlag;
      }
      
      if (VERBOSE) {
	if (iend>istart)
	  printf("* working on plot #%d:  %s ...\n",iplt+1,pw->FName.toAscii().data());
	printf("* list of z-layers, their positions along the z-axis and visibility:\n");
	printf("  (differential displacements will be calculated only between atoms in visible layers)\n");
	
	for (i=1; i<=pw->NumZLayers; i++) {
	  if (pw->zLayerSel(i))
	    stat = "visible";
	  else
	    stat = "hidden";
	  
	  printf("    %3d  %8.4lf  %s\n", 
		 i, pw->zCoordLayer(i), stat.toAscii().data());
	}
      }
      
      // Construct the neighbor list in the unrelaxed configuration
      InitNeighborList( pw, pw->NeighListInit );
      
      // must re-calculate the relative displacements - arrows plotted between different neighbors
      if (pw->DispComponent==EDGE)
	pw->calcEdgeRelDisp();
      else if (pw->DispComponent==SCREW) {
	pw->calcScrewRelDisp();
      }
      
      pw->repaint();
    }
  }
}


void AppWin::transform()
{
  PltWin *pw = plotWidget[ tabWidget->currentIndex() ];
  DMatrix newCSys(3,3), mat(3,3), prod(3,3);
  DVector coor(3);
  double sum;
  int res, i, j, k, n, istart, iend, iplt;
  bool needcsys;


  // Is the coordinate system of the block given ?
  needcsys = true;
  sum = 0;
  for (i=1; i<=3; i++) {
    for (j=1; j<=3; j++) {
      sum += fabs(pw->csys(i,j));
    }
  }
  needcsys = (sum==0);

  if (needcsys) {
    msgInfo(info_GiveCoordBlock);
    return;
  }

  Ui::DTransform ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  ui.x1->setText( QString::number( pw->csys(1,1), 'g', 4 ));
  ui.x2->setText( QString::number( pw->csys(1,2), 'g', 4 ));
  ui.x3->setText( QString::number( pw->csys(1,3), 'g', 4 ));
  ui.y1->setText( QString::number( pw->csys(2,1), 'g', 4 ));
  ui.y2->setText( QString::number( pw->csys(2,2), 'g', 4 ));
  ui.y3->setText( QString::number( pw->csys(2,3), 'g', 4 ));
  ui.z1->setText( QString::number( pw->csys(3,1), 'g', 4 ));
  ui.z2->setText( QString::number( pw->csys(3,2), 'g', 4 ));
  ui.z3->setText( QString::number( pw->csys(3,3), 'g', 4 ));
  
  dialog->exec();
  res = dialog->result();
  if (res!=QDialog::Accepted)
    return;

  // target coordinate system
  newCSys(1,1) = ui.X1->text().toDouble();
  newCSys(1,2) = ui.X2->text().toDouble();
  newCSys(1,3) = ui.X3->text().toDouble();
  newCSys(2,1) = ui.Y1->text().toDouble();
  newCSys(2,2) = ui.Y2->text().toDouble();
  newCSys(2,3) = ui.Y3->text().toDouble();
  newCSys(3,1) = ui.Z1->text().toDouble();
  newCSys(3,2) = ui.Z2->text().toDouble();
  newCSys(3,3) = ui.Z3->text().toDouble();
  
  if (VERBOSE) {
    printf("* new orientation of the block will be:\n");
    for (i=1; i<=3; i++)
      printf("    [  %8.4f  %8.4f  %8.4f  ]\n", newCSys(i,1), newCSys(i,2), newCSys(i,3));
  }

  // how to apply this settings?
  if (ui.applyHow->currentIndex()==0)
    istart = iend = tabWidget->currentIndex();
  else {
    istart = 0;
    iend = tabWidget->count()-1;
  }

  // transform the coordinates...
  for (iplt=istart; iplt<=iend; iplt++) {
    pw = plotWidget[iplt];
    if (!pw->isSelected)
      continue;

    // transformation matrix
    mtransf( pw->csys, newCSys, mat );

    // save the new coordinate system
    pw->csys = newCSys;

    // transformation of coordinates of atoms - unrelaxed
    for (n=1; n<=pw->NInit; n++) {
      for (i=1; i<=3; i++) {
	coor(i) = 0;
	for (j=1; j<=3; j++) {
	  coor(i) += mat(i,j)*pw->xyzInit(n,j);
	}
      }
      
      for (i=1; i<=3; i++)
	pw->xyzInit(n,i) = coor(i);
    }
    
    // transformation of coordinates of atoms - relaxed
    for (n=1; n<=pw->NRel; n++) {
      for (i=1; i<=3; i++) {
	coor(i) = 0;
	for (j=1; j<=3; j++) {
	  coor(i) += mat(i,j)*pw->xyzRel(n,j);
	}
      }
      
      for (i=1; i<=3; i++)
	pw->xyzRel(n,i) = coor(i);    
    }

    pw->PrepareBlock(false);
    pw->SetGeometry();
    pw->repaint();
  }
}


void AppWin::zoomIn()
{
  plotWidget[ tabWidget->currentIndex() ]->Zoom(2.0); 
  repaintStatusBar();
}


void AppWin::zoomOut()
{
  plotWidget[ tabWidget->currentIndex() ]->Zoom(0.5); 
  repaintStatusBar();
}
