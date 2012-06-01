// definition wmsocket

#include <qapplication.h>
#include <qeventloop.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3socket.h>
#include <q3textstream.h>
#include <qmessagebox.h>
#include <qtimer.h>

#include "zhclientsocket.h"


cZHClientSocket::cZHClientSocket(int t, QObject *parent, const char *name)
    :Q3Socket(parent,name)
{
    m_nTime = t; 
    m_nError = 0;
    m_bHostFound = false;
    
    QObject::connect(this,SIGNAL(error(int)),this,SLOT(TCPErrorHandling(int)));
    QObject::connect(this,SIGNAL(hostFound()),this,SLOT(HostFound()));
    QObject::connect(this,SIGNAL(readyRead()),this,SLOT(ReceiveInput()));
    QObject::connect(this,SIGNAL(connected()),this,SLOT(ConnectionDone()));
    QObject::connect(&ToDataTimer,SIGNAL(timeout()),this,SLOT(DataTimerExpired()));
    QObject::connect(&ToConnTimer,SIGNAL(timeout()),this,SLOT(ConnectTimerExpired()));
    QObject::connect(&DecoupleTimer,SIGNAL(timeout()),this,SLOT(SendError()));
}


QString cZHClientSocket::readLine()
{
    QString s = Q3Socket::readLine();
    QString l = QString ("Inp[%1:%2] : %3").arg(peerName())
		                                 .arg(peerPort())
				     .arg(s);
    emit SendLogData(l);
    return s;
}


int cZHClientSocket::writeLine(QString &s)
{
    QString l = QString ("Out[%1:%2] : %3") .arg(peerName()) .arg(peerPort()) .arg(s);
    emit SendLogData(l); // fürs logfile
    int ret;
    if ( (ret = this->writeBlock(s.latin1(),s.length())) == -1) {
	l = "Error while writing TCP-Block";
	emit SendLogData (l);
	m_nError |= myErrSocketWrite;
	DecoupleTimer.start(0,true); // der fehler wird später gesendet ....
    }
    else
    {
	flush(); // raus damit
    }
    return ret;
}


void cZHClientSocket::SendCommand(QString& cmds) { // gibt ein kommando an einem socket aus 
    QStringList sl;
    sl.append("ACK"); // nur ack !!!!!
    SendCommand(cmds, sl);
}


void cZHClientSocket::SendCommand(QString& cmds,QStringList& sl) { // kommando an socket mit liste der möglichen antwort en
    m_sCommand = cmds;
    m_sAnswerList = sl;
        if ( writeLine(cmds) == -1) // wenn bei write ein fehler auftrat -> den gibts später als signal
	return;
    ToDataTimer.start(m_nTime,true); // wir starten den timeout timer
}


void cZHClientSocket::SendQuery(QString& cmds) { // gibt ein kommando an einem socket aus 
    QStringList sl; // leere liste....akzeptiert jede antwort
    SendCommand(cmds, sl);
}


void cZHClientSocket::connectToHost (const QString& host,Q_UINT16 port)
{
    m_nError = 0; // wir setzen neu auf
    m_bHostFound = false; 
    
    m_sHost = host;
    m_nPort = port;
 
    QString l = QString ("Connect to %1:%2") .arg(host) .arg(port);
    emit SendLogData(l); // fürs logfile
    Q3Socket::connectToHost(host,port);  
    ToConnTimer.start(m_nTime,true);
}


QString& cZHClientSocket::GetAnswer()
{
    return m_sAnswer;
}


int cZHClientSocket::GetError()
{
    return m_nError;
}


void cZHClientSocket::SetError(int e)
{
    m_nError = e;
}

void cZHClientSocket::ResetError()
{
    m_nError = 0;
}

bool cZHClientSocket::hostfound()
{
    return m_bHostFound;
}


// hier sind alle slots

void cZHClientSocket::TCPErrorHandling(int e)
{
    QString l;
    switch (e)
    {
	case Q3Socket::ErrConnectionRefused:
	m_nError |= myErrConnectionRefused;
	l = "Connection refused !";
	emit SendLogData (l);   
	ToConnTimer.stop(); // fehler schon diagn.
	break;
    case Q3Socket::ErrHostNotFound:
	m_nError |= myErrHostNotFound;
	l = "Host not found !";
	emit SendLogData (l);   
	ToConnTimer.stop(); // fehler schon diagn.
	break;
    case Q3Socket::ErrSocketRead :
	m_nError |= myErrSocketRead;
	l = "Socket Data read error !";
	emit SendLogData (l);   
	ToConnTimer.stop(); // fehler schon diagn.
	break;
    }
    qDebug("TCP-Error %d\n",e);
    emit Error(); // fehler signal senden
}


void cZHClientSocket::HostFound()
{
    m_bHostFound = true;
}
 

void cZHClientSocket::ReceiveInput() 
{
    QString s;
    while (canReadLine()) {
	s = readLine().upper().stripWhiteSpace();
	if ( s.contains("DSPINT") ) { // es ist eine asynchrone meldung .... das ändern wir noch
	    emit AsyncDataReceived(s); // später "Interrupt DSPINT:1"
	}
	else
	{
	    ToDataTimer.stop(); // wir haben input -> timer anhalten
	    m_sAnswer = s; // antwort speichern
	    if ( !m_sAnswerList.empty() && (m_sAnswerList.findIndex(m_sAnswer) == -1) ) { //antwort  falsch	
		m_nError |= myErrSocketUnexpectedAnswer;
		QString l = "Unexpected Answer !";
		emit SendLogData (l);
		emit Error();
	    }
	    else
	    {
		emit DataReceived();
	    }
	}
	// flush();
    }
}


void cZHClientSocket::DataTimerExpired()
{
    m_nError |= myErrSocketReadTimeOut;
    QString l = "Timeout !";
    emit SendLogData (l);
    emit Error(); // fehler signal senden
}


void cZHClientSocket::ConnectTimerExpired()
{
    m_nError |= myErrSocketConnectionTimeOut;
    QString l = "Timeout !";
    emit SendLogData (l);   
    emit Error(); // fehler signal senden    
}
 

void cZHClientSocket::SendError()
{
    emit Error(); // fehler signal senden       
}


void cZHClientSocket::ConnectionDone()
{
    ToConnTimer.stop(); // wenn wir verbunden sind -> timer stop
    QString l = "Connected !";
    emit SendLogData (l);   
}
