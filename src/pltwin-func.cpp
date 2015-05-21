#include <math.h>
#include <qlineedit.h>
#include <qpainter.h>
#include <stdlib.h>

#include "ui_dnewcutoff.h"

#include "calcs.h"
#include "messages.h"
#include "misc.h"
#include "neighlist.h"
#include "pltwin.h"


// Finds the atom in the topmost layer that is nearest to the position [x,y] on the screen
int PltWin::FindNearestAtom( int x, int y )
{
  double dist, mindist;
  int n, atom;

  atom = -1;
  for (n=1; n<=NInit; n++) {
    dist = sqrt( pow(atomScreen(n,1)-x,2) + pow(atomScreen(n,2)-y,2) );

    if (dist<mindist || atom<0) {
      mindist = dist;
      atom = n;
    }
  }

  return atom;
}


//-----------------------------------------------------------------------------
//
//   Reading the BOP block file (.bl, .block)
//
//-----------------------------------------------------------------------------

bool PltWin::LoadBOP( QString fname )
{
  FILE *f;
  QString rec, elem;
  char *buf;
  double latpar;
  int n, idx, d;
  bool eof, ok=true;

  FName = fname;
  f = fopen(fname.toAscii(),"r");
  if (f==NULL)
    return false;

  buf = (char *) malloc(128);

  // reading the plot file
  while (!feof(f)) {
    eof = fgets(buf,128,f)==NULL;
    if (eof)
      break;

    // throw out comments
    rec = buf;
    idx = rec.indexOf('#');   if (idx>=0) rec.truncate(idx);
    idx = rec.indexOf('\n');  if (idx>=0) rec.truncate(idx);
    idx = rec.indexOf('\r');  if (idx>=0) rec.truncate(idx);  // Linux terminates with \r\n

    // length of the simulation cell (in units of the lattice parameter)
    if (rec=="LEN") {
      fscanf(f, "%lf %lf %lf\n", &period(1), &period(2), &period(3));
      continue;
    }

    // lattice parameter
    if (rec=="LATPAR") {
      fscanf(f, "%lf\n", &latpar);
      continue;
    }

    // relaxed coordinates of atoms
    if (rec=="ND") { 
      for (d=1; d<=3; d++)
	period(d) /= latpar;    // the correct periodicity along the three directions
      Bvect = 0.0;
      Bvect(3) = period(3);

      fscanf(f, "%d\n", &NRel);  
      fgets(buf, 128, f);       // skip the "D" token

      xyzRel.Allocate(NRel,3);
      atomType.Allocate(NRel);
      atomType = 0;

      for (n=1; n<=NRel; n++) {
	fscanf(f, "%lf %lf %lf %d", &xyzRel(n,1), &xyzRel(n,2), &xyzRel(n,3), &atomType(n));
	atomType(n)++;          // must be from 1
	fgets(buf, 128, f);     // skip the rest, if there is some junk

	for (d=1; d<=3; d++)
	  xyzRel(n,d) *= period(d);  // correct positions of relaxed atoms
      }
      continue;
    }   

    // inert atoms near the surface of the simulated block
    if (rec=="NINERT") {
      fscanf(f, "%d\n", &NInert);
      fgets(buf, 128, f);       // skip DINERT token
      xyzInert.Allocate(NInert,3);

      for (n=1; n<=NInert; n++) {
	fscanf(f, "%lf %lf %lf", &xyzInert(n,1), &xyzInert(n,2), &xyzInert(n,3));
	fgets(buf, 128, f);     // skip the rest, if there is some junk

	for (d=1; d<=3; d++)
	  xyzInert(n,d) *= period(d);  // correct positions of inert atoms
      }

      continue;
    }

     // unrelaxed coordinates of atoms
    if (rec=="UNRLD") {
      NInit = NRel;  // NRel comes from block under D
      xyzInit.Allocate(NInit,3);

      for (n=1; n<=NInit; n++) {
	fscanf(f, "%lf %lf %lf", &xyzInit(n,1), &xyzInit(n,2), &xyzInit(n,3));
	fgets(buf, 128, f);  // skip the rest, if there is some junk
      }
      continue;
     }
  }

  free(buf);
  fclose(f);

  if (xyzInit.data==NULL) {
    msgError(err_NoUnrelPos); 
    return false;
  }

  return ok;
}


//-----------------------------------------------------------------------------
//
//   Reading the block in the DDPlot internal format (.dd)
//
//-----------------------------------------------------------------------------

