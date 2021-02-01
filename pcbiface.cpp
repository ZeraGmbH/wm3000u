// implementation cPCBIFace

#include <q3textstream.h>
#include <qdir.h>

#include "pcbiface.h"


cPCBIFace::cPCBIFace(QString hn, int hp)
    : cZHServerIFace(hn, hp, 5000)
{
    QObject::connect(iFaceSock,SIGNAL(DataReceived()),this,SLOT(SocketDoneSlot()));
    QObject::connect(iFaceSock,SIGNAL(connected()),this,SLOT(SocketDoneSlot()));
    QObject::connect(iFaceSock,SIGNAL(Error()),this,SLOT(SocketErrorSlot()));
    QObject::connect(iFaceSock,SIGNAL(AsyncDataReceived(const QString&)),this,SIGNAL(iFaceAsync(const QString&))); // dieses Signal reichen wir einfach durch
    m_sHost = hn;
    m_nPort = hp;
    m_ActTimer = new cSMTimer();
    QObject::connect(m_ActTimer,SIGNAL(timeout(int)),this,SLOT(ActionHandler(int)));
}


cPCBIFace::~cPCBIFace()
{
    iFaceSock->clearPendingData() ;
    iFaceSock->close();
    delete m_ActTimer;
}


void cPCBIFace::ActionHandler(int entryAHS)
{
    static int AHS = pcbIFaceIdle;
        
    if ( entryAHS != pcbIFaceContinue )
    { // bei continue machen wir beim internen state weiter
        if (AHS != pcbIFaceIdle)
        { // wir sollen was neues starten
            qDebug ("PCBIFace reentered with new AHS=%d while AHS=%d , check your program !",entryAHS, AHS); // das darf eigentlich nicht passieren
            return; // und sind fertig
        }
        else
        {
            m_biFaceError = false; // wir haben noch keinen fehler
            AHS = entryAHS; // wir starten es
        }
    }
    
    switch (AHS)
    {
    case pcbIFaceConnectYourselfStart:
        iFaceSock->connectToHost(m_sHost,m_nPort); // verbindung zum leiterkarten server herstellen
        AHS++;
        break; // pcbIFaceConnectYourselfStart
	
    case JustFlashProgFinished:
    case JustFlashExportFinished:
    case JustFlashImportFinished:
    case JustFlashGetChksumFinished:
    case setPhaseNodeInfoFinished:
    case setPhaseStatusFinished:
    case cmpPhaseCoefficientFinished:
    case setOffsetNodeInfoFinished:
    case setOffsetStatusFinished:
    case cmpOffsetCoefficientFinished:
    case SetSenseModeFinished:
    case ReadPhaseCorrectionFinished:	
    case ReadGainCorrectionFinished:
    case ReadOffsetCorrectionFinished:
    case SetSyncTimingFinished:	
    case SetSyncSourceFinished:	
    case SetSamplingPSamplesFinished:	
    case SetSamplingFrequencyFinished:	
    case SwitchRangeFinished:
    case ReadRangeFinished:	
    case OpenChannelFinished:
    case pcbIFaceConnectionFinished:
    case PCBReadDeviceVersionFinished:
    case PCBReadServerVersionFinished:
    case PCBReadSerialNrFinished:
    case PCBReadJustdataVersionFinished:
    case GetAdjStatusFinished:
    case SetSenseProtectionFinished:	
        if (m_biFaceError)
            emit iFaceError();
        else
            emit iFaceDone();
        AHS = pcbIFaceIdle; // wir sind durch
        break; // pcbIFaceConnectionFinished
	
    case OpenChannelStart:
        SendOpenChannelCommand();
        AHS++;
        break; // OpenChannelStart
	
    case ReadRangeStart:
        SendReadRangeCommand();
        AHS++;
        break;
	
    case SwitchRangeStart:
        SendSwitchRangeCommand();
        AHS++;
        break; // SwitchRangeStart
	
    case SetSamplingFrequencyStart:
        SendSetSamplingFrequencyCommand();
        AHS++;
        break; // SetSamplingFrequencyStart
	
    case SetSamplingPSamplesStart:
        SendSetSamplingPSamplesCommand();
        AHS++;
        break; // SetSamplingPSamplesStart
	
    case SetSenseModeStart:
        SendSetSenseModeCommand();
        AHS++;
        break; // SetSenseModeStart
	
    case SetSyncSourceStart:
        SendSetSyncSourceCommand();
        AHS++;
        break; // SetSyncSourceStart
	
    case SetSyncTimingStart:
        SendSetSyncTimingCommand();
        AHS++;
        break; // SetSyncTimingStart
	
    case ReadGainCorrectionStart:
        SendReadGainCorrectionCommand();
        AHS++;
        break; // ReadGainCorrectionStart
	
    case ReadPhaseCorrectionStart:
        SendReadPhaseCorrectionCommand();
        AHS++;
        break; // ReadPhaseCorrectionStart

    case ReadOffsetCorrectionStart:
        SendReadOffsetCorrectionCommand();
        AHS++;
        break; // ReadOffsetCorrectionStart

    case setPhaseNodeInfoStart:
        SendsetPhaseNInfoCommand();
        AHS++;
        break; // setPhaseNodeInfoStart

    case setPhaseStatusStart:
        SendsetPhaseStatusCommand();
        AHS++;
        break; // setPhaseStatusStart
	
    case cmpPhaseCoefficientStart:
        SendcmpPhaseCoefficientCommand();
        AHS++;
        break; // setPhaseNodeInfoStart

    case setOffsetNodeInfoStart:
        SendsetOffsetNInfoCommand();
        AHS++;
        break; // setPhaseNodeInfoStart

    case setOffsetStatusStart:
        SendsetOffsetStatusCommand();
        AHS++;
        break; // setOffsetStatusStart

    case cmpOffsetCoefficientStart:
        SendcmpOffsetCoefficientCommand();
        AHS++;
        break; // setPhaseNodeInfoStart
	
    case JustFlashProgStart:
        SendJustFlashProgCommand();
        AHS++;
        break; // JustFlashProgStart
    
    case JustFlashExportStart:
        SendJustFlashExportCommand();
        AHS++;
        break;
	
    case JustFlashImportStart:
        SendJustFlashImportCommand();
        AHS++;
        break;
	
    case JustFlashGetChksumStart:
        SendReadFlashChksumCommand();
        AHS++;
        break;
	
    case PCBReadDeviceVersionStart:
        SendReadDeviceVersionCommand();
        AHS++;
        break;
	
    case PCBReadServerVersionStart:
        SendReadServerVersionCommand();
        AHS++;
        break;
	
    case PCBReadSerialNrStart:
        SendReadSerialNrCommand();
        AHS++;
        break;
	
    case PCBReadJustdataVersionStart:
        SendReadJustdataVersionCommand();
        AHS++;
        break;

    case GetAdjStatusStart:
        SendGetAdjStatusCommand();
        AHS++;
        break;
	
    case SetSenseProtectionStart:
        SendSetSenseProtectionCommand();
        AHS++;
        break;
	
   }
}


