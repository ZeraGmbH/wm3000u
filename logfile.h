// headerdatei logfile.h

#ifndef LOGFILE_H
#define LOGFILE_H

#include <qobject.h>
#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3textedit.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qevent.h>


class CLogFile:public QObject
{
    Q_OBJECT
public:
    CLogFile(const QString,const long=8192);
    ~CLogFile();
    
public slots:
    void SetFileSizeSlot(const long); // falls logfile umkonfiguriert wird
    void AddLogTextSlot(const QString&); // wenn neue daten einlaufen
    void SendLogSlot(); // aufforderung alle daten zu senden 
    
signals:
    void SendLogDataSignal(const QString&); 
    
private:
    QStringList m_sLogLinesList;
    QString m_sFileName;
    long m_nMaxFileLen,m_nActFileLen; // max. , akt. logfilegrösse
};

#endif