bool PltWin::LoadDD( QString fname )
{
  FILE *f;
  QString cmd;
  char *buf;
  long int pos;
  int i,idx;
  bool eof, ok=true;

  FName = fname;
  f = fopen(fname.toAscii(),"r");
  if (f==NULL)
    return false;

  buf = (char *) malloc(128);

  // reading the plot file
  while (!feof(f)) {
    eof = fgets(buf,128,f)==NULL;
    if (eof)
      break;

    // throw out comments
    cmd = buf;

    idx = cmd.indexOf('#');   if (idx>=0) cmd.truncate(idx);
    idx = cmd.indexOf('\n');  if (idx>=0) cmd.truncate(idx);
    idx = cmd.indexOf('\r');  if (idx>=0) cmd.truncate(idx);  // Linux terminates with \r\n
    if (cmd.isEmpty())
      continue;

    cmd = cmd.trimmed();
    cmd = cmd.toUpper();

    // interpretation of commands    
    if (cmd=="CSYS") {
      for (i=1; i<=3; i++)
	fscanf(f, "%lf %lf %lf\n", &csys(i,1), &csys(i,2), &csys(i,3));
      continue;
    }

    if (cmd=="PERIOD") {
      fscanf(f, "%lf %lf %lf\n", &period(1), &period(2), &period(3));
      Bvect = 0.0;
      Bvect(3) = period(3);
      continue;
    }

    if (cmd=="DISLO_CENTER") {
      fscanf(f, "%lf %lf\n", &xCore, &yCore);
      continue;
    }

    if (cmd=="NUM_UNREL") {
      fscanf(f, "%d\n", &NInit);
      continue;
    }

    if (cmd=="COOR_UNREL") {
      xyzInit.Allocate(NInit,3);
      atomType.Allocate(NInit);

      for (i=1; i<=NInit; i++) {
	fscanf(f, "%lf %lf %lf %d\n", 
	       &xyzInit(i,1), &xyzInit(i,2), &xyzInit(i,3), &atomType(i));
        atomType(i)++;   // must be from 1...
      }
      continue;
    }

    if (cmd=="NUM_REL") {
      fscanf(f, "%d\n", &NRel);
      continue;
    }

    if (cmd=="COOR_REL") {
      xyzRel.Allocate(NRel,3);

      for (i=1; i<=NRel; i++)
	fscanf(f, "%lf %lf %lf\n", 
	       &xyzRel(i,1), &xyzRel(i,2), &xyzRel(i,3));
      continue;
    }

    if (cmd=="NUM_INERT") {
      fscanf(f, "%d\n", &NInert);
      continue;
    }

    if (cmd=="COOR_INERT") {
      xyzInert.Allocate(NInert,3);

      for (i=1; i<=NInert; i++)
	fscanf(f, "%lf %lf %lf\n", 
	       &xyzInert(i,1), &xyzInert(i,2), &xyzInert(i,3));
      continue;
    }

    // read the rest of the file - information about the relaxation
    if (cmd=="RELAX_DETAILS") {
      pos = ftell(f);
      fseek(f, 0, SEEK_END);
      TailSize = ftell(f) - pos + 1;
      fseek(f, pos, SEEK_SET);
      
      tail = (char*) malloc(TailSize);
      if (tail==NULL) {
      	msgCritical(crit_OutOfMemory);
      	exit(2);
      }
      
      fread(tail, 1, TailSize, f);
    }
  }

  free(buf);
  fclose(f);

  return ok;
}


/*--------------------------------------------------------------------

  Reading the block containing the grain boundary data file (.PLT
  format).  The first configuration in the block is regarded as the
  unrelaxed (starting) configuration. The arrows then mean the
  relative displacements between an actual atomic positions and the
  atomic positions in this unrelaxed configuration.

--------------------------------------------------------------------*/

