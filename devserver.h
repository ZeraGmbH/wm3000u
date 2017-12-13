// headerdatei devserver.h 
// deklaration einer device server klasse

#ifndef DEVSERVER_H
#define DEVSERVER_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3serversocket.h>
#include <q3socket.h>
#include <q3socketdevice.h> 
#include <qsocketnotifier.h>
#include <q3ptrlist.h>

#include "wmglobal.h"
#include "cbiface.h"
#include "confdata.h"
#include "cmdinterpret.h"
#include "scpiface.h"
#include "en61850.h"


class cClientIODevice: public QObject // dies haben alle client io devices gemein, egal ob tcp/ip socket oder serial 
{
    Q_OBJECT    
    
public:    
    cClientIODevice(){};
    
signals:
    void SendCommand( QString&); // der client hat ein kommando und sendet dieses
    void connectionClosed(cClientIODevice*); // der client meldet sich ab
    
public slots:
    virtual void ReceiveAnswer( QString&) = 0; // pure virtual
};
	

class cClientSocketDevice : public cClientIODevice
{
    Q_OBJECT
    
public:
    cClientSocketDevice(int sock, QObject *parent = 0, const char*  name = 0);
    ~cClientSocketDevice();
    
public slots:
    virtual void ReceiveAnswer( QString&); 
        
private slots:
    void ReadCommand(); // wenn daten da sind
    void SendAnswer( int); // von notifier aktiviert mit int fd als parameter
    void CloseConnection();
    
private:
    Q3Socket* m_pSock;
    QSocketNotifier* m_pZDSWriteNotifyer;
    QString m_sInput;
    QString m_sOutput;
};
    

// class cSCPIFace;

class cDeviceServer:  public Q3ServerSocket // asynchroner device server pure virtual
{
    Q_OBJECT
    
public:
    cDeviceServer( Q_UINT16 port, int backlog = 1, QObject * parent = 0, const char * name = 0 );
    virtual void newConnection(int) = 0; // instanziiert ein client socket device und trägt es in liste ein 
    
signals:
    void RemoteCtrlInfo(bool); // wir informieren ob remote control oder nicht
 
private slots:
    void CloseConnection(cClientIODevice*); // clients melden sich hierüber ab
   
protected:    
    Q3PtrList<cSCPIFace> m_SCPIFacePtrList; // die liste aller angelegten interfaces
    void setPriority(); // alle clients werden informiert
};


class cwm3000DeviceServer:  public cDeviceServer  // der wm3000 device server
{
    Q_OBJECT
    
public:
    cwm3000DeviceServer( Q_UINT16 port, int backlog = 1, QObject * parent = 0, const char * name = 0 );
    virtual void newConnection(int); // instanziiert ein client socket device und ein wm3000 scpi interfaceund trägt das interface in die liste ein 
    virtual void newConnection(cClientIODevice*); // nimmt das bereits instanziierte xxx interface, generiert noch ein wm3000 scpi interface und trägt das interface in die liste ein
    
public slots:
    void ReceiveDeviceConfiguration(cConfData*); // empfangen der conf. vom device
    void ReceiveIFaceConfiguration(cConfData*); // empfangen der conf. vom interface
    void ReceiveETHStatus( cEN61850Info*); // empfangen der en61850 status infos
    void ReceiveENInfoReguest(); // wenn eins der interfaces en61850 info angefragt hat
    void ReceiveSelftestResult(int);
    void ReceiveSelftestRequest(); // wenn eins der interfaces *tst ausführen soll
    void ReceiveOffsetResult(double offs);
    void ReceiveChannelNOffsetMeasureRequest();
    void ReceiveChannelXOffsetMeasureRequest();
    void ReceiveResetETHStatusCommand();
    void ReceiveIFaceRangeCommand(cConfData*);
    void SetRangeListSlot( cWMRangeList&,  cWMRangeList&);
    void ReceiveIFaceDefaultConfig(); // das device soll default konfig. setzen
    void ReceiveRangeAutomaticDone(); // rückmeldung wenn bereich automatik fertig ist
    void ReceiveActualValues(cwmActValues*); // empfängt messwerte vom device
    void ReceiveLPValue(cwmActValues*); // empfängt lastpunkt  vom device
    void ReceiveVersionInfo(tVersSerial*); // empfängt info von device
    void ReceiveAffectStatus(uchar act, ushort stat);
    
signals:
    void SendDeviceConfiguration(cConfData*); // senden der conf.  an das device
    void SendIFaceConfiguration(cConfData*); // senden der conf.  an das interface
    void SendIFaceETHStatus(cEN61850Info*); // wir senden den eth status an das interface
    void SendSelftestResult(int); // wir senden das ergebnis des selbsttest an das interface
    void SendOffsetResult(double); // dito
    void RequestEN61850Info(); //  der server fordert die info an und verteilt sie
    void RequestSelftest(); // der server lässt den selbsttest durchführen
    void RequestChannelNOffsetMeasure(); // der server lässt die n kanal offsetmessung durchführen
    void RequestChannelXOffsetMeasure(); // der server lässt die x kanal offsetmessung durchführen
    void ResetETHStatus(); // der server leitet das reset kommando weiter
    void SendRangeCommand(cConfData*); // ebenso den bereich setzen befehl
    void SendDefaultConfigCommand(); // der server leitet kommando zum setzen default konfig. weiter
    void ConfirmRangeAutoRdy();
    void SendActualValues(cwmActValues*); // sendet messwerte an die clients
    void SendLPValue(cwmActValues*); // sendet lastpunkt an die clients
    void SendVersionInfo(tVersSerial*); // sendet info an die clients
    void AffectStatus(uchar, ushort);
    
private:
    cConfData m_ConfData; // wir speichern die konfiguration zwischen
    tVersSerial* m_pV;
    cWMRangeList m_sNXRangeList;
    cWMRangeList m_sEVTRangeList;
};

#endif