void cPCBIFace::SocketDoneSlot()
{
    m_ActTimer->start(0,pcbIFaceContinue); // statemachine wird gestartet
}


void cPCBIFace::SocketErrorSlot()
{
    m_biFaceError = true; // fehler merken
    m_ActTimer->start(0,pcbIFaceContinue); // statemachine wird gestartet
}


void cPCBIFace::connectYourself()
{
    m_ActTimer->start(0,pcbIFaceConnectYourselfStart);
}
	

void cPCBIFace::openChannel(int cn)
{
    m_nP1 = cn;
    m_ActTimer->start(0,OpenChannelStart);
}


void cPCBIFace::readRange(int cn)
{
    m_nP1 = cn;
    m_ActTimer->start(0,ReadRangeStart);
}


void cPCBIFace::switchRange(int chn, QString rng) // schaltet den bereich (QString) im kanal (int)
{
    m_nP1 = chn;
    m_sP1 = rng;
    m_ActTimer->start(0,SwitchRangeStart);
}


void cPCBIFace::setSamplingFrequency(float fr) // abtast frequenz setzen
{
    m_fP1 = fr;
    m_ActTimer->start(0,SetSamplingFrequencyStart);
}


void cPCBIFace::setSamplingPSamples(int ps) // samples/periode einstellen
{
    m_nP1 = ps;
    m_ActTimer->start(0,SetSamplingPSamplesStart);
}


void cPCBIFace::setSenseMode(int sm) // test modus einstellen
{
    m_nP1 = sm;
    m_ActTimer->start(0,SetSenseModeStart);
}


void cPCBIFace::setSyncSource(int sm)  // sync mode einstellen
{
    m_nP1 = sm;
    m_ActTimer->start(0,SetSyncSourceStart);
}


