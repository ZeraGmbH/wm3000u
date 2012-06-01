// implementierung des wmEditor

#include <qwidget.h>
#include <qevent.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QCloseEvent>
#include "wmeditor.h"

wmEditor::wmEditor (QWidget* parent, const char *name)
    : Q3TextEdit (parent,name)	{};
	
void wmEditor::closeEvent (QCloseEvent *e) {
    emit textMessage ( text() );
    e->accept();
}
