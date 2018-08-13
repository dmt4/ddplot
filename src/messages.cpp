#include <qmessagebox.h>
#include <qstring.h>


void msgCritical( char *msg )
{
  QMessageBox::critical( 0, "Critical Error", msg );
}


void msgError( char *msg )
{
  QMessageBox::critical( 0, "Error", msg );
}


int msgQuestion( char *title, char *msg )
{
  return QMessageBox::warning( 0, title, msg, "&Yes", "&No", "&Cancel", 0, 1 );
}


int msgWarning( char *msg, char *button1, char *button2, char *button3, int par1, int par2 )
{
  return QMessageBox::warning( 0, "Warning", msg, button1, button2, button3, par1, par2 );
}


void msgInfo( QString msg )
{
  QMessageBox::information( 0, "Information", msg.toLatin1().data() );
}
