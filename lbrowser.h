//  headerdatei lbrowser.h

#ifndef LBROWSER_H
#define LBROWSER_H

#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>
#include <q3textbrowser.h>
#include <q3mainwindow.h>

class cLittleBrowser;

class cLittleBrowser : public Q3MainWindow
{
   Q_OBJECT
public:
    cLittleBrowser(const QString& home, const QString& path,QWidget* parent,const char* name);
public slots:
    virtual void show();
private:
    QString m_sFileName;
    Q3TextBrowser* m_pbrowser;
};

#endif