bool PltWin::LoadGB( QString fname, long int *blstart, PltWin *initConf )
{
  FILE *f;
  char *buf, c;
  char txt[80];
  int dummy, endflag;
  int n, d;


  // the file read is the grain boundary data file
  isGBfile = true;
  FName = fname;

  f = fopen(fname.toAscii(), "r");
  if (f==NULL)
    return false;

  // set the position to the start of a given block
  fseek(f, *blstart, SEEK_SET);

  // read one block
  fscanf(f, "%d\n", &NRel);

  // allocate the space for the data
  xyzRel.Allocate(NRel,3);
  atomType.Allocate(NRel);
  gbLayer.Allocate(10);
  gbLayer.EnlargeStep(10);

  // relaxed block - reading coordinates
  for (n=1; n<=NRel; n++)
    fscanf(f, "%lf\n", &xyzRel(n,3));
  
  for (n=1; n<=NRel; n++) {
    fscanf(f, "%lf %lf %d %d %d\n", 
	   &xyzRel(n,1), &xyzRel(n,2), &atomType(n), &gbLayer(n), &dummy);
    if (gbLayer(n)==0)  gbYcoor = xyzRel(n,2);
  }


  // Initial configuration is identical to the relaxed configuration only
  // for the first data set in the file. For all other sets, the unrelaxed
  // configuration is taken from the first block in the file.

  if (*blstart==0) {  
    NInit = NRel;
    xyzInit.Allocate(NInit,3);
    xyzInit = xyzRel;
  } else {
    NInit = initConf->NInit;
    xyzInit.Allocate(NInit,3);
    xyzInit = initConf->xyzInit;  
  }
  
  // skip the trailing zero - what if it is nonzero ?
  fscanf(f, "%d\n", &endflag);  

  // save the position of the start of the subsequent block; if this is the last one,
  // return -1
  if (feof(f))
    *blstart = -1;
  else
    *blstart = ftell(f);

  fclose(f);
  return true;
}


//-----------------------------------------------------------------------------
//
//   Reading the block in the plot format of BOP (.plt)
//
//-----------------------------------------------------------------------------

bool PltWin::LoadPLT( QString fname )
{
  FILE *f;
  QString qtxt;
  char *buf, c;
  char txt[80];
  double unused;
  long int pos;
  int n, d;

  FName = fname;
  f = fopen(fname.toAscii(), "r");
  if (f==NULL)
    return false;

  //
  // relaxed block - reading coordinates
  //

  fscanf(f, "%d\n", &NRel);

  xyzRel.Allocate(NRel,3);
  atomType.Allocate(NRel);

  for (n=1; n<=NRel; n++)
    fscanf(f, "%lf\n", &xyzRel(n,3));

  for (n=1; n<=NRel; n++)
    fscanf(f, "%lf %lf %d\n", &xyzRel(n,1), &xyzRel(n,2), &atomType(n));


  //
  // initial block - reading coordinates
  //

  fscanf(f, "%d\n", &NInit);

  xyzInit.Allocate(NInit,3);

  for (n=1; n<=NInit; n++)
    fscanf(f, "%lf\n", &xyzInit(n,3));

  for (n=1; n<=NInit; n++)
    fscanf(f, "%lf %lf\n", &xyzInit(n,1), &xyzInit(n,2));

  // if there is any dislocation info, read it
  fscanf(f, "%lf \n %lf \n %lf \n %lf \n", &unused, &period(1), &period(2), &period(3));
  fscanf(f, "CORE { %lf;%lf }\n", &xCore, &yCore);

  // the Burgers vector is assumed to be parallel to the z-direction
  Bvect = 0.0;
  Bvect(3) = period(3);

  // read the rest of the file - information about the relaxation
  pos = ftell(f);
  qtxt = "";

  // is there any reasonable info (not just spaces) ?
  while (!feof(f)) {
    n = fscanf(f,"%s\n",&txt);
    if (n<0)
      break;

    qtxt = QString(txt).remove(' ');
    if (qtxt!="")
      break;
  }

  if (qtxt!="") {
    fseek(f, 0, SEEK_END);
    TailSize = ftell(f) - pos + 1;
    fseek(f, pos, SEEK_SET);
    
    tail = (char*) malloc(TailSize);
    if (tail==NULL) {
      msgCritical(crit_OutOfMemory);
      exit(2);
    }
  
    fread(tail, 1, TailSize, f);
  }

  fclose(f);

  return true;
}


//-----------------------------------------------------------------------------
//
//   Reading the block in the plot format of JMol (.xyz)
//
//-----------------------------------------------------------------------------

