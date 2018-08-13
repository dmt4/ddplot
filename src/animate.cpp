#include <qdialog.h>

#include "animate.h"


Animate::Animate(QDialog *parent) : QDialog(parent)
{
  setupUi(this);
  connect(btSelAll, SIGNAL(released()), this, SLOT(selectAll()));
  connect(btUnselAll, SIGNAL(released()), this, SLOT(unselectAll()));
}


void Animate::selectAll()
{
  unsigned int i;
  
  for (i=0; i<plotFiles->count(); i++)
    plotFiles->setItemSelected(plotFiles->item(i), true);
}


void Animate::unselectAll()
{
  unsigned int i;
  
  for (i=0; i<plotFiles->count(); i++)
    plotFiles->setItemSelected(plotFiles->item(i), false);
}
