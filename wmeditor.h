// editor headerdatei 

#ifndef WMEDITOR_H
#define WMEDITOR_H

#include <qtextedit.h>
#include <qobject.h>
#include <qwidget.h>
#include <qevent.h>

class wmEditor: public QTextEdit {
    Q_OBJECT
public:
    wmEditor(QWidget* parent = 0, const char *name = 0);
    ~wmEditor(){};
    
signals:
    void textMessage (QString);
    
protected:
    virtual void closeEvent (QCloseEvent*); // falls text geändert wurde
};

#endif

    
    