bool PltWin::LoadXYZ( QString fname )
{
  FILE *f;
  QString title, tag, elems[MAX_ELEMENTS];
  char *buf;
  char elem[3];
  double unused;
  int n, d, i, nelem;

  FName = fname;
  f = fopen(fname.toAscii(), "r");
  if (f==NULL)
    return false;

  buf = (char *) malloc(128);

  //
  // initial block - reading coordinates
  //

  fscanf(f, "%d\n", &NInit);
  fgets(buf, 128, f);
  title = QString(buf).trimmed();
  xyzInit.Allocate(NInit,3);
  atomType.Allocate(NInit);

  nelem = 0;

  for (n=1; n<=NInit; n++) {
    fscanf(f, "%s %lf %lf %lf\n", &elem, &xyzInit(n,1), &xyzInit(n,2), &xyzInit(n,3));

    atomType(n) = 0;
    for (i=1; i<=nelem; i++)
      if (elems[i]==elem) {
	atomType(n) = i;
	break;
      }

    if (atomType(n)==0) {
      nelem++;
      if (nelem>MAX_ELEMENTS) {
	printf("Maximum number of elements reached. Increase MAX_ELEMENTS.\n");
	exit(0);
      }
      atomType(n) = nelem;
      elems[nelem] = elem;
    }
  }

  if (feof(f)) {
    free(buf);
    return false;
  }


  //
  // relaxed block - reading coordinates
  //

  fscanf(f, "%d\n", &NRel);
  fgets(buf, 128, f);
  title = QString(buf).trimmed();
  xyzRel.Allocate(NRel,3);

  for (n=1; n<=NRel; n++)
    fscanf(f, "%s %lf %lf %lf\n", &elem, &xyzRel(n,1), &xyzRel(n,2), &xyzRel(n,3));

  if (!feof(f))
    fscanf(f, "%s %lf\n", buf, &Bvect(3));

  tag = QString(buf).trimmed();
  if (tag!="BVECT") {
    printf("The Burgers vector is not given at the end of the file.\n");
    printf("The screw components will not be plotted !\n");
    printf("Add line e.g.: BVECT 0.866\n");
  }

  free(buf);
  fclose(f);

  return true;
}


