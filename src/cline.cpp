#include <qstring.h>
#include <string.h>
#include <stdio.h>

#include "cline.h"
#include "ddplot.h"


void CLineHelp()
{
  printf("ddplot [options] [plot file]\n\n");
  printf("Options:\n");
  printf("  -f file    reads the custom settings from [file]. By default, the settings\n");
  printf("             is read from file .ddplot located in your HOME directory. When using\n");
  printf("             more different settings, use -f to specify which settings should \n");
  printf("             be adopted for the current session.\n"); 
  printf("  -h         this help\n");
  printf("  -v         verbose mode - print detailed information about what is currently\n");
  printf("             going on into the terminal window\n");
}


bool InterpretArgs( int argc, char **argv ) 
{
  QString arg, arg2;
  int i, idx;
  bool ok=true,found;

  for (i=1; i<argc; i++) {
    arg = argv[i];

    // this is needed for Mac OS X when running ddplot from the Finder - it gives as argv[1]
    // the process serial number
    arg2 = arg;  arg2.remove(5,arg2.length()-5);
    if (arg2=="-psn_") continue;

    found = false;

    // load settings file instead of .ddplot
    if (arg=="-f" && i+1<argc) {
      SETTINGS_FILE = argv[++i];
      printf("User settings read from %s\n",SETTINGS_FILE.toAscii().data());
      continue;
    }

    // command-line help
    if (arg=="-h") {
      CLineHelp();
      return false;
    }

    // verbose mode
    if (arg=="-v") {
      VERBOSE = true;
      continue;
    }

    // last argument is the name of the plot file
    PLOT_FILE = arg;
  }

  return ok;
}
