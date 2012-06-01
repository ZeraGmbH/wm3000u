// definition cZHClientSocket  (ZeraHardwareClientSocket)
// abgeleitet von qsocket , erweitert so dass signale für logfiles ausgegeben werden
// und signale wenn daten empfangen, asynchron oder synchron sowie fehlermeldungen

#ifndef ZHCLIENTSOCKET_H
#define ZHCLIENTSOCKET_H

#include <q3socket.h>
#include <qstringlist.h>
#include <qtimer.h>


enum AddError { myErrConnectionRefused = 1, myErrHostNotFound = 2, myErrSocketRead = 4 , myErrSocketWrite = 4, myErrSocketUnexpectedAnswer = 8 , myErrSocketReadTimeOut = 16, myErrSocketConnectionTimeOut = 32, myErrDeviceBusy = 64};

class cZHClientSocket: public Q3Socket
{
    Q_OBJECT
    
public:
    cZHClientSocket(int, QObject *parent = 0, const char *name = 0);
    void SendCommand(QString&); // kommando an socket wenn nur ack als antwort akzeptiert wird
    void SendCommand(QString&,QStringList&); // kommando an socket mit list als mögl. antworten
    void SendQuery(QString&); // query socket intern antwortliste leer -> rückmeldung ist rückgabewert
    virtual void connectToHost (const QString&,Q_UINT16);
    QString& GetAnswer();
    int GetError();
    void SetError(int);
    void ResetError();
    bool hostfound();
    
signals:    
    void SendLogData(const QString&); // für das logfile 
    void AsyncDataReceived(const QString&); // wenn asynchrone meldungen einlaufen 
    void DataReceived(); // wir senden nur, dass wir was empfangen haben, der empfänger entscheidet ob er es lesen möchte
    void Error(); // das gleiche gilt für den fehlerfall 
    
private:
    QTimer ToDataTimer, ToConnTimer, DecoupleTimer; // timeout data timer, to connection timer, 
    QStringList m_sAnswerList;
    QString m_sCommand;
    QString m_sAnswer;
   
    virtual QString readLine(); // liesst eine zeile vom server und schreibt logfile
    int writeLine(QString&); // sendet eine zeile an den server und schreibt logfile
    QString m_sHost;
    int m_nPort;
    
    bool m_bHostFound;
    int m_nError; // alle möglichn fehlerzustände
    int m_nTime; // in ms für timeout
    
 private slots:
    void TCPErrorHandling(int);
    void HostFound();
    void ReceiveInput();
    void DataTimerExpired();
    void ConnectTimerExpired();
    void SendError();
    void ConnectionDone();
};

#endif
