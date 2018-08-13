#include "selectzlayers.h"


SelectZLayers::SelectZLayers(QDialog *parent) : QDialog(parent)
{
  setupUi(this);
  connect(btSelAll, SIGNAL(released()), this, SLOT(selectAll()));
  connect(btUnselAll, SIGNAL(released()), this, SLOT(unselectAll()));
}


void SelectZLayers::selectAll()
{
  unsigned int i;  
  for (i=0; i<zLayers->count(); i++)
    zLayers->setItemSelected(zLayers->item(i), true);
}


void SelectZLayers::unselectAll()
{
  unsigned int i;
  for (i=0; i<zLayers->count(); i++)
    zLayers->setItemSelected(zLayers->item(i), false);
}