// Calculation of the atomic displacements
bool PltWin::PrepareBlock(bool toggle_inert)
{ 
  QString stat;
  DVector zcoord;
  IVector isorted;
  double zlast, dmin, dmax, disp, rtry, rsave;
  int i, m, n, d, layer, fgidx, bgidx, thidx, diaidx;
  bool ok;


  // calculation of the boundaries of the block of atoms

  for (d=1; d<=3; d++) {
    xyzMin(d) = xyzInit(1,d);
    xyzMax(d) = xyzMin(d);
  }

  // atom #1 is considered above
  for (n=2; n<=NInit; n++) {
    for (d=1; d<=3; d++) {
      if (xyzInit(n,d) < xyzMin(d))  xyzMin(d) = xyzInit(n,d);
      if (xyzInit(n,d) > xyzMax(d))  xyzMax(d) = xyzInit(n,d);
    }
  }

  if (InertAtoms) {
    for (n=1; n<=NInert; n++) {
      for (d=1; d<=3; d++) {
	if (xyzInert(n,d) < xyzMin(d))  xyzMin(d) = xyzInert(n,d);
	if (xyzInert(n,d) > xyzMax(d))  xyzMax(d) = xyzInert(n,d);
      }
    }
  }

  for (d=1; d<=3; d++) {
    blSize(d) = xyzMax(d) - xyzMin(d);
    xyzCen(d) = (int) trunc((xyzMin(d)+xyzMax(d))/2.0);
  }

  if (VERBOSE) {
    printf("* properties of the loaded block:\n");
    printf("    - number of atoms in the perfect lattice = %d\n", NInit);
    printf("    - number of atoms in the relaxed configuration = %d\n", NRel);
    printf("    - number of inert atoms = %d\n", NInert);
    printf("    - position of the dislocation core = [ %0.4lf, %0.4lf ]\n", xCore, yCore);

    printf("* geometry of the block of atoms (unrelaxed coordinates):\n");
    printf("    xmin = %8.4lf,  xmax = %8.4lf,  xsize = %8.4lf\n", 
	   xyzMin(1), xyzMax(1), blSize(1));
    printf("    ymin = %8.4lf,  ymax = %8.4lf,  ysize = %8.4lf\n", 
	   xyzMin(2), xyzMax(2), blSize(2));
    printf("    zmin = %8.4lf,  zmax = %8.4lf,  zsize = %8.4lf\n", 
	   xyzMin(3), xyzMax(3), blSize(3));    
  }


  // only inert atoms were accounted for or removed; no need to do the stuff below
  if (toggle_inert)
    return true;


  // find z-layers by sorting the z-coordinates in ascending order

  isorted.Allocate(NInit);
  sort(xyzInit, 1, NInit, 3, isorted);  // isorted are indexes to a sorted xyzInit list

  zcoord.Allocate(NInit);
  for (n=1; n<=NInit; n++)
    zcoord(n) = xyzInit(isorted(n),3);


  // layers and their appearance

  layer = 0;
  zLayer.Allocate(NInit);

  for (n=1; n<=NInit; n++) {
    if (n==1 || zcoord(n)>zlast+zTolerance) {
      layer++;     
      zlast = zcoord(n);

      zCoordLayer(layer) = zlast;

      // calculation of the color, thickness and diameter
      fgidx = CalcIndex(layer-1,NUM_FGCOLORS);
      bgidx = CalcIndex(layer-1,NUM_BGCOLORS);
      thidx = CalcIndex(layer-1,NUM_THICKNESS);
      diaidx = CalcIndex(layer-1,NUM_DIAM);

      zColorLayer(layer,1) = ATOM_FGCOLOR[fgidx];
      zColorLayer(layer,2) = ATOM_BGCOLOR[bgidx];
      zLineThickLayer(layer) = ATOM_THICKNESS[thidx];
      zDiamLayer(layer) = ATOM_DIAM[diaidx];
    }
    zLayer(isorted(n)) = layer;
  } 

  NumZLayers = layer;

  zcoord.Free();
  isorted.Free();


  // all layers are selected by default -> i.e. atoms in all layers are considered
  // for plotting arrows

  zLayerSel.Allocate(NumZLayers);
  zLayerSel = 1;  // all layers are visible by default

  if (VERBOSE) {
    printf("* list of layers, their positions along z, visibility, line thickness, diameter and colors:\n");
    printf("  (differential displacements will be calculated only between atoms in visible layers)\n");

    for (i=1; i<=NumZLayers; i++) {
      if (zLayerSel(i))
	stat = "visible";
      else
	stat = "hidden";

      printf("    %3d  %8.4lf  %s  %d  %d  %s  %s\n", i, zCoordLayer(i), stat.toAscii().data(), 
	     zLineThickLayer(i), zDiamLayer(i), 
	     zColorLayer(i,1).name().toAscii().data(), zColorLayer(i,2).name().toAscii().data());
    }
  }


  // calculation of the displacement vectors

  dmin = INF;
  dmax = -INF;

  aDisp.Allocate(NRel,3);

  for (n=1; n<=NRel; n++) {
    for (d=1; d<=3; d++)
      aDisp(n,d) = xyzRel(n,d) - xyzInit(n,d);    

    if (VERBOSE) {
      disp = sqrt( pow(aDisp(n,1),2) + pow(aDisp(n,2),2) + pow(aDisp(n,3),2) );
      if (disp<dmin)  dmin = disp;
      if (disp>dmax)  dmax = disp;
    }
  }

  if (VERBOSE) {
    printf("* calculation of displacements induced by the applied stress:\n"); 
    printf("    dmin = %8.4lf,  dmax = %8.4lf\n", dmin, dmax);
  }


  // Construct the neighbor list in the unrelaxed configuration.
  // If an error occurs here, it means that an atom has too many neighbors that they cannot
  // be stored in the allocated array -> invoke the dialog for the user to input a smaller
  // cut-off

  ok = InitNeighborList( this, NeighListInit );
  if (ok)
    return true;
  
  // if the error occurred, allow the user to input NeighRCut manually for this plot

  if (!isGBfile) 
    rtry = vlength(period);
  else
    rtry = blSize(1)/10.0;  // this is only a guess - can be changed as we like

  Ui::DNewCutOff ui;
  QDialog *dialog = new QDialog;
  ui.setupUi(dialog);

  do {
    ui.rCurrent->setText( QString::number( rcut, 'g', 4 ) );
    ui.rTry->setText( QString::number( rtry, 'g', 4 ) );
    dialog->exec();
    
    if (dialog->result()==QDialog::Accepted) {
      rcut = ui.rNew->text().toDouble();
      rsave = NEIGHRCUT;
      NEIGHRCUT = -1;  // use the given rcut

      ok = InitNeighborList( this, NeighListInit );

      NEIGHRCUT = rsave;
    }
  } while (dialog->result()==QDialog::Accepted && !ok);

  return ok;
}
