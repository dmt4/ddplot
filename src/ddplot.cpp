/*--------------------------------------------------------------------------------

                                  d d p l o t

  Program for visualization of relative displacements between atoms in
  crystalline lattices using the dislocation displacement plots. When started,
  the program reads the configuration file .ddplot in (1) the given directory if
  the program is called as "ddplot -f <given directory>/.ddplot", (2) the
  current directory, (1) the home directory $HOME

  Copyright (C)2002-2009 Roman Groger
  under the GNU General Public License

--------------------------------------------------------------------------------*/

#include <pwd.h>
#include <qapplication.h>
#include <qcolor.h>

#include "appwin.h"
#include "cline.h"


char *ATOMPOS_NAME[3] = {"UNRELAXED", "RELAXED", "COMPOSITE"};
int ATOM_DIAM[10] = {12, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int ATOM_THICKNESS[10] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
QColor ATOM_FGCOLOR[10];   // defined below
QColor ATOM_BGCOLOR[10];   // defined below
QString ATOM_NAME[MAX_LAYERS];
int ARR_NEIGHNUM = 1;
int ARR_NEIGHBORS[MAX_NEIGHBORS];
double CSYS[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int DISP_COMPONENT = SCREW;
double DSCALE_FACT = 1;
int NUM_FGCOLORS = 1;
int NUM_BGCOLORS = 8;
int NUM_DIAM = 1;
int NUM_THICKNESS = 1;
double PROJ_VECTOR[3] = {1,1,1};
int ARROW_SHORTEST = 0;
int ARROW_THICKNESS = 1;
int PLOT_TYPE = PLOT_ATOM_LAYERS;
int ATOMPOS = UNRELAXED;
double ATOMDISP_SCALE = 1.0;
int WINWIDTH=900, WINHEIGHT=700;
bool PRINT_MAG_DISP = false;
bool PRINT_NUM_ATOM = false;
double ZOOM_FACT = 1.0;
bool ATOM_NUMBERS = false;
bool SHOW_DCENTER = false;
bool PTRACES = false;
int PTRACES_ANGLE = 0.0;
bool SHOWGB = true;
bool SHOW_ZLAYERS = false;
bool ALL_ZLAYERS = true;
bool VERBOSE = false;
double NEIGHRCUT = -1;  // if no rcut has been defined by user
bool SHOW_CSYS = false;
bool SHOW_NEIGHCELLS = false;
bool CORR_EDGECOMP_BCC = false;
double ZTOLERANCE = 0.0;

QString SETTINGS_FILE;
QString PLOT_FILE = "";

#ifdef DEBUG
  FILE *fdbg;
#endif


/*
  The main function of the ddplot code.
*/

int main( int argc, char ** argv ) 
{
  char *home_dir;
  char *fgcolors[1] = {"black"};
  char *bgcolors[8] = {"white", "yellow", "green", "cyan", "red", "blue", "gray", "black"};
  int i;

  // debugging mode ?
#ifdef DEBUG
  printf("Running ddplot in the DEBUG mode...\n");
  printf("The output will be saved in file ddplot.dbg\n");

  fdbg = fopen("ddplot.dbg","w+");
#endif

  // getting the home directory
  home_dir = getenv("HOME");
  SETTINGS_FILE = home_dir + QString("/.ddplot");

  // interpretation of the command-line arguments
  if (argc>1) {
    if (!InterpretArgs(argc,argv))
      return 0;
  }

  // default settings
  for (i=0; i<NUM_FGCOLORS; i++)
    ATOM_FGCOLOR[i] = QColor(fgcolors[i]);

  for (i=0; i<NUM_BGCOLORS; i++)
    ATOM_BGCOLOR[i] = QColor(bgcolors[i]);

  for (i=0; i<MAX_LAYERS; i++)
    ATOM_NAME[i] = "X";

  ARR_NEIGHBORS[0] = 1;

  QApplication App(argc, argv);
  AppWin *MainWin = new AppWin();
  MainWin->setWindowTitle( QString(PLATFORM) + QString("ddplot ") + QString(VERSION) );
  MainWin->show();

  return App.exec();

#ifdef DEBUG
  fclose(fdbg);
#endif
}
