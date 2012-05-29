// headerdatei logfileview.h

#ifndef LOGFILEVIEW_H
#define LOGFILEVIEW_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtimer.h>
#include <qtextedit.h>
#include <qdialog.h>
#include <qevent.h>
#include "widgeom.h"

class CLogFileView:public QDialog
{
    Q_OBJECT
    
public:
    CLogFileView(const QString,const long,QWidget * parent = 0, const char * wname = 0);
    virtual ~CLogFileView();
    QTextEdit *m_pText;
    
public slots:    
    void SaveSession(QString);
    bool LoadSession(QString);
    void ShowHideLogFileSlot(bool); // sichtbar oder nicht
    void AddLogTextSlot(const QString&); // wenn neue daten einlaufen
    void show(); // mittels timer aktiviert
    
protected:
    virtual void closeEvent ( QCloseEvent * );
    virtual void resizeEvent ( QResizeEvent *);
    
signals:
    void isVisibleSignal(bool);    
    
private:
    cWidgetGeometry m_widGeometry;
    QTimer showT;
    QStringList m_loglist;
    
private slots:
    void showList();
};

#endif
