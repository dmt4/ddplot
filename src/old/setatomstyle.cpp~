#include <qcolordialog.h>
#include <qcombobox.h>
#include <qlayout.h>

#include "appwin.h"
#include "pltwin.h"
#include "setatomstyle.h"


Ui_SetAtomStyle::Ui_SetAtomStyle(PltWin *pwid)
{
  pw = pwid;
  zColor.Allocate(pw->NumZLayers,2);
}


void Ui_SetAtomStyle::setData(void)
{
  int i, nrows;

  // enough rows ?
  nrows = atomTable->rowCount();
  if (pw->NumZLayers>nrows)
    atomTable->setRowCount(pw->NumZLayers-nrows);

  // settings
  for ( i=1; i<=pw->NumZLayers; i++ ) {
    QTableWidgetItem *witem1 = new QTableWidgetItem(QString("%1").arg( pw->zCoordLayer(i), 0, 'f', 5 ));
    atomTable->setItem(i-1, 0, witem1);      
    QTableWidgetItem *witem2 = new QTableWidgetItem(QString("%1").arg( pw->zDiamLayer(i) ));
    atomTable->setItem(i-1, 1, witem2);
    QTableWidgetItem *witem3 = new QTableWidgetItem(QString("%1").arg( pw->zLineThickLayer(i) ));
    atomTable->setItem(i-1, 2, witem3);

    QTableWidgetItem *wfgcol = new QTableWidgetItem("");
    wfgcol->setBackground(QBrush(pw->zColorLayer(i,1)));
    atomTable->setItem(i-1, 3, wfgcol);    

    QTableWidgetItem *wbgcol = new QTableWidgetItem("");
    wbgcol->setBackground(QBrush(pw->zColorLayer(i,2)));
    atomTable->setItem(i-1, 4, wbgcol);
  }
}


bool Ui_SetAtomStyle::verify()
{
  int diam,thick,i; 
  bool ok,ok_diam,ok_thick;

  ok = true;

  for (i=0; i<atomTable->rowCount(); i++) {
    diam = atomTable->item(i,1)->text().toInt( &ok_diam );
    thick = atomTable->item(i,2)->text().toInt( &ok_thick );

    ok = ok || (ok_diam && ok_thick);
    if (!ok)
      return false;
  }

  return true;
}


//------------------------------------------------------------------------------
// SLOTS
//------------------------------------------------------------------------------

void Ui_SetAtomStyle::setColor( int row, int col )
{
  if (col==3 || col==4) {
    QColor color = QColorDialog::getColor(zColor(row+1,col-2), atomTable);

    if (color.isValid()) { 
      QTableWidgetItem *wcol = new QTableWidgetItem();
      wcol->setBackground(color);
      atomTable->setItem(row, col, wcol); 
      zColor(row+1,col-2) = color;
    }
  }
}
