#ifndef SETATOMSTYLE_H
#define SETATOMSTYLE_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include "ui_datomstyle.h"

#include "ddplot.h"
#include "pltwin.h"
#include "darray.h"

QT_BEGIN_NAMESPACE

class Ui_SetAtomStyle : public Ui_DAtomStyle
{
Q_OBJECT
public:
  DArray<QColor,2> zColor;

  SetAtomStyle( PltWin *pwid );
  void setData(void);
  bool verify();

public:
  PltWin *pw;

public slots:
  void setColor(int row, int col); 
};

namespace Ui {
    class DSetAtomStyle: public Ui_SetAtomStyle {};
} // namespace Ui

QT_END_NAMESPACE

#endif
