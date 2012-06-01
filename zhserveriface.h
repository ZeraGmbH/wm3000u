// headerdatei zhserveriface.h 
// deklaration einer virt. basisklasse für zera hardware server interfaces

#ifndef ZHSERVERIFACE_H
#define ZHSERVERIFACE_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3ptrlist.h>

#include "zhclientsocket.h"


class cZHServerIFace: public QObject
{
    Q_OBJECT
    
public:
    cZHServerIFace(QString, int, int);
    ~cZHServerIFace();
    cZHClientSocket *iFaceSock;
    
    virtual void connectYourself() = 0; // verbinde dich mit deinem server
    
signals:
    void iFaceDone();
    void iFaceError();
    void iFaceAsync(const QString&);
        
protected slots:
    virtual void SocketDoneSlot() = 0;    
    virtual void SocketErrorSlot() = 0;
    
private:
    QString m_sHost; // host name
    int m_nPort; // host port
    bool m_biFaceError;
};
    
#endif
