#include <math.h>

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qprogressdialog.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qspinbox.h>
#include <qtextedit.h>

#include "fileopen.xpm"
#include "filesave.xpm"
#include "fileprint.xpm"

#include "appwin.h"


AppWin::AppWin() : QMainWindow()
{
  QPixmap openIcon, printIcon, saveIcon;
  int pos, i;

  loadSettings(SETTINGS_FILE);

  tabWidget = NULL;

  openIcon = QPixmap(fileopen);
  printIcon = QPixmap(fileprint);
  saveIcon = QPixmap(filesave);

  resize(WINWIDTH, WINHEIGHT);

  createMenus();
  enableMenuItems(false);

  printer = new QPrinter( QPrinter::PrinterResolution );
  printer->setColorMode(QPrinter::Color);
  printer->setPageSize(QPrinter::Letter);
}


AppWin::~AppWin()
{
  delete(printer);
}


void AppWin::closeEvent( QCloseEvent * )
{
  quit();
}


void AppWin::resizeEvent( QResizeEvent * )
{
  if (tabWidget==NULL)
    return;

  tabWidget->setGeometry(0, 23, width(), height()-46); 
}