void cPCBIFace::setSyncTiming(int st)  // sync timing einstellen
{
    m_nP1 = st;
    m_ActTimer->start(0,SetSyncTimingStart);
}


 
void cPCBIFace::readGainCorrection(int ch, QString sRange, float actVal)
{
    m_nP1 = ch;
    m_sP1 = sRange;
    m_fP1 = actVal;
    m_ActTimer->start(0,ReadGainCorrectionStart);
}


void cPCBIFace::readPhaseCorrection(int ch, QString sRange, float actVal)
{
    m_nP1 = ch;
    m_sP1 = sRange;
    m_fP1 = actVal;
    m_ActTimer->start(0,ReadPhaseCorrectionStart);
}


void cPCBIFace::readOffsetCorrection(int ch, QString sRange, float actVal)
{
    m_nP1 = ch;
    m_sP1 = sRange;
    m_fP1 = actVal;
    m_ActTimer->start(0,ReadOffsetCorrectionStart);
}


void cPCBIFace::setPhaseNodeInfo(QString chn, QString rng, int index, float node, float arg)
{
    m_sP1 = chn;
    m_sP2 = rng;
    m_nP1 = index;
    m_fP1 = node;
    m_fP2 = arg;
    m_ActTimer->start(0,setPhaseNodeInfoStart);
}


void cPCBIFace::setPhaseStatus(QString chn, QString rng, int stat)
{
    m_sP1 = chn;
    m_sP2 = rng;
    m_nP1 = stat;
    m_ActTimer->start(0,setPhaseStatusStart);
}


void cPCBIFace::cmpPhaseCoefficient(QString chn)
{
    m_sP1 = chn;
    m_ActTimer->start(0,cmpPhaseCoefficientStart);
}


void cPCBIFace::setOffsetNodeInfo(QString chn, QString rng, int index, float node, float arg)
{
    m_sP1 = chn;
    m_sP2 = rng;
    m_nP1 = index;
    m_fP1 = node;
    m_fP2 = arg;
    m_ActTimer->start(0,setOffsetNodeInfoStart);
}


void cPCBIFace::setOffsetStatus(QString chn, QString rng, int stat)
{
    m_sP1 = chn;
    m_sP2 = rng;
    m_nP1 = stat;
    m_ActTimer->start(0,setOffsetStatusStart);
}


void cPCBIFace::cmpOffsetCoefficient(QString chn)
{
    m_sP1 = chn;
    m_ActTimer->start(0,cmpOffsetCoefficientStart);
}

void cPCBIFace::JustFlashProgram()
{
    m_ActTimer->start(0,JustFlashProgStart);
}
    
void cPCBIFace::JustFlashExport(QString fname)
{
    m_sP1 = fname;
    m_ActTimer->start(0,JustFlashExportStart);
}


void cPCBIFace::JustFlashImport(QString fname)
{
    m_sP1 = fname;
    m_ActTimer->start(0,JustFlashImportStart);
}


void cPCBIFace::JustFlashGetChksum()
{
    m_ActTimer->start(0,JustFlashGetChksumStart);
}


void cPCBIFace::ReadDeviceVersion()
{
    m_ActTimer->start(0,PCBReadDeviceVersionStart);
}


void cPCBIFace::GetAdjustmentStatus()
{
    m_ActTimer->start(0,GetAdjStatusStart);
}


void cPCBIFace::ReadServerVersion()
{
    m_ActTimer->start(0,PCBReadServerVersionStart);   
}


void cPCBIFace::ReadSerialNr()
{
    m_ActTimer->start(0,PCBReadSerialNrStart);
}


void cPCBIFace::ReadJustDataVersion()
{
    m_ActTimer->start(0,PCBReadJustdataVersionStart);
}


void cPCBIFace::SetSenseProtection(int p)
{
    m_nP1 = p;
    m_ActTimer->start(0,SetSenseProtectionStart);       
}


void cPCBIFace::SendOpenChannelCommand()
{
    QString s = QString ("sense:ch%1:open\n").arg(m_nP1);
    iFaceSock->SendCommand(s);
}


void cPCBIFace::SendReadRangeCommand()
{
    QString s= QString ("sense:ch%1:range ?\n").arg(m_nP1);
    iFaceSock->SendQuery(s); // kommando an server 
}
    
 
void cPCBIFace::SendSwitchRangeCommand()
{
    QString s = QString("sense:ch%1:range %2\n").arg(m_nP1).arg(m_sP1);
    iFaceSock->SendCommand(s);
}


