// implementierung cZeraDeviceServer und zugehoerigen client

#include "scpiface.h"
#include "wm3000scpiface.h"
#include "devserver.h"


cClientSocketDevice::cClientSocketDevice(int sock, QObject *parent, const char*  name)
{
    m_pSock = new Q3Socket(parent, name);
    m_pSock->setSocket(sock);
    m_pZDSWriteNotifyer = new QSocketNotifier ( sock, QSocketNotifier::Write);
    m_pZDSWriteNotifyer->setEnabled (false);
    connect(m_pZDSWriteNotifyer,SIGNAL(activated(int)),this,SLOT(SendAnswer(int)));
    connect( m_pSock, SIGNAL(connectionClosed()), SLOT(CloseConnection()) );
    connect( m_pSock, SIGNAL(readyRead()), SLOT(ReadCommand()) );
}


cClientSocketDevice::~cClientSocketDevice()
{
    delete m_pSock;
    delete m_pZDSWriteNotifyer;
}


void cClientSocketDevice::CloseConnection()
{
    emit connectionClosed(this);
}


void cClientSocketDevice::ReadCommand()
{
    m_sInput = "";
    while ( m_pSock->canReadLine() )
    {
        m_sInput = m_pSock->readLine();
        m_sInput.remove('\r');
        m_sInput.remove('\n');
        emit SendCommand( m_sInput);
    }
}


void cClientSocketDevice::ReceiveAnswer( QString& s)
{
    m_sOutput = s + "\n"; // setzen wir den 
    m_pZDSWriteNotifyer->setEnabled (true);
}     


void cClientSocketDevice::SendAnswer(int)
{
    m_pZDSWriteNotifyer->setEnabled(false);
    m_pSock->writeBlock(m_sOutput.latin1(), m_sOutput.length());
}


cDeviceServer::cDeviceServer( Q_UINT16 port, int backlog, QObject *parent, const char *name)
    :Q3ServerSocket(port, backlog, parent, name) 
{
}    


void cDeviceServer::CloseConnection(cClientIODevice* ciod)
{
    int i;
    cSCPIFace* delIFace;
    for (i = 0;;i++) // wir suchen nach dem interface dessen client io device sich abgemeldet hat
	if ( m_SCPIFacePtrList.at(i)->GetClientIODevice() == ciod )
	    break;
    
    delIFace = m_SCPIFacePtrList.at(i);
    m_SCPIFacePtrList.remove(delIFace); // entfernen es aus der liste
    delete delIFace; // und löschen es
    if ( m_SCPIFacePtrList.isEmpty() ) // wenn wir keinen mehr haben
	emit RemoteCtrlInfo( false); // wird der remote betrieb aufgehoben 
    setPriority(); // alle clients werden informiert
}


void cDeviceServer::setPriority() // alle clients werden informiert
{
    uint i;
    for (i = 0; i < m_SCPIFacePtrList.count(); i++)
	m_SCPIFacePtrList.at(i)->ReceivePriority(i);
}
    
cwm3000DeviceServer::cwm3000DeviceServer( Q_UINT16 port, int backlog, QObject * parent, const char * name)
    :cDeviceServer( port, backlog, parent, name)
{
    m_pV = 0;
}


