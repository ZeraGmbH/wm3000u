// implementierung des wmEditor

#include <qwidget.h>
#include <qevent.h>
#include <qpopupmenu.h>
#include "wmeditor.h"

wmEditor::wmEditor (QWidget* parent, const char *name)
    : QTextEdit (parent,name)	{};
	
void wmEditor::closeEvent (QCloseEvent *e) {
    emit textMessage ( text() );
    e->accept();
}