void cPCBIFace::SendSetSamplingFrequencyCommand()
{
    QString cmds = QString("syst:samp:freq %1\n").arg(m_fP1);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendSetSamplingPSamplesCommand()
{
    QString cmds = QString("syst:samp:psamples %1\n").arg(m_nP1);
    iFaceSock->SendCommand(cmds);
}	 


void cPCBIFace::SendSetSenseModeCommand()
{
    QString cmds = QString("syst:samp:mode %1\n").arg(m_nP1);
    iFaceSock->SendCommand(cmds);
}	 


void cPCBIFace::SendSetSyncSourceCommand() // sync mode einstellen
{
    QString s = QString("syst:sync:source %1\n").arg(m_nP1);
    iFaceSock->SendCommand(s);
}


void cPCBIFace::SendSetSyncTimingCommand() // sync timing einstellen
{
    QString cmds = QString("syst:sync:period %1\n").arg(m_nP1);
    iFaceSock->SendCommand(cmds);
}

void cPCBIFace::SendReadGainCorrectionCommand()
{
    QString cmds = QString("calc:ch%1:%2:camp %3?\n").arg(m_nP1).arg(m_sP1).arg(m_fP1);    
    iFaceSock->SendQuery(cmds);
}

void cPCBIFace::SendReadPhaseCorrectionCommand()
{
    QString cmds = QString("calc:ch%1:%2:cph %3?\n").arg(m_nP1).arg(m_sP1).arg(m_fP1);
    iFaceSock->SendQuery(cmds);
}


void cPCBIFace::SendReadOffsetCorrectionCommand()
{
    QString cmds = QString("calc:ch%1:%2:coff %3?\n").arg(m_nP1).arg(m_sP1).arg(m_fP1);
    iFaceSock->SendQuery(cmds);
}

void cPCBIFace::SendsetPhaseNInfoCommand()
{
    QString cmds = QString("calc:%1:%2:pcn%3 %4,%5\n").arg(m_sP1).arg(m_sP2).arg(m_nP1).arg(m_fP1).arg(m_fP2);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendsetPhaseStatusCommand()
{
    QString cmds = QString("calc:%1:%2:pst %3\n").arg(m_sP1).arg(m_sP2).arg(m_nP1);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendcmpPhaseCoefficientCommand()
{
    QString cmds = QString("calc:%1:comp:cph\n").arg(m_sP1);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendsetOffsetNInfoCommand()
{
    QString cmds = QString("calc:%1:%2:ocn%3 %4,%5\n").arg(m_sP1).arg(m_sP2).arg(m_nP1).arg(m_fP1).arg(m_fP2);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendsetOffsetStatusCommand()
{
    QString cmds = QString("calc:%1:%2:ost %3\n").arg(m_sP1).arg(m_sP2).arg(m_nP1);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendcmpOffsetCoefficientCommand()
{
    QString cmds = QString("calc:%1:comp:coff\n").arg(m_sP1);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendJustFlashProgCommand()
{
    QString cmds = "mmem:writ\n";
    iFaceSock->SendCommand(cmds);
}

void cPCBIFace::SendJustFlashExportCommand()
{
    QString cmds = QString("mmem:exp %1\n").arg(m_sP1);
    iFaceSock->SendCommand(cmds);
}

void cPCBIFace::SendJustFlashImportCommand()
{
    QString cmds = QString("mmem:imp %1\n").arg(m_sP1);
    iFaceSock->SendCommand(cmds);
}


void cPCBIFace::SendReadDeviceVersionCommand()
{
    QString cmds = QString("syst:vers:dev?\n");
    iFaceSock->SendQuery(cmds);  
}


void cPCBIFace::SendReadServerVersionCommand()
{
    QString cmds = QString("syst:vers:serv?\n");
    iFaceSock->SendQuery(cmds);    
}


void cPCBIFace::SendReadSerialNrCommand()
{
    QString cmds = QString("syst:sern?\n");
    iFaceSock->SendQuery(cmds);
}


void cPCBIFace::SendReadJustdataVersionCommand()
{
    QString cmds = QString("syst:vers:adj?\n");
    iFaceSock->SendQuery(cmds);
}


void cPCBIFace::SendReadFlashChksumCommand()
{
    QString cmds = QString("mmem:chks?\n");
    iFaceSock->SendQuery(cmds);    
}	


void cPCBIFace::SendGetAdjStatusCommand()
{
    QString cmds = QString("stat:adj?\n");
    iFaceSock->SendQuery(cmds);    
}	
    

void cPCBIFace::SendSetSenseProtectionCommand()
{
    QString s = QString ("sens:prot %1\n").arg(m_nP1);
    iFaceSock->SendCommand(s);
}