void cwm3000DeviceServer::newConnection(int fd)
{
    cClientSocketDevice* csd = new cClientSocketDevice( fd, parent(), name() );
    cWM3000SCPIFace* iface = new cWM3000SCPIFace( csd, 20);
			     
    if  (m_SCPIFacePtrList.isEmpty() )
	emit RemoteCtrlInfo(true); // wir informieren wenn sich der 1. client verbunden hat
    
    m_SCPIFacePtrList.append( iface); // das neue interface in die liste
    setPriority(); // alle clients werden informiert
    connect( csd, SIGNAL( connectionClosed(cClientIODevice*)),SLOT(CloseConnection(cClientIODevice*)) ); // wenn sich der client abgemeldet hat
    
    connect( this, SIGNAL( SendIFaceConfiguration(cConfData*)), iface, SLOT(ReceiveConfiguration(cConfData*)) ); 
    connect( iface, SIGNAL( SendConfiguration(cConfData*)), this, SLOT(ReceiveIFaceConfiguration(cConfData*)) );
    iface->setConfiguration(&m_ConfData); // neue interfaces werden 1x expl. informiert
    connect( iface, SIGNAL(EN61850StatusRequest()), this, SLOT(ReceiveENInfoReguest())); 
    connect( this, SIGNAL(SendIFaceETHStatus(cEN61850Info*)), iface, SLOT( ReceiveEN61850Status(cEN61850Info*)));
    connect( iface, SIGNAL(ResetETHStatus()), this, SLOT(ReceiveResetETHStatusCommand())); 
    connect( iface, SIGNAL(SendRange(cConfData*)), this, SLOT(ReceiveIFaceRangeCommand(cConfData*))); 
    iface->SetRangeListSlot(m_sNXRangeList, m_sEVTRangeList); // wir setzen die liste 1x
    connect( iface, SIGNAL(SetDefaultMeasConfig()), this, SLOT(ReceiveIFaceDefaultConfig())); 
    connect( this, SIGNAL(ConfirmRangeAutoRdy()), iface, SLOT( ReceiveRangeAutomaticRdy()));
    connect( this, SIGNAL(SendActualValues(cwmActValues*)), iface, SLOT(ReceiveActValues(cwmActValues*)));
    
    connect( this, SIGNAL(SendLPValue(cwmActValues*)), iface, SLOT(ReceiveLPValue(cwmActValues*)));
    
    connect( this, SIGNAL(SendVersionInfo(tVersSerial*)), iface, SLOT(ReceiveVersionInfo(tVersSerial*)));
    iface->ReceiveVersionInfo(m_pV); // neue interfaces werden 1x expl. informiert
    connect( iface, SIGNAL(SelftestRequest()), this, SLOT(ReceiveSelftestRequest())); 
    connect( this, SIGNAL(SendSelftestResult(int)), iface, SLOT(ReceiveSelftestResult(int)));

    connect( iface, SIGNAL(ChannelNOffsetMeasureRequest()), this, SLOT(ReceiveChannelNOffsetMeasureRequest()));
    connect( iface, SIGNAL(ChannelXOffsetMeasureRequest()), this, SLOT(ReceiveChannelXOffsetMeasureRequest()));
    connect( this, SIGNAL(SendOffsetResult(double)), iface, SLOT(ReceiveNXOffset(double)));

    connect( this, SIGNAL(AffectStatus(uchar, ushort)), iface, SLOT(AffectSCPIStatus(uchar, ushort)));
    
}
    

void cwm3000DeviceServer::newConnection(cClientIODevice*)
{
    
}


void cwm3000DeviceServer::ReceiveDeviceConfiguration(cConfData* cd) // empfangen der conf. vom device
{
    m_ConfData = *cd; // wir übernehmen die  konf. vom  device
    emit SendIFaceConfiguration(cd);  // und weitergeben an die interfaces
}
 

void cwm3000DeviceServer::ReceiveIFaceConfiguration(cConfData* cd) // empfangen der conf. vom interface
{
    emit SendDeviceConfiguration(cd); // und weiterleiten ans device
}


void cwm3000DeviceServer::ReceiveETHStatus( cEN61850Info* eni)
{
    emit SendIFaceETHStatus(eni);
}

void cwm3000DeviceServer::ReceiveENInfoReguest()
{
    emit RequestEN61850Info(); // anfordern der info beim device
}


void cwm3000DeviceServer::ReceiveSelftestResult(int r)
{
    emit SendSelftestResult(r);
}


void cwm3000DeviceServer::ReceiveSelftestRequest()
{
    emit RequestSelftest(); // selbsttest durchführen lassen
}


void cwm3000DeviceServer::ReceiveOffsetResult(double offs)
{
    emit SendOffsetResult(offs);
}


void cwm3000DeviceServer::ReceiveChannelNOffsetMeasureRequest()
{
    emit RequestChannelNOffsetMeasure();
}


void cwm3000DeviceServer::ReceiveChannelXOffsetMeasureRequest()
{
    emit RequestChannelXOffsetMeasure();
}


void cwm3000DeviceServer::ReceiveResetETHStatusCommand()
{
    emit ResetETHStatus();
}


void cwm3000DeviceServer::ReceiveIFaceRangeCommand(cConfData* cd)
{
    emit SendRangeCommand(cd);
}


void cwm3000DeviceServer::SetRangeListSlot( cWMRangeList& nx,  cWMRangeList& evt)
{
    m_sNXRangeList = nx;
    m_sEVTRangeList = evt;
}
    

void cwm3000DeviceServer::ReceiveIFaceDefaultConfig() // das device soll default konfig. setzen
{
    emit SendDefaultConfigCommand();
}


void cwm3000DeviceServer::ReceiveRangeAutomaticDone() // rückmeldung hierfür
{
    emit ConfirmRangeAutoRdy();
}


void cwm3000DeviceServer::ReceiveActualValues(cwmActValues* av)
{
    emit SendActualValues(av);
}


void cwm3000DeviceServer::ReceiveLPValue(cwmActValues* av)
{
    emit SendLPValue(av);
}


void cwm3000DeviceServer::ReceiveVersionInfo(tVersSerial *v)
{
    m_pV = v;
    emit SendVersionInfo(m_pV);
}


void cwm3000DeviceServer::ReceiveAffectStatus(uchar act, ushort stat)
{
    emit AffectStatus(act, stat);
}



