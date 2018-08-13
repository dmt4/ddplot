#include "selectplots.h"


SelectPlots::SelectPlots(QDialog *parent) : QDialog(parent)
{
  setupUi(this);
  connect(btSelAll, SIGNAL(released()), this, SLOT(selectAll()));
  connect(btUnselAll, SIGNAL(released()), this, SLOT(unselectAll()));
}


void SelectPlots::selectAll()
{
  unsigned int i;
  
  for (i=0; i<plotFiles->count(); i++)
    plotFiles->setItemSelected(plotFiles->item(i), true);
}


void SelectPlots::unselectAll()
{
  unsigned int i;
  
  for (i=0; i<plotFiles->count(); i++)
    plotFiles->setItemSelected(plotFiles->item(i), false);
}
