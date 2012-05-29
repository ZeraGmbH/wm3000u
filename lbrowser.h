//  headerdatei lbrowser.h

#ifndef LBROWSER_H
#define LBROWSER_H

#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>
#include <qtextbrowser.h>
#include <qmainwindow.h>

class cLittleBrowser;

class cLittleBrowser : public QMainWindow
{
   Q_OBJECT
public:
    cLittleBrowser(const QString& home, const QString& path,QWidget* parent,const char* name);
public slots:
    virtual void show();
private:
    QString m_sFileName;
    QTextBrowser* m_pbrowser;
};

#endif
