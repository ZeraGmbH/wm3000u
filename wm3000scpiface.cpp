#include <qfile.h>

#include "tools.h"
#include "confdata.h"
#include "eparameter.h"
#include "wm3000scpiface.h"
#include "wm3000u.h"

extern cWM3000U* g_WMDevice;
extern  scpiErrorType SCPIError[];
extern char* MModeName[];
char* SModeName[maxSMode] = {(char*)"AC",(char*)"DC"};
char* FreqName[MaxFreq] = {(char*)"16.67",(char*)"50.0",(char*)"60.0"};
double SFrequency[MaxFreq] = {16.67, 50.0, 60.0};
char* SRatesName[MaxSRate] = {(char*)"80",(char*)"256"}; // abtastraten
int SRates[MaxSRate] = {80,256};
char* SSourceName[MaxSSource] = {(char*)"intern",(char*)"extern"};


cWM3000SCPIFace::cWM3000SCPIFace(cClientIODevice* ciod, short l)
    :cSCPIFace(ciod, l)
{
    m_pCommands = InitScpiCmdTree(m_pCommands); // verketten der common und scpi commands
    m_nWait4What = wait4Nothing;
    m_bAddEventError = false;
    mMeasChannelList << "N" << "X" << "EVT";
    m_pVersion  = 0;
}


char* cWM3000SCPIFace::GetDeviceIdentification()
{
    QString s;
    if (m_pVersion)
	s = "ZERA Gmbh," + m_pVersion->DeviceName + ","+m_pVersion->PCBSerialNr + "," +		m_pVersion->DeviceVersion;
    else
	s = "UnKnown";

    return sAlloc(s);
}


char* cWM3000SCPIFace::DeviceSelfTest()
{
    m_pSMachineTimer->start(0, ifSelftestStart);
    return 0;
}


void cWM3000SCPIFace::ResetDevice()
{
    emit SetDefaultMeasConfig(); // die messeinrichtung stellt defaults ein
    
    m_ConfDataTarget.m_sRangeNVorgabe = "480V";
    m_ConfDataTarget.m_sRangeXVorgabe = "480V";
    m_ConfDataTarget.m_sRangeEVTVorgabe = "15.0V";
    m_ConfDataTarget.m_bSimulation = false; // reset schaltet auf normalen messbetrieb
    m_ConfDataTarget.m_bRunning = true; // läuft 
    
    emit SendRange(&m_ConfDataTarget); // bereich automatik einstellen
    
    
    cSCPIFace::ResetDevice();
}


char *cWM3000SCPIFace::GetChannelNOffset()
{
    m_pSMachineTimer->start(0, ifChannelNOffsetStart);
    return 0;
}


char *cWM3000SCPIFace::GetChannelXOffset()
{
    m_pSMachineTimer->start(0, ifChannelXOffsetStart);
    return 0;
}


void cWM3000SCPIFace::setConfiguration(cConfData* cd)
{
    m_ConfDataActual = m_ConfDataTarget =  *cd;
}


void cWM3000SCPIFace::ReceiveConfiguration(cConfData* cd)
{
    m_ConfDataActual = /*m_ConfDataTarget = */ *cd;
}


// system modell routinen
 
char* cWM3000SCPIFace::mGetScpiErrorAll()
{
    
    int n = m_ErrEventQueue.size();
    if (n == 0)
	return mGetScpiError();
    else
    {
	QString rm;
	rm = mGetScpiError();
	if (n > 1)
	    for (int i = 1;i < n; i++)
	    {
            rm += ";";
            rm += mGetScpiError();
	    }
	return sAlloc(rm);
    }
}


char* cWM3000SCPIFace::mGetScpiErrorCount()
{
    int n = m_ErrEventQueue.size();
    QString rm = QString("+%1").arg(n);
    return sAlloc(rm);
}


char* cWM3000SCPIFace::mGetScpiError()
{
    QString rm;
    if ( m_ErrEventQueue.empty() )
	rm = QString("+%1,%2").arg(SCPIError[0].ErrNum).arg(SCPIError[0].ErrTxt);
    else
    {
	int e = m_ErrEventQueue.first(); // fifo
	m_ErrEventQueue.pop_front(); //  eintrag löschen wenn gelesen
	if ( m_ErrEventQueue.empty() )
	    ResetSTB(STBeeQueueNotEmpty);
	rm = QString("%1,%2").arg(SCPIError[e].ErrNum).arg(SCPIError[e].ErrTxt);
	
    }
    
    return sAlloc(rm);
}


char* cWM3000SCPIFace::mGetSerialNumber()
{
    QString s;
    if (m_pVersion)
	s = m_pVersion->PCBSerialNr;
    else
	s = "UnKnown";
	
    return sAlloc(s);
}


char* cWM3000SCPIFace::mGetDSPVersion()
{
    QString s;
    if (m_pVersion)
	s = m_pVersion->DSPVersion;
    else
	s = "UnKnown";

    return sAlloc(s);
}


char* cWM3000SCPIFace::mGetPCBVersion()
{
    QString s;
    if (m_pVersion)
	s = m_pVersion->PCBVersion;
    else
	s = "UnKnown";

    return sAlloc(s);
}
 

char* cWM3000SCPIFace::mGetDeviceVersion()
{
    return sAlloc(m_pVersion->DeviceVersion);
}
  
// status modell routinen
void cWM3000SCPIFace::mSetStatusEN61850Clear(char*)
{
    emit ResetETHStatus();
}


char*  cWM3000SCPIFace::mGetStatusEN61850SynclostCount()
{
    m_pSMachineTimer->start(0, EN61850SynclostCountStart); // wir starten die statemachine weil
    return 0; // der en61850 status erst noch gelesen werden muss
}


char* cWM3000SCPIFace::mGetStatusEN61850Error()
{
    m_pSMachineTimer->start(0, EN61850ErrorStart);
    return 0;
}
 

char* cWM3000SCPIFace::mGetStatusEN61850DataCount()
{
     m_pSMachineTimer->start(0, EN61850DataCountStart);
     return 0;
}


void cWM3000SCPIFace::ReceiveEN61850Status(cEN61850Info* eni)
{
    if (m_nWait4What == wait4ENStatus) // wir machen nur was draus wenn wir drauf warten
    {
	m_nWait4What = wait4Nothing;
	m_EN61850Info = *eni; // wir haben die angeforderte info
	m_pSMachineTimer->start(0, ExecCmdContinue); // und geben die kontrolle an die statemachine
    }
}    
    

void cWM3000SCPIFace::ReceiveSelftestResult(int r) 
{
    if (m_nWait4What == wait4SelftestResult) // wir machen nur was draus wenn wir drauf warten
    {
        m_nWait4What = wait4Nothing;
        SelftestResult = r;
        m_pSMachineTimer->start(0, ExecCmdContinue); // und geben die kontrolle an die statemachine
    }
}


void cWM3000SCPIFace::ReceiveNXOffset(double offs)
{
    if (m_nWait4What == wait4Offsetresult)
    {
        m_nWait4What = wait4Nothing;
        OffsetResult = offs;
        m_pSMachineTimer->start(0, ExecCmdContinue); // und geben die kontrolle an die statemachine
    }
}

    
void cWM3000SCPIFace::CmdExecution(QString& s)
{
    char* lcmd = (char*) malloc (s.length()+1); 
    strcpy(lcmd, s.latin1());
    answ = 0;
    bool ok = m_pCmdInterpreter->CmdExecute(m_pCommands, this, lcmd, answ);
    free(lcmd);    
    if ( !ok) // ok bedeutet kommando interpreter hat kommando dekodiert  
    {
	AddEventError(CommandError); // wenn nicht ok fehler eintragen
	m_pSMachineTimer->start(0, ExecCmdFinished); // kommen wir von hier zurück zur statemachine und beenden die bearbeitung 
    }
}
 

void cWM3000SCPIFace::SetRangeListSlot( cWMRangeList& nx,  cWMRangeList& evt)
{
    CWMRange *Range;
    Q3PtrListIterator<CWMRange> it(nx);
   
   m_sNXItemList.clear(); // liste erst mal leeren
   for(Range=it.toFirst();Range;Range=++it) m_sNXItemList.append(Range->Name());
      
   it=evt;
   m_sEVTItemList.clear(); // liste erst mal leeren   
   for(Range=it.toFirst();Range;Range=++it) m_sEVTItemList.append(Range->Name());
}


void cWM3000SCPIFace::ReceiveRangeAutomaticRdy()
{
    if (m_nWait4What == wait4RangeAutomatic) // wir machen nur was draus wenn wir drauf warten
    {
	m_nWait4What = wait4Nothing;
	m_pSMachineTimer->start(0, ExecCmdContinue); // und geben die kontrolle an die statemachine
    }    
}


void cWM3000SCPIFace::ReceiveActValues(cwmActValues* av)
{
    if (m_nWait4What == wait4MeasurementData) // wir machen nur was draus wenn wir drauf warten
    {
	m_nWait4What = wait4Nothing;
	mActValues = *av;
	m_pSMachineTimer->start(0, ExecCmdContinue); // und geben die kontrolle an die statemachine
    }    
}


void cWM3000SCPIFace::ReceiveLPValue(cwmActValues* av)
{
    if (m_nWait4What == wait4LoadpointData) // wir machen nur was draus wenn wir drauf warten
    {
	m_nWait4What = wait4Nothing;
	mActValues.LoadPoint = av->LoadPoint;
	mActValues.LoadPoint1 = av->LoadPoint1;
    mActValues.LoadPointX = av->LoadPointX;
    mActValues.LoadPoint1X = av->LoadPoint1X;
	m_pSMachineTimer->start(0, ExecCmdContinue); // und geben die kontrolle an die statemachine
    }    
}


void cWM3000SCPIFace::ReceiveVersionInfo(tVersSerial* v)
{
    m_pVersion = v;
}

void cWM3000SCPIFace::mSetStatusQuestionablePTrans(char* s)
{
    ushort us;
    if (GetParameter(&s, us,true))
	m_pQuestionableStat->SetPTransition(us);        
}
    
 
char* cWM3000SCPIFace::mGetStatusQuestionablePTrans()
{
    QString rs;
    rs = QString("+%1").arg(m_pQuestionableStat->GetPTransition());
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetStatusQuestionableNTrans(char* s)
{
    ushort us;
    if (GetParameter(&s, us,true))
	m_pQuestionableStat->SetNTransition(us);        
}
    
 
char* cWM3000SCPIFace::mGetStatusQuestionableNTrans()
{
    QString rs;
    rs = QString("+%1").arg(m_pQuestionableStat->GetNTransition());
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetStatusQuestionableEnable(char* s)
{
    ushort us;
    if (GetParameter(&s, us,true))
	m_pQuestionableStat->SetEnable(us);    
}
    
 
char* cWM3000SCPIFace::mGetStatusQuestionableEnable()
{
    QString rs;
    rs = QString("+%1").arg(m_pQuestionableStat->GetEnable());
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetStatusQuestionableCondition(char* s)
{
    ushort ns;
    if (GetParameter(&s, ns,true))
	m_pQuestionableStat->SetCondition(ns);
}


char* cWM3000SCPIFace::mGetStatusQuestionableCondition()
{
    QString rs;
    rs = QString("+%1").arg(m_pQuestionableStat->GetCondition());
    return sAlloc(rs);
}


char* cWM3000SCPIFace::mGetStatusQuestionableEvent()
{
    QString rs;
    rs = QString("+%1").arg(m_pQuestionableStat->GetEvent());
    return sAlloc(rs);
}

 
void cWM3000SCPIFace::mSetStatusOperationPTrans(char* s)
{
    ushort us;
    if (GetParameter(&s, us,true))
	m_pOperationStat->SetPTransition(us);
}
 

char* cWM3000SCPIFace::mGetStatusOperationPTrans()
{
    QString rs;
    rs = QString("+%1").arg(m_pOperationStat->GetPTransition());
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetStatusOperationNTrans(char* s)
{
    ushort us;
    if (GetParameter(&s, us,true))
	m_pOperationStat->SetNTransition(us);
}
 

char* cWM3000SCPIFace::mGetStatusOperationNTrans()
{
    QString rs;
    rs = QString("+%1").arg(m_pOperationStat->GetNTransition());
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetStatusOperationEnable(char* s)
{
    ushort us;
    if (GetParameter(&s, us,true))
	m_pOperationStat->SetEnable(us);
}
 

char* cWM3000SCPIFace::mGetStatusOperationEnable()
{
    QString rs;
    rs = QString("+%1").arg(m_pOperationStat->GetEnable());
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetStatusOperationCondition(char* s)
{
    ushort us;
    if (GetParameter(&s, us,true))
	m_pOperationStat->SetCondition(us);
}


char* cWM3000SCPIFace::mGetStatusOperationCondition()
{
    QString rs;
    rs = QString("+%1").arg(m_pOperationStat->GetCondition());
    return sAlloc(rs);
}


char* cWM3000SCPIFace::mGetStatusOperationEvent()
{
    QString rs;
    rs = QString("+%1").arg(m_pOperationStat->GetEvent());
    return sAlloc(rs);
}


char* cWM3000SCPIFace::mGetStatusStandard()
{
    return  GetDeviceSTB();
}
			  
// sense model routinen    
char* cWM3000SCPIFace::mOutRangeCatalog()
{
    int i;
    QString dedicatedChannel = m_pCmdInterpreter->dedicatedList.first();
    QString s = "";
    QStringList* sl;
    int n = mMeasChannelList.findIndex( dedicatedChannel);
    
    switch (n)
    {
    case 0:
    case 1: sl = &m_sNXItemList; break;
    case 2: sl = &m_sEVTItemList; break;
    default: break;
    }
    
    for (i = 0; i < sl->count()-1; i++)
	s = s + (*sl)[i] +";";
    
    s += (*sl)[i];
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetRange(char* s)
{
    if ( isAuthorized())
    {
	QString range;
	QString* nrange;
	QString dedicatedChannel = m_pCmdInterpreter->dedicatedList.first();
	QStringList* sl;
	int n = mMeasChannelList.findIndex( dedicatedChannel);
    
	switch (n)
	{
	case 0: sl = &m_sNXItemList; nrange = &m_ConfDataTarget.m_sRangeNVorgabe;break;
	case 1: sl = &m_sNXItemList; nrange = &m_ConfDataTarget.m_sRangeXVorgabe;break;
	case 2: sl = &m_sEVTItemList; nrange = &m_ConfDataTarget.m_sRangeEVTVorgabe;break;
	default: break;
	}
    
	if (GetParameter(&s, range, true))
	{
	    if ( sl->contains(range) )
	    {
		*nrange = range;
		emit SendRange(&m_ConfDataTarget);
	    }
	    else
		AddEventError(ParameterNotAllowed);
	 }
	else
	    AddEventError(InvalidSeparator);
    }
    else
	AddEventError(CommandProtected);
}
 

char* cWM3000SCPIFace::mGetRange()
{
    QString dedicatedChannel = m_pCmdInterpreter->dedicatedList.first();
    QString s;
    
    int n = mMeasChannelList.findIndex( dedicatedChannel);
    
    switch (n)
    {
    case 0: s = m_ConfDataActual.m_sRangeN; break;
    case 1: s = m_ConfDataActual.m_sRangeX; break;
    case 2: s = m_ConfDataActual.m_sRangeEVT; break;
    default: break;
    }
    
    return sAlloc(s);
}
    

char* cWM3000SCPIFace::mOutChannelCatalog()
{
    int i;
    QString s = "";
    for (i = 0; i < mMeasChannelList.size()-1; i++)
	s = s+ mMeasChannelList[i] + ";";
    s = s+ mMeasChannelList[i];
    return sAlloc(s);
}

// implementiertes measure model 
char* cWM3000SCPIFace::mMeasurementRead()
{
    m_pSMachineTimer->start(0, ReadStart);
    return 0;
}


char* cWM3000SCPIFace::mMeasurementReadLoadpoint()
{
    m_pSMachineTimer->start(0, ReadLPStart);
    return 0;
}


void cWM3000SCPIFace::mMeasurementConfigure(char*)
{
    m_pSMachineTimer->start(0, ConfigStart);
}


char* cWM3000SCPIFace::mMeasurementFetch()
{
    m_pSMachineTimer->start(0, FetchStart);
    return 0;
}


void cWM3000SCPIFace::mMeasurementInitiate(char*)
{
        m_pSMachineTimer->start(0, InitiateStart); // wir gehen auf die statemachine weil wir synch. müssen
}


char* cWM3000SCPIFace::mMeasurement()
{ 	
    m_pSMachineTimer->start(0, MeasStart); // wir gehen auf die statemachine weil wir synch. müssen
    return 0;
}

     
// implementiertes configuration model 
/*
char* cWM3000SCPIFace::mGetConfLogFileSize()
{
    return "bla";
}


void cWM3000SCPIFace::mSetConfLogFileSize(char*)
{
}
*/


void cWM3000SCPIFace::mConfigurationApply(char*)
{
    // fehler fällt erst hier auf -> status byte setzen

    if ((m_ConfDataTarget.m_nSRate == S256) && (m_ConfDataTarget.m_nMeasPeriod > nmaxS256MeasPeriod)) // 256 samples/periode
        m_bAddEventError = true; // we add the error later to ensure the whole command string is executed
    if ((m_ConfDataTarget.m_nSRate == S80) && (m_ConfDataTarget.m_nMeasPeriod > nmaxS80MeasPeriod)) // 80 samples/periode
        m_bAddEventError = true;

    emit SendConfiguration(&m_ConfDataTarget);
    SetnoOperFlag(false); // wir warten darauf daß konfigurieren fertig wird
}


char* cWM3000SCPIFace::mGetConfEnAppid()
{
    QString s = QString("%1").arg(m_ConfDataActual.m_nEthTypeHeader & 0xFFFF,0,16);
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfEnAppid(char* s)
{
    ushort us;
    if ( GetParameter(&s, us, 0, 0xFFFF, 16, true) )
    {
        m_ConfDataTarget.m_nEthTypeHeader &= 0xFFFF0000;
        m_ConfDataTarget.m_nEthTypeHeader |= us;
    }
}


char* cWM3000SCPIFace::mGetConfEnVid()
{
    QString s = QString("%1").arg(m_ConfDataActual.m_nPriorityTagged  & 0xFFF,0,16);
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfEnVid(char* s)
{
    ushort us;
    if ( GetParameter(&s, us, 0, 0xFFF, 16, true) )
    {
        m_ConfDataTarget.m_nPriorityTagged &= 0xFFFFF000;
        m_ConfDataTarget.m_nPriorityTagged |= us;
    }
}


char* cWM3000SCPIFace::mGetConfEnCfi()
{
    QString s = QString("%1").arg((m_ConfDataActual.m_nPriorityTagged >> 12) & 1);
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfEnCfi(char* s)
{ 
    ushort us;
    if ( GetParameter(&s, us, 0, 1, 10, true) )
    {
        m_ConfDataTarget.m_nPriorityTagged &= 0xFFFFEFFF;
        m_ConfDataTarget.m_nPriorityTagged |= (us << 12);
    }
}


char* cWM3000SCPIFace::mGetConfEnUPriority()
{
    QString s = QString("%1").arg((m_ConfDataActual.m_nPriorityTagged >> 13) & 7);
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfEnUPriority(char* s)
{
    ushort us;
    if ( GetParameter(&s, us, 1, 7, 10, true) )
    {
        m_ConfDataTarget.m_nPriorityTagged &= 0xFFFF1FFF;
        m_ConfDataTarget.m_nPriorityTagged |= (us << 13);
    }
}


char* cWM3000SCPIFace::mGetConfENFAsdu()
{
    QString s = QString("%1").arg(m_ConfDataActual.FirstASDU);
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfENFAsdu(char* s)
{
    ushort us;
    if ( GetParameter(&s, us, 1, 8, 10, true) )
    {
        m_ConfDataTarget.FirstASDU = us;
    }
}


char* cWM3000SCPIFace::mGetConfENLAsdu()
{
    QString s = QString("%1").arg(m_ConfDataActual.LastASDU);
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfENLAsdu(char* s)
{
    ushort us;

    if ( GetParameter(&s, us, 1, 8, 10, true) )
    {
        m_ConfDataTarget.LastASDU = us;
    }
}


char* cWM3000SCPIFace::mGetConfEnDSet()
{
    QString s = QString("%1").arg(m_ConfDataActual.DataSet);
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfEnDSet(char* s)
{
    ushort us;

    if ( GetParameter(&s, us, 1, 8, 10, true) )
    {
        m_ConfDataTarget.DataSet = us;
    }
}


char* cWM3000SCPIFace::mGetConfENMAdrWM3000()
{
    QString s;

    s = m_ConfDataActual.m_MacDestAdr.GetMacAdress();
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfENMAdrWM3000(char* s)
{
    SetMacAdress(&s, m_ConfDataTarget.m_MacDestAdr);
}


char* cWM3000SCPIFace::mGetConfENMAdrMU()
{
    QString s;

    s = m_ConfDataActual.m_MacSourceAdr.GetMacAdress();
    return sAlloc(s);
}


void cWM3000SCPIFace::mSetConfENMAdrMU(char* s)
{
    SetMacAdress(&s, m_ConfDataTarget.m_MacSourceAdr);
}


char* cWM3000SCPIFace::mGetConfRatioEvt()
{
    QString rs;

    rs = QString("%1:%2").arg(m_ConfDataActual.m_EVTPrimary).arg(m_ConfDataActual.m_EVTSecondary);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfRatioEvt(char* s)
{
    QString sprim, ssek; 
    if ( GetTransformerRatio(&s, sprim, ssek,true) )
    {
        m_ConfDataTarget.m_EVTPrimary = sprim;
        m_ConfDataTarget.m_EVTSecondary = ssek;
    }
}


char* cWM3000SCPIFace::mGetConfRatioChx()
{
    QString rs;

    rs = QString("%1:%2").arg(m_ConfDataActual.m_XPrimary).arg(m_ConfDataActual.m_XSecondary); 
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfRatioChx(char* s)
{
    QString sprim, ssek; 

    if ( GetTransformerRatio(&s, sprim, ssek,true) )
    {
        m_ConfDataTarget.m_XPrimary = sprim;
        m_ConfDataTarget.m_XSecondary = ssek;
    }
}


char* cWM3000SCPIFace::mGetConfRatioChn()
{
    QString rs;

    rs = QString("%1:%2").arg(m_ConfDataActual.m_NPrimary).arg(m_ConfDataActual.m_NSecondary);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfRatioChn(char* s)
{
    QString sprim, ssek; 

    if ( GetTransformerRatio(&s, sprim, ssek, true) )
    {
        m_ConfDataTarget.m_NPrimary = sprim;
        m_ConfDataTarget.m_NSecondary = ssek;
    }
}


char *cWM3000SCPIFace::mGetConfSyncStrong()
{
    QString rs;

    if (m_ConfDataActual.m_bStrongEthSynchronisation)
        rs = "1";
    else
        rs = "0";

    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfSyncStrong(char *s)
{
    ushort us;

    if ( GetParameter(&s, us, 0, 1, 10, true) )
    {
        m_ConfDataTarget.m_bStrongEthSynchronisation = ( us ==1 );
    }
}


char* cWM3000SCPIFace::mGetConfSyncPeriod()
{
    QString rs;

    rs = QString("%1ms").arg(m_ConfDataActual.m_nTSync);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfSyncPeriod(char* s)
{
    ushort us;

    if ( GetParameter(&s, us, 500, 2500, 10, true))
    {
        m_ConfDataTarget.m_nTSync = us;
    }
}


char* cWM3000SCPIFace::mGetConfSyncSource()
{
   QString rs;

   rs = QString("%1,%2").arg(m_ConfDataActual.m_nSyncSource).arg(SSourceName[m_ConfDataActual.m_nSyncSource]);
   return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfSyncSource(char* s)
{
    int ss;

    if ( SearchEntry(&s, SSourceName, MaxSSource, ss, true) )
    {
        m_ConfDataTarget.m_nSyncSource = ss;
    }	
}


char* cWM3000SCPIFace::mGetConfMeasTInt()
{
    QString rs;

    rs = QString("%1s").arg(m_ConfDataActual.m_nIntegrationTime);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfMeasTInt(char* s)
{
    ushort us;

    if ( GetParameter(&s, us, 1, 20, 10, true))
    {
        m_ConfDataTarget.m_nIntegrationTime = us;
    }
}


char* cWM3000SCPIFace::mGetConfMeasMPeriod()
{
    QString rs;

    rs = QString("%1").arg(m_ConfDataActual.m_nMeasPeriod);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfMeasMPeriod(char* s)
{
    ushort us;
    if (m_ConfDataTarget.m_nSRate == S256) // 256 samples/periode
    {
        if (GetParameter(&s, us, 1, nmaxS256MeasPeriod, 10, true)) // we set stb in case of error
            m_ConfDataTarget.m_nMeasPeriod = us; // but send the value with apply
    }	
    else
    {
        if (GetParameter(&s, us, 1, nmaxS80MeasPeriod, 10, true))
            m_ConfDataTarget.m_nMeasPeriod = us;
    }
}


char* cWM3000SCPIFace::mGetConfMeasSRate()
{
    QString rs;

    rs = SRatesName[m_ConfDataActual.m_nSRate];
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfMeasSRate(char* s)
{
    int src;
    
    if ( SearchEntry(&s, SRates, MaxSRate, src, true) )
    {
        m_ConfDataTarget.m_nSRate = src;
        // wenn die samplerate gesetzt wird werden die asdu's default gesetzt.
        // sie lassen sich einzeln immer noch überschreiben falls nötig
        m_ConfDataTarget.FirstASDU = 1;
        if (m_ConfDataTarget.m_nSRate == S80)
            m_ConfDataTarget.LastASDU = 1;
        if (m_ConfDataTarget.m_nSRate == S256)
            m_ConfDataTarget.LastASDU = 8;
    }
}


char* cWM3000SCPIFace::mGetConfMeasSFreq()
{
     QString rs;

     rs = QString("%1Hz").arg(FreqName[m_ConfDataActual.m_nSFreq]);
     return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfMeasSFreq(char* s)
{
    int fc;

    if ( SearchEntry(&s, SFrequency, MaxFreq, fc, true) )
    {
        m_ConfDataTarget.m_nSFreq = fc;
        m_ConfDataTarget.m_fSFreq = SFrequency[fc];
    }
}
 

char* cWM3000SCPIFace::mGetConfCompPhcTime()
{
    QString rs;

    rs = QString("%1ms").arg(m_ConfDataActual.m_fxTimeShift);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfCompPhcTime(char* s)
{
    double dtime;

    if ( GetParameter(&s, dtime, 0.0, 20.0, true) )// max. 20 ms
    {
        m_ConfDataTarget.m_fxTimeShift = dtime;
    }
}


char* cWM3000SCPIFace::mGetConfCompPhcPhase()
{
    QString rs;

    rs = QString("%1°").arg(m_ConfDataActual.m_fxPhaseShift);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfCompPhcPhase(char* s)
{
    double dphase;

    if ( GetParameter(&s, dphase, 0.0, 360.0, true) )
    {
        m_ConfDataTarget.m_fxPhaseShift = dphase;
    }
}


char* cWM3000SCPIFace::mGetConfCompOecFile()
{
    return sAlloc(m_ConfDataActual.m_sOETFile);
}


void cWM3000SCPIFace::mSetConfCompOecFile(char* s)
{
    if (SearchFile(s,true))
    {
        m_ConfDataTarget.m_sOETFile = m_pParser->GetKeyword(&s);
        m_ConfDataTarget.m_bOENewLoad = true; // 1x analysieren
    }
}


char* cWM3000SCPIFace::mGetConfCompOecOn()
{
    QString rs;
 
    if (m_ConfDataActual.m_bOECorrection)
        rs = "1";
    else
        rs = "0";
    
    return sAlloc(rs);
}


char *cWM3000SCPIFace::mGetConfCompOffskN()
{
    QString rs;

    if (m_ConfDataActual.m_bOffsetCorrectionN)
        rs = "1";
    else
        rs = "0";

    return sAlloc(rs);
}


char *cWM3000SCPIFace::mGetConfCompOffskX()
{
    QString rs;

    if (m_ConfDataActual.m_bOffsetCorrectionX)
        rs = "1";
    else
        rs = "0";

    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfCompOecOn(char* s)
{
    ushort us;

    if ( GetParameter(&s, us, 0, 1, 10, true) )	    
    {
        m_ConfDataTarget.m_bOECorrection = ( us ==1 );
    }
}


void cWM3000SCPIFace::mSetConfCompOffskN(char* s)
{
    ushort us;

    if ( GetParameter(&s, us, 0, 1, 10, true) )
    {
        m_ConfDataTarget.m_bOffsetCorrectionN = ( us ==1 );
    }
}


void cWM3000SCPIFace::mSetConfCompOffskX(char* s)
{
    ushort us;

    if ( GetParameter(&s, us, 0, 1, 10, true) )
    {
        m_ConfDataTarget.m_bOffsetCorrectionX = ( us ==1 );
    }
}


char* cWM3000SCPIFace::mGetConfOperModeCatalog()
{
    QString rs;

    rs = QString("%1,%2").arg(Un_UxAbs).arg(MModeName[Un_UxAbs]);
    if (!g_WMDevice->isConventional())
        for (int i = Un_UxAbs+1; i < maxMMode; i++)
            rs = rs + ";" + QString("%1,%2").arg(i).arg(MModeName[i]);
     
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfOperMode(char* s)
{
    int m;

    if ( SearchEntry(&s,MModeName,maxMMode,m,true) )
    {
        if (g_WMDevice->isConventional())
        {
            m_ConfDataTarget.m_nMeasMode = Un_UxAbs;
            if (m != Un_UxAbs)
                AddEventError(ParameterNotAllowed);
            return;
        }
        else
        {
            m_ConfDataTarget.m_nMeasMode = m;
            return;
        }
    }
    AddEventError(ParameterNotAllowed);
}


char* cWM3000SCPIFace::mGetConfOperMode()
{
    QString rs;

    rs = QString("%1,%2").arg(m_ConfDataActual.m_nMeasMode).arg(MModeName[m_ConfDataActual.m_nMeasMode]);
    return sAlloc(rs);
}


char *cWM3000SCPIFace::mGetConfOperSignalCatalog()
{
    QString rs;

    rs = QString("%1,%2;%3,%4").arg(AC).arg(SModeName[AC])
                               .arg(DC).arg(SModeName[DC]);
    return sAlloc(rs);
}


void cWM3000SCPIFace::mSetConfOperSignal(char *s)
{
    int m;

    if ( SearchEntry(&s,SModeName,maxSMode,m,true) )
    {
        if (g_WMDevice->isDC())
        {   // wir dürfen alle signal modi
            m_ConfDataTarget.m_bDCmeasurement = (m == DC);
            return;
        }
        else
        {
            m_ConfDataTarget.m_bDCmeasurement = false;
            if (m != AC)
               AddEventError(ParameterNotAllowed);
            return;
        }
    }
    AddEventError(ParameterNotAllowed);
}


char *cWM3000SCPIFace::mGetConfOperSignal()
{
    QString rs;

    if (m_ConfDataActual.m_bDCmeasurement)
        rs = QString("%1,%2").arg(DC).arg(SModeName[DC]);
    else
        rs = QString("%1,%2").arg(AC).arg(SModeName[AC]);
    return sAlloc(rs);
}


bool cWM3000SCPIFace::SearchEntry(char** s, char** sa, int max, int& n, bool chkEnd) // suche, worin, wieviele einträge, eintrag, test auf ende
{
    bool ok;
    QString inp =  m_pParser->GetKeyword(s); // wir holen den Input
    
    if (chkEnd)
    {
        QString par = m_pParser->GetKeyword(s);
        if ( !par. isEmpty()  )
        {
            AddEventError(InvalidSeparator); // macht agilent auch so
            return false;
        }
    }
    
    inp = inp.upper(); // grossbuchstaben
    n = inp.toInt(&ok); // erst mal testen ob es der index ist
    if (ok)
    {
        if (n < max)
            return ok;
        else
        {
            AddEventError(ParameterNotAllowed);
            return false;
        }
    }
    
    QString cs;
    int i;
    for (i = 0; i < max; i++)
    {
        cs = sa[i];
        cs = cs.upper();
        if (inp == cs)
            break;
    }
    
    if (i == max)
    {
        AddEventError(ParameterNotAllowed);
        return false;
    }
    
    n = i;
    return true;
}


bool cWM3000SCPIFace::SearchEntry(char** s, double* pdval, int max, int& n, bool chkEnd) // suche, worin, wieviele einträge, eintrag, test auf ende
{
    bool ok;
    QString inp =  m_pParser->GetKeyword(s); // wir holen den Input
    
    if (chkEnd)
    {
        QString par = m_pParser->GetKeyword(s);
        if ( !par. isEmpty()  )
        {
            AddEventError(InvalidSeparator); // macht agilent auch so
            return false;
        }
    }
    
    double d;
    d = inp.toDouble(&ok); // erst mal testen ob es der index ist
    
    if ( !ok ) 
    {
        AddEventError(NumericDataError);
        return ok;
    }
    
    int i;
    for ( i = 0; i < max; i++, pdval++)
	if (fabs(d - *pdval) < 1e-4)
	    break;
    
    if (i == max) 
    {
        AddEventError(ParameterNotAllowed); // wir haben den wert nicht gefunden
        return false;
    }
    
    n = i;
    return true;
}


bool cWM3000SCPIFace::SearchEntry(char** s, int* pival, int max, int& n, bool chkEnd) // suche, worin, wieviele einträge, eintrag, test auf ende
{
    bool ok;
    QString inp =  m_pParser->GetKeyword(s); // wir holen den Input
    
    if (chkEnd)
    {
        QString par = m_pParser->GetKeyword(s);
        if ( !par. isEmpty()  )
        {
            AddEventError(InvalidSeparator); // macht agilent auch so
            return false;
        }
    }
    
    int id;
    id = inp.toInt(&ok); // erst mal testen ob es der index ist
    
    if ( !ok ) 
    {
        AddEventError(NumericDataError);
        return ok;
    }
    
    int i;
    for ( i = 0; i < max; i++, pival++)
	if (id == *pival)
	    break;
    
    if (i == max) 
    {
        AddEventError(ParameterNotAllowed); // wir haben den wert nicht gefunden
        return false;
    }
    
    n = i;
    return true;
}


bool cWM3000SCPIFace::SearchFile(char* s, bool chkEnd)
{
    QString inp =  m_pParser->GetKeyword(&s); // wir holen den Input
    if (inp.isEmpty()) // wenn nicht input -> file = "" -> close file
	return true;
    if (chkEnd)
    {
        QString par = m_pParser->GetKeyword(&s);
        if ( !par. isEmpty()  )
        {
            AddEventError(InvalidSeparator); // macht agilent auch so
            return false;
        }
    }
    
    QFile f(inp);
    bool e = f.exists();
    if (!e)
        AddEventError(FileNameError);
    
    return e;
}


bool cWM3000SCPIFace::GetParameter(char** s, ushort& us, bool chkEnd) // zeiger auf input,  der gefundene wert
{
    bool ok; 
    QString par = m_pParser->GetKeyword(s); // holt den parameter aus dem kommando
    ushort nPar = par.toUShort(&ok);
    if ( ok)  
    {
        us = nPar;
        if (chkEnd)
        {
            par = m_pParser->GetKeyword(s);
            if ( par.isEmpty() )
            return ok;
            else
            {
                AddEventError(InvalidSeparator); // macht agilent auch so
                return false;
            }
        }
        else
            return ok;
    }
    else 
        AddEventError(NumericDataError);

    return ok;
}   


bool cWM3000SCPIFace::GetParameter(char** s, ushort& us, int min, int max, int base, bool chkEnd) // zeiger auf input,  der gefundene wert
{
    bool ok; 
    QString par = m_pParser->GetKeyword(s); // holt den parameter aus dem kommando
    ushort nPar = par.toUShort(&ok,base);
    if ( ok)  
    {
        us = nPar;
        if (chkEnd)
        {
            par = m_pParser->GetKeyword(s);
            if ( ! par.isEmpty() )
            {
                AddEventError(InvalidSeparator); // macht agilent auch so
                return false;
            }
        }

        if ( (nPar < min) || (nPar >max) )
        {
            AddEventError(ParameterNotAllowed);
            return false;
        }

        return ok;
    }
    else 
        AddEventError(NumericDataError);

    return ok;
}   

bool cWM3000SCPIFace::GetParameter(char** s, double& d, double min, double max, bool chkEnd) // zeiger auf input,  der gefundene wert
{
    bool ok; 
    QString par = m_pParser->GetKeyword(s); // holt den parameter aus dem kommando
    double Par = par.toDouble(&ok);
    if ( ok)  
    {
        if (chkEnd)
        {
            par = m_pParser->GetKeyword(s);
            if ( ! par.isEmpty() )
            {
                AddEventError(InvalidSeparator);
                return false;
            }
        }

        if ( (Par < min) || (Par > max) )
        {
            AddEventError(ParameterNotAllowed);
            return false;
        }

        d = Par;
        return ok;
    }
    else 
	AddEventError(NumericDataError);
    
    return ok;
}   


bool cWM3000SCPIFace::GetParameter(char** s, QString& par,bool chkEnd) // zeiger auf input, der gefundene string, test auf blank?
{
    par = m_pParser->GetKeyword(s); // holt den parameter aus dem kommando
    if (chkEnd) 
    {
        if (!(m_pParser->GetKeyword(s)).isEmpty())
        {
            AddEventError(InvalidSeparator); // macht agilent auch so
            return false;
        }
    }
    
    return true;
}


bool cWM3000SCPIFace::GetTransformerRatio(char** s, QString& ps, QString& ss, bool chkEnd)
{
    bool ok = true ; 
    int i;
    QString par[2];
    eParameter ep[2];
    
    QString ws = m_pParser->SetWhiteSpace(" :"); // wir definieren whitespace um
    
    for ( i = 0; i < 2; i++)
	par[i] = m_pParser->GetKeyword(s); //  2  parameter lesen
    
    m_pParser->SetWhiteSpace(ws); // jetzt wieder zurück
    
    if (chkEnd && !(m_pParser->GetKeyword(s)).isEmpty() )
    {
        AddEventError(InvalidSeparator);
        return false;
    }	
	
    if ( par[0].isEmpty() || par[1].isEmpty() )
    {
        AddEventError(MissingParameter);
        return false;
    }
    
    for ( i = 0; i < 2; i++)
    {
        ep[i] = par[i];
        if ( !( ep[i].isVoltage() || ep[i].withoutUnit() ))
            ok = false;
    }
    
    if ( !ok)
    {
        AddEventError(ParameterNotAllowed);
        return false;
    }
    
    ps = par[0];
    ss = par[1];
    
    return true;
}


void cWM3000SCPIFace::SetMacAdress(char** s, cETHAdress& leth)
{
    QString par = m_pParser->GetKeyword(s);
    if (!m_pParser->GetKeyword(s).isEmpty() )
    {
        AddEventError(InvalidSeparator);
        return;
    }
    
    if (par.isEmpty())
    {
        AddEventError(MissingParameter);
        return;
    }
    
    if (!leth.SetMacAdress(par))
        AddEventError(ParameterNotAllowed);
}


void cWM3000SCPIFace::ExecuteCommand(int entryState) // ausführen eines common kommandos
{
    static int EXS = ExecCmdIdle; // execution state
    static int CmdPart;
    static QString CmdInProgress;
    QString s;
    
    if ( entryState == ExecCmdStart )
    { // wir bekommen neues kommando
        if (EXS != ExecCmdIdle)
        { // sind aber noch nicht fertig
            EXSFifo.push_back(m_sCommand);  // wir merken uns das kommondo wir was starten sollten

            return; // und sind fertig
        }
        else
        {// oder wir sind fertig und
            CmdInProgress = m_sCommand;
            EXS = entryState; // wir starten es
        }
    }

    else

    if (entryState != 	ExecCmdContinue)
        EXS = entryState; // wenns dann nicht continue ist gehen wir  dahin, wo die bearbeiten routine uns hinhaben will
        
    switch (EXS)    
    {
	
    case ExecCmdStart:
    CmdPart = 0;
    m_sAnswer = "";
    m_bCmdError = false;
    EXS++; // wir gehen direkt zum nächsten state

    case ExecCmdPart:
    {
        QString cmd;
        cmd = CmdInProgress.section(';',CmdPart,CmdPart); // ein teil kommando ... falls es mehrere sind
        if ( cmd.length() > 0 ) // wir haben ein kommando
        {
            m_pCmdTimer->start(0, cmd);
        }
        else
            m_pSMachineTimer->start(0, ExecCmdFinished); // kommando fertig

        break;
    }

    case ExecCmdPartFinished:
    CmdPart++; // nächstes teilkommando

    if (answ) { // wenn wir eine antwort des teilkommandos haben, kleben wir sie an die gesamt antwort
        if (m_sAnswer.length() > 0)
        m_sAnswer += ";";
        m_sAnswer += answ;
        free(answ);
    }

    if ( m_bCmdError ) // ist ein fehler aufgetreten ?
        m_pSMachineTimer->start(0, ExecCmdFinished); // dann ->kommando fertig
    else
    m_pSMachineTimer->start(0, ExecCmdPart); // wir bearbeiten das kommando weiter

    break;

    case ExecCmdFinished:
    {
        if (m_bAddEventError)
        {
            m_bAddEventError = false;
            AddEventError(ParameterNotAllowed);
        }
        if (m_sAnswer.length() > 0)
        emit SendAnswer(m_sAnswer);

        EXS = ExecCmdIdle;
        break;
    }
	
    case EN61850SynclostCountStart:
    case EN61850ErrorStart:
    case EN61850DataCountStart:
	emit EN61850StatusRequest(); // für jedes der 3 kommandos erst mal aktuelle info anfordern
	m_nWait4What = wait4ENStatus;
	EXS++;
	break;
	
	
    case EN61850SynclostCountFinished:
	s = QString("%1").arg(m_EN61850Info.SyncLostCount);
	answ = sAlloc(s);
	m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
	break;
	
    case EN61850ErrorFinished:
	s = QString("%1").arg(m_EN61850Info.ETHErrors);
	answ = sAlloc(s);
	m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
	break;
	
    case EN61850DataCountFinished:
	{
	    double n;
	    n = m_EN61850Info.ByteCount[0] * 4294967296.0 + m_EN61850Info.ByteCount[1];
	    s = QString("%1").arg(n);
	    answ = sAlloc(s);
	    m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
	    break;
	}
	
    case MeasStart:
    case ConfigStart:
	if ( isAuthorized())
	    EXS++; // wenn wir dürfen gehts direkt weiter
	else
	{
	    AddEventError(CommandProtected);
	    m_pSMachineTimer->start(0, ExecCmdFinished); // bei fehlern sind wir direkt fertig
	}
    
    case MeasConfiguration:
    case ConfigConfiguration:
	// erst mal defaults setzen
	emit SetDefaultMeasConfig(); // die messeinrichtung stellt defaults ein
    
	m_ConfDataTarget.m_sRangeNVorgabe = "Auto";
	m_ConfDataTarget.m_sRangeXVorgabe = "Auto";
	m_ConfDataTarget.m_sRangeEVTVorgabe = "Auto";
    
	emit SendRange(&m_ConfDataTarget); // bereich automatik einstellen
	m_nWait4What = wait4RangeAutomatic; 
	EXS++;
	break;
	
    case MeasInitiate:
    case InitiateStart:
    case ReadStart:	
	EXS++;
	m_nWait4What = wait4MeasurementData;
	break;
	
    case MeasFetch:
    case FetchStart:	
    {
        double ampln, amplx;

        if (m_ConfDataActual.m_bDCmeasurement)
        {
            ampln = mActValues.VekN.re();
            amplx = mActValues.VekX.re();
        }
        else
        {
            ampln = fabs(mActValues.VekN);
            amplx = fabs(mActValues.VekX);
        }
        s = QString("%1;%2;%3;%4;%5;%6;%7;%8;%9")
            .arg(mActValues.Frequenz)
            .arg(ampln)
            .arg(mActValues.PHIN)
            .arg(amplx)
            .arg(mActValues.PHIX)
            .arg(mActValues.LoadPointX)
            .arg(mActValues.LoadPoint1X)
            .arg(mActValues.AmplErrorIEC)
            .arg(mActValues.AmplErrorANSI);
        s += QString(";%1;%2").arg(mActValues.AngleError).arg(mActValues.RCF);
        answ = sAlloc(s);
        m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
        break;
    }
	
    case ReadLPStart:
	EXS++;
	m_nWait4What = wait4LoadpointData;
	break;
	
    case ReadLPRead:	
    s = QString("%1,%2").arg(mActValues.LoadPointX).arg(mActValues.LoadPoint1X);
	answ = sAlloc(s);
	m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
	break;	 
	
    case ConfigFinished:	
    case InitiateFinished:	
	m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
	break;	
	
    case ReadRead:
	m_pSMachineTimer->start(0, FetchStart);
	break;
	
    case ifSelftestStart:
	if ( !isAuthorized())
	{
	    AddEventError(CommandProtected); // wir dürfen nicht !!!
	    m_pSMachineTimer->start(0, ExecCmdFinished); // bei fehlern sind wir direkt fertig
	}
	else
	{
	    emit SelftestRequest(); // selbsttest anstossen
	    m_nWait4What =  wait4SelftestResult;
	    EXS++;
	}
	
	break;
	
    case ifSelftestFinished:
    s = QString("+%1").arg(SelftestResult);
	answ = sAlloc(s);
	m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
	break;

    case ifChannelNOffsetStart:
    if ( !isAuthorized())
    {
        AddEventError(CommandProtected); // wir dürfen nicht !!!
        m_pSMachineTimer->start(0, ExecCmdFinished); // bei fehlern sind wir direkt fertig
    }
    else
    {
        emit ChannelNOffsetMeasureRequest(); // selbsttest anstossen
        m_nWait4What =  wait4Offsetresult;
        EXS++;
    }

    break;
	
    case ifChannelNOffsetFinished:
    s = QString("%1").arg(OffsetResult); // wir benutzen das selftest resultat auch für hier
    answ = sAlloc(s);
    m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
    break;

    case ifChannelXOffsetStart:
    if ( !isAuthorized())
    {
        AddEventError(CommandProtected); // wir dürfen nicht !!!
        m_pSMachineTimer->start(0, ExecCmdFinished); // bei fehlern sind wir direkt fertig
    }
    else
    {
        emit ChannelXOffsetMeasureRequest(); // selbsttest anstossen
        m_nWait4What =  wait4Offsetresult;
        EXS++;
    }

    break;

    case ifChannelXOffsetFinished:
    s = QString("%1").arg(OffsetResult); // wir benutzen das selftest resultat auch für hier
    answ = sAlloc(s);
    m_pSMachineTimer->start(0, ExecCmdPartFinished); // teil kommando fertig
    break;

    }
    
		
    if (EXS == ExecCmdIdle)  // wenn wir fertig sind
	if ( !EXSFifo.empty() )  // und das kommando fifo nicht leer ist
	{
	QString cmd;
	cmd = EXSFifo.first();
	EXSFifo.pop_front();
	ReceiveCommand(cmd);
    }	
}


void cWM3000SCPIFace::SCPICmd( int cmd,char* s) {
    switch (cmd)
    {
	case MeasurementInitiate: mMeasurementInitiate(s);break;
              if ( isAuthorized() )
	    {
		  case ResetCmd: DeviceReset(s);break; // *RST
		  case OperationCompleteCmd: SetDeviceOPC(s);break; // *OPC
		  case ClearStatusCmd: DeviceClearStatus(s); break; // *CLS
		  case StdEventStatEnableCmd: SetDeviceESE(s); break; // *ESE
		  case ServiceRequestEnableCmd: SetDeviceSRE(s); break; // *SRE	
		      
		  case SetStatusEN61850Clear: mSetStatusEN61850Clear(s);break;
		  case SetStatusQuestionablePTrans: mSetStatusQuestionablePTrans(s);break;
		  case SetStatusQuestionableNTrans: mSetStatusQuestionableNTrans(s);break;
		  case SetStatusQuestionableEnable: mSetStatusQuestionableEnable(s);break;
		  case SetStatusQuestionableCondition: mSetStatusQuestionableCondition(s);break;
		  case SetStatusOperationPTrans: mSetStatusOperationPTrans(s);break;
		  case SetStatusOperationNTrans: mSetStatusOperationNTrans(s);break;
		  case SetStatusOperationEnable: mSetStatusOperationEnable(s);break;
		  case SetStatusOperationCondition: mSetStatusOperationCondition(s);break;
		  case SetRange: mSetRange(s);break;
		  case MeasurementConfigure: mMeasurementConfigure(s);break;
		  case ConfApply: mConfigurationApply(s);break;    
//		  case SetConfLogFileSize: mSetConfLogFileSize(s);break;
		  case SetConfEnAppid: mSetConfEnAppid(s);break;
		  case SetConfEnVid: mSetConfEnVid(s);break;
		  case SetConfEnCfi: mSetConfEnCfi(s);break;
		  case SetConfEnUPriority: mSetConfEnUPriority(s);break;
          case SetConfENFAsdu: mSetConfENFAsdu(s);break;
          case SetConfENLAsdu: mSetConfENLAsdu(s);break;
		  case SetConfEnDSet: mSetConfEnDSet(s);break;
		  case SetConfENMAdrWM3000: mSetConfENMAdrWM3000(s);break;
		  case SetConfENMAdrMU: mSetConfENMAdrMU(s);break;
		  case SetConfRatioEvt: mSetConfRatioEvt(s);break;
		  case SetConfRatioChx: mSetConfRatioChx(s);break;
		  case SetConfRatioChn: mSetConfRatioChn(s);break;
          case SetConfSyncStrong: mSetConfSyncStrong(s);break;
		  case SetConfSyncPeriod: mSetConfSyncPeriod(s);break;
		  case SetConfSyncSource: mSetConfSyncSource(s);break;
		  case SetConfMeasTInt: mSetConfMeasTInt(s);break;
		  case SetConfMeasMPeriod: mSetConfMeasMPeriod(s);break;
		  case SetConfMeasSRate: mSetConfMeasSRate(s);break;
		  case SetConfMeasSFreq: mSetConfMeasSFreq(s);break;
		  case SetConfCompPhcTime: mSetConfCompPhcTime(s);break;
		  case SetConfCompPhcPhase: mSetConfCompPhcPhase(s);break;
		  case SetConfCompOecFile: mSetConfCompOecFile(s);break;
		  case SetConfCompOecOn: mSetConfCompOecOn(s);break;
          case SetConfCompOffskN: mSetConfCompOffskN(s);break;
          case SetConfCompOffskX: mSetConfCompOffskX(s);break;
		  case SetConfOperMode: mSetConfOperMode(s);break;
          case SetConfOperSignal: mSetConfOperSignal(s);break;
		  default: qDebug("ProgrammierFehler"); // hier sollten wir nie hinkommen
		  }
	      else
	      {
              AddEventError(CommandProtected);
              m_pSMachineTimer->start(0, ExecCmdFinished);
	      }
	  }
	
	switch (cmd) // hier alle kommandos die keine synchr. benötigen und deshalb sofort in die statemachine zurückkehren 
	{
	case ResetCmd:
	case OperationCompleteCmd:
	case ClearStatusCmd: 
	case StdEventStatEnableCmd:
	case ServiceRequestEnableCmd:
	    
	case SetStatusEN61850Clear: 
	case SetStatusQuestionablePTrans: 
	case SetStatusQuestionableNTrans: 
	case SetStatusQuestionableEnable: 
	case SetStatusQuestionableCondition: 
	case SetStatusOperationPTrans: 
	case SetStatusOperationNTrans: 
	case SetStatusOperationEnable: 
	case SetStatusOperationCondition: 
	case SetRange: 
	case ConfApply:    
//	case SetConfLogFileSize: 
	case SetConfEnAppid: 
	case SetConfEnVid: 
	case SetConfEnCfi: 
	case SetConfEnUPriority: 
    case SetConfENLAsdu:
    case SetConfENFAsdu:
	case SetConfEnDSet: 
	case SetConfENMAdrWM3000: 
	case SetConfENMAdrMU: 
	case SetConfRatioEvt: 
	case SetConfRatioChx:
	case SetConfRatioChn: 
    case SetConfSyncStrong:
	case SetConfSyncPeriod: 
	case SetConfSyncSource:
	case SetConfMeasTInt: 
	case SetConfMeasMPeriod:
	case SetConfMeasSRate: 
	case SetConfMeasSFreq: 
	case SetConfCompPhcTime:
	case SetConfCompPhcPhase:
	case SetConfCompOecFile: 
	case SetConfCompOecOn: 
    case SetConfCompOffskN:
    case SetConfCompOffskX:
	case SetConfOperMode: 
    case SetConfOperSignal:
	    m_pSMachineTimer->start(0, ExecCmdPartFinished);
	default: 
	    break;
	}
}


char* cWM3000SCPIFace::SCPIQuery( int cmd, char* s) {
    QString keyw = m_pParser->GetKeyword(&s);
    if ( keyw.isEmpty() ) // hier darf nichts stehen 
    {
	char * an;
	switch (cmd)
	{
	case IdentQuery: an = GetDeviceIdentification();break; // *IDN?
	case SelfTestQuery: an = DeviceSelfTest();break; // *TST?
	case OperationCompleteQuery: an = GetDeviceOPC();break; // *OPC?
	case StdEventStatEnableQuery: an = GetDeviceESE();break; // *ESE?
	case StdEventStatRegQuery: an = GetDeviceESR();break; // *ESR?
	case ServiceRequestEnableQuery: an = GetDeviceSRE();break; // *SRE?
	case StatusByteQuery: an = GetDeviceSTB();break; // *STB?
	    
    case GetChannelNOffsetCmd: an = GetChannelNOffset();break;
    case GetChannelXOffsetCmd: an = GetChannelXOffset();break;

	case GetScpiErrorAll: an = mGetScpiErrorAll();break;
	case GetScpiErrorCount: an = mGetScpiErrorCount();break;
	case GetScpiError: an = mGetScpiError();break;
	case GetSerialNumber: an = mGetSerialNumber();break;
	case GetDSPVersion: an = mGetDSPVersion();break;
	case GetPCBVersion: an = mGetPCBVersion();break;
	case GetDeviceVersion: an = mGetDeviceVersion();break;
	case GetStatusEN61850SynclostCount: an = mGetStatusEN61850SynclostCount();break;
	case GetStatusEN61850Error: an =mGetStatusEN61850Error();break;
	case GetStatusEN61850DataCount: an = mGetStatusEN61850DataCount();break;
	case GetStatusQuestionablePTrans: an = mGetStatusQuestionablePTrans();break;
	case GetStatusQuestionableNTrans: an = mGetStatusQuestionableNTrans();break;    
	case GetStatusQuestionableEnable: an = mGetStatusQuestionableEnable();break;
	case GetStatusQuestionableCondition: an = mGetStatusQuestionableCondition();break;
	case GetStatusQuestionableEvent: an = mGetStatusQuestionableEvent();break;
	case GetStatusOperationPTrans: an = mGetStatusOperationPTrans();break;
	case GetStatusOperationNTrans: an = mGetStatusOperationNTrans();break;
	case GetStatusOperationEnable: an = mGetStatusOperationEnable();break;
	case GetStatusOperationCondition: an = mGetStatusOperationCondition();break;
	case GetStatusOperationEvent: an = mGetStatusOperationEvent();break;
	case GetStatusStandard: an = mGetStatusStandard();break;
	case OutRangeCatalog: an = mOutRangeCatalog();break;
	case GetRange: an = mGetRange();break;
	case OutChannelCatalog: an = mOutChannelCatalog();break;
	case MeasurementRead: an = mMeasurementRead();break;
	case MeasurementReadLoadpoint: an = mMeasurementReadLoadpoint();break;
	case MeasurementFetch: an = mMeasurementFetch();break;	         
	case Measurement: an = mMeasurement();break;
//	case GetConfLogFileSize: an = mGetConfLogFileSize();break;
	case GetConfEnAppid: an = mGetConfEnAppid();break;
	case GetConfEnVid: an = mGetConfEnVid();break;
	case GetConfEnCfi: an = mGetConfEnCfi();break;
	case GetConfEnUPriority: an = mGetConfEnUPriority();break;
    case GetConfENFAsdu: an = mGetConfENFAsdu();break;
    case GetConfENLAsdu: an = mGetConfENLAsdu();break;
	case GetConfEnDSet: an = mGetConfEnDSet();break;
	case GetConfENMAdrWM3000: an = mGetConfENMAdrWM3000();break;
	case GetConfENMAdrMU: an = mGetConfENMAdrMU();break;
	case GetConfRatioEvt: an = mGetConfRatioEvt();break;
	case GetConfRatioChx: an = mGetConfRatioChx();break;
	case GetConfRatioChn: an = mGetConfRatioChn();break;
    case GetConfSyncStrong: an = mGetConfSyncStrong();break;
	case GetConfSyncPeriod: an = mGetConfSyncPeriod();break;
	case GetConfSyncSource: an = mGetConfSyncSource();break;
    case GetConfMeasTInt: an = mGetConfMeasTInt();break;
	case GetConfMeasMPeriod: an = mGetConfMeasMPeriod();break;
	case GetConfMeasSRate: an = mGetConfMeasSRate();break;
	case GetConfMeasSFreq: an = mGetConfMeasSFreq();break;
	case GetConfCompPhcTime: an = mGetConfCompPhcTime();break;
	case GetConfCompPhcPhase: an = mGetConfCompPhcPhase();break;
	case GetConfCompOecFile: an = mGetConfCompOecFile();break;
	case GetConfCompOecOn: an = mGetConfCompOecOn();break;
    case GetConfCompOffskN: an = mGetConfCompOffskN();break;
    case GetConfCompOffskX: an = mGetConfCompOffskX();break;
	case GetConfOperModeCatalog: an = mGetConfOperModeCatalog();break;
	case GetConfOperMode: an = mGetConfOperMode();break;
    case GetConfOperSignalCatalog: an = mGetConfOperSignalCatalog();break;
    case GetConfOperSignal: an = mGetConfOperSignal();break;

	default:	qDebug("ProgrammierFehler"); // hier sollten wir nie hinkommen     
	}
	
	switch (cmd)
	{
	case IdentQuery:
	case OperationCompleteQuery:
	case StdEventStatEnableQuery:
	case StdEventStatRegQuery:
	case ServiceRequestEnableQuery:
	case StatusByteQuery:
	    
	case GetScpiErrorAll:
	case GetScpiErrorCount:
	case GetScpiError:
	case GetSerialNumber:
	case GetDSPVersion:
	case GetPCBVersion:
	case GetDeviceVersion:
	
	case GetStatusQuestionablePTrans:
	case GetStatusQuestionableNTrans:
	case GetStatusQuestionableEnable:
	case GetStatusQuestionableCondition:
	case GetStatusQuestionableEvent:
	case GetStatusOperationPTrans:
	case GetStatusOperationNTrans:
	case GetStatusOperationEnable:
	case GetStatusOperationCondition:
	case GetStatusOperationEvent:
	case GetStatusStandard:
	case OutRangeCatalog:
	case GetRange:
	case OutChannelCatalog:
//	case GetConfLogFileSize:
	case GetConfEnAppid:
	case GetConfEnVid:
	case GetConfEnCfi:
	case GetConfEnUPriority:
    case GetConfENFAsdu:
    case GetConfENLAsdu:
	case GetConfEnDSet:
	case GetConfENMAdrWM3000:
	case GetConfENMAdrMU:
	case GetConfRatioEvt:
	case GetConfRatioChx:
	case GetConfRatioChn:
    case GetConfSyncStrong:
	case GetConfSyncPeriod:
	case GetConfSyncSource:
    case GetConfMeasTInt:
	case GetConfMeasMPeriod:
	case GetConfMeasSRate:
	case GetConfMeasSFreq:
	case GetConfCompPhcTime:
	case GetConfCompPhcPhase:
	case GetConfCompOecFile:
	case GetConfCompOecOn:
    case GetConfCompOffskN:
    case GetConfCompOffskX:
	case GetConfOperModeCatalog:
	case GetConfOperMode:
    case GetConfOperSignalCatalog:
    case GetConfOperSignal:
	    m_pSMachineTimer->start(0, ExecCmdPartFinished);	    
	default:	
	    break;
	}
	
	
	return an;
    }
    else
    {
	AddEventError(ParameterNotAllowed);
	m_pSMachineTimer->start(0, ExecCmdFinished);
    }	
    
    return 0;
}


// die vollständige scpi kommando liste

cNodeSCPI* Configuration;
    cNodeSCPI* ConfigurationOperation;
        cNodeSCPI* ConfigurationOperationMode;
            cNodeSCPI* ConfigurationOperationModeCatalog;
        cNodeSCPI* ConfigurationOperationSignal;
            cNodeSCPI* ConfigurationOperationSignalCatalog;
    cNodeSCPI* ConfigurationComputation;
        cNodeSCPI* ConfigurationComputationMode;
            cNodeSCPI* ConfigurationComputationModeCatalog;
        cNodeSCPI* ConfigurationComputationOECorrection;
            cNodeSCPI* ConfigurationComputationOECorrectionOn;
        cNodeSCPI* ConfigurationComputationXOffset;
        cNodeSCPI* ConfigurationComputationNOffset;
        cNodeSCPI* ConfigurationComputationOECorrectionFile;
        cNodeSCPI* ConfigurationComputationPHCorrection;
            cNodeSCPI* ConfigurationComputationPHCorrectionPhase;
            cNodeSCPI* ConfigurationComputationPHCorrectionTime;
    cNodeSCPI* ConfigurationMeasure;
        cNodeSCPI* ConfigurationMeasureSigFrequency;
        cNodeSCPI* ConfigurationMeasureSRate;
        cNodeSCPI* ConfigurationMeasureMPeriod;
        cNodeSCPI* ConfigurationMeasureIntegrationTime;
    cNodeSCPI* ConfigurationSynchronization;
        cNodeSCPI* ConfigurationSynchronizationSource;
        cNodeSCPI* ConfigurationSynchronizationPeriod;
        cNodeSCPI* ConfigurationSynchronizationStrong;
    cNodeSCPI* ConfigurationRatio;
        cNodeSCPI* ConfigurationRatioN;
        cNodeSCPI* ConfigurationRatioX;
        cNodeSCPI* ConfigurationRatioEVT;
    cNodeSCPI* ConfigurationEN61850; 					   		                                                      cNodeSCPI* ConfigurationEN61850MacAdress;
        cNodeSCPI* ConfigurationEN61850MacAdressMergingUnit;
        cNodeSCPI* ConfigurationEN61850MacAdressWM3000;
        cNodeSCPI* ConfigurationEN61850DataSet;
        cNodeSCPI* ConfigurationEN61850LAsdu;
        cNodeSCPI* ConfigurationEN61850FAsdu;
        cNodeSCPI* ConfigurationEN61850UserPriority;
        cNodeSCPI* ConfigurationEN61850Cfi;
        cNodeSCPI* ConfigurationEN61850Vid;
        cNodeSCPI* ConfigurationEN61850Appid;
			    			    
//	      cNodeSCPI* ConfigurationLogFile;				  
//			    cNodeSCPI* ConfigurationLogFileSize;	
			 
        cNodeSCPI* ConfigurationApply;
			    
		    
cNodeSCPI* Measure;
cNodeSCPI* Initiate;
cNodeSCPI* Fetch;
cNodeSCPI* Read;
cNodeSCPI* ReadLoadpoint;
	      
	     
cNodeSCPI* Sense;
    cNodeSCPI* SenseChannel;
        cNodeSCPI* SenseChannelCatalog;
    cNodeSCPIVar* SenseCName;
        cNodeSCPI* SenseCNameRange;
            cNodeSCPI* SenseCNameRangeCatalog;
	
cNodeSCPI* Store;
    cNodeSCPI* StoreChannelNOffset;
    cNodeSCPI* StoreChannelXOffset;

				       
cNodeSCPI* Status;
    cNodeSCPI* StatusStandard;
    cNodeSCPI* StatusOperation;
        cNodeSCPI* StatusOperationEvent;
        cNodeSCPI* StatusOperationCondition;
        cNodeSCPI* StatusOperationEnable;
        cNodeSCPI* StatusOperationNTransition;
        cNodeSCPI* StatusOperationPTransition;
    cNodeSCPI* StatusQuestionable;
        cNodeSCPI* StatusQuestionableEvent;
        cNodeSCPI* StatusQuestionableCondition;
        cNodeSCPI* StatusQuestionableEnable;
        cNodeSCPI* StatusQuestionableNTransition;
        cNodeSCPI* StatusQuestionablePTransition;
			  
        cNodeSCPI* StatusEN61850;
            cNodeSCPI* StatusEN61850DataCount;
            cNodeSCPI* StatusEN61850Error;
            cNodeSCPI* StatusEN61850SynclostCount;
            cNodeSCPI* StatusEN61850Clear;

			  
cNodeSCPI* System;
    cNodeSCPI* SystemVersion;
        cNodeSCPI* SystemVersionDevice;
        cNodeSCPI* SystemVersionPCB;
        cNodeSCPI* SystemVersionDSP;
    cNodeSCPI* SystemSerial;
    cNodeSCPI* SystemError;
        cNodeSCPI* SystemErrorCount;
        cNodeSCPI* SystemErrorAll;
		          		   
// cNodeScpi (QString,tNodeSpec,cNode*,cNode*,SCPICmdType,SCPICmdType); 
// konstruktor, sNodeName, nNodedef, pNextNode, pNewLevelNode, Cmd, Query				
// konstruktor, psNodeNames,psNode2Set, nNodedef, pNextNode, pNewLevelNode, Cmd, Query
//cNodeZHServer::cNodeZHServer(QStringList* sl,QString* s,tNodeSpec ns,cNode* n1,cNode* n2,SCPICmdType,SCPICmdType)
					
					
cNode* cWM3000SCPIFace::InitScpiCmdTree(cNode* cn) {
    
    // implementiertes system modell
    
    SystemErrorAll=new cNodeSCPI("ALL",isQuery,NULL,NULL,nixCmd,GetScpiErrorAll);
    SystemErrorCount=new cNodeSCPI("COUNT",isQuery,SystemErrorAll,NULL,nixCmd,GetScpiErrorCount);
    SystemError=new cNodeSCPI("ERROR",isNode | isQuery,NULL,SystemErrorCount,nixCmd,GetScpiError);
    SystemSerial=new cNodeSCPI("SERIAL",isQuery,SystemError,NULL,nixCmd,GetSerialNumber);
    SystemVersionDSP=new cNodeSCPI("DSP",isQuery,NULL,NULL,nixCmd,GetDSPVersion);
    SystemVersionPCB=new cNodeSCPI("PCB",isQuery,SystemVersionDSP,NULL,nixCmd,GetPCBVersion);
    SystemVersionDevice=new cNodeSCPI("DEVICE",isQuery,SystemVersionPCB,NULL,nixCmd,GetDeviceVersion);
    SystemVersion=new cNodeSCPI("VERSION",isNode,SystemSerial,SystemVersionDevice,nixCmd,nixCmd);
    System=new cNodeSCPI("SYSTEM",isNode,cn,SystemVersion,nixCmd,nixCmd);	       
    // implementiertes status modell
    
    StatusEN61850Clear=new cNodeSCPI("CLEAR",isCommand,NULL,NULL,SetStatusEN61850Clear,nixCmd);			  			  
    StatusEN61850SynclostCount=new cNodeSCPI("SLCOUNT",isQuery,StatusEN61850Clear,NULL,nixCmd,GetStatusEN61850SynclostCount);			  
    StatusEN61850Error=new cNodeSCPI("ERROR",isQuery,StatusEN61850SynclostCount,NULL,nixCmd,GetStatusEN61850Error);
    StatusEN61850DataCount=new cNodeSCPI("DATCOUNT",isQuery,StatusEN61850Error,NULL,nixCmd,GetStatusEN61850DataCount);			  			  
    StatusEN61850=new cNodeSCPI("EN61850",isNode,NULL,StatusEN61850DataCount,nixCmd,nixCmd);			  
    StatusQuestionablePTransition=new cNodeSCPI("PTRANSITION",isQuery | isCommand,NULL,NULL,SetStatusQuestionablePTrans,GetStatusQuestionablePTrans);			  
    StatusQuestionableNTransition=new cNodeSCPI("NTRANSITION",isQuery | isCommand,StatusQuestionablePTransition,NULL,SetStatusQuestionableNTrans,GetStatusQuestionableNTrans);
    StatusQuestionableEnable=new cNodeSCPI("ENABLE",isQuery | isCommand,StatusQuestionableNTransition,NULL,SetStatusQuestionableEnable,GetStatusQuestionableEnable);
    StatusQuestionableCondition=new cNodeSCPI("CONDITION",isQuery | isCommand,StatusQuestionableEnable,NULL,SetStatusQuestionableCondition,GetStatusQuestionableCondition);
    StatusQuestionableEvent=new cNodeSCPI("EVENT",isQuery,StatusQuestionableCondition,NULL,nixCmd,GetStatusQuestionableEvent);
    StatusQuestionable=new cNodeSCPI("QUESTIONABLE",isNode | isQuery,StatusEN61850,StatusQuestionableEvent,nixCmd,GetStatusQuestionableEvent);	
    
    StatusOperationPTransition=new cNodeSCPI("PTRANSITION",isQuery | isCommand,NULL,NULL,SetStatusOperationPTrans,GetStatusOperationPTrans);
    StatusOperationNTransition=new cNodeSCPI("NTRANSITION",isQuery | isCommand,StatusOperationPTransition,NULL,SetStatusOperationNTrans,GetStatusOperationNTrans);
    StatusOperationEnable=new cNodeSCPI("ENABLE",isQuery | isCommand,StatusOperationNTransition,NULL,SetStatusOperationEnable,GetStatusOperationEnable);
    StatusOperationCondition=new cNodeSCPI("CONDITION",isQuery | isCommand,StatusOperationEnable,NULL,SetStatusOperationCondition,GetStatusOperationCondition);
    StatusOperationEvent=new cNodeSCPI("EVENT",isQuery,StatusOperationCondition,NULL,nixCmd,GetStatusOperationEvent);
    StatusOperation=new cNodeSCPI("OPERATION",isNode | isQuery,StatusQuestionable,StatusOperationEvent,nixCmd,GetStatusOperationEvent);
    StatusStandard=new cNodeSCPI("STANDARD",isQuery,StatusOperation,NULL,nixCmd,GetStatusStandard);
    Status=new cNodeSCPI("STATUS",isNode,System,StatusStandard,nixCmd,nixCmd);
    

    // implementiertes store model
   
    StoreChannelNOffset=new cNodeSCPI("NOFFSET",isQuery,NULL,NULL,nixCmd,GetChannelNOffsetCmd);
    StoreChannelXOffset=new cNodeSCPI("XOFFSET",isQuery,StoreChannelNOffset,NULL,nixCmd,GetChannelXOffsetCmd);
    Store=new cNodeSCPI("STORE",isNode,Status,StoreChannelXOffset,nixCmd,nixCmd);

    // implementiertes sense model
    
    SenseCNameRangeCatalog=new cNodeSCPI("CATALOG",isQuery,NULL,NULL,nixCmd,OutRangeCatalog);
    SenseCNameRange=new cNodeSCPI("RANGE",isNode | isCommand | isQuery,NULL,SenseCNameRangeCatalog,SetRange,GetRange);
    SenseCName=new cNodeSCPIVar(&mMeasChannelList,isNode,NULL,SenseCNameRange,nixCmd,nixCmd);
    SenseChannelCatalog=new cNodeSCPI("CATALOG",isQuery,NULL,NULL,nixCmd,OutChannelCatalog);
    SenseChannel=new cNodeSCPI("CHANNEL",isNode,SenseCName,SenseChannelCatalog,nixCmd,nixCmd);
    Sense=new cNodeSCPI("SENSE",isNode,Store,SenseChannel,nixCmd,nixCmd);
    
    
    // implementiertes measure model 
    
    ReadLoadpoint=new cNodeSCPI("LOADPOINT",isQuery,NULL,NULL,nixCmd,MeasurementReadLoadpoint); 
    Read=new cNodeSCPI("READ",isNode | isQuery,Sense,ReadLoadpoint,nixCmd,MeasurementRead);
    Fetch=new cNodeSCPI("FETCH",isNode | isQuery,Read,NULL,nixCmd,MeasurementFetch);
    Initiate=new cNodeSCPI("INITIATE",isNode | isCommand,Fetch,NULL,MeasurementInitiate,nixCmd);	     
    Measure=new cNodeSCPI("MEASURE",isNode | isQuery,Initiate,Initiate,nixCmd,Measurement);
 
     // implementiertes configuration model 
    
    ConfigurationApply=new cNodeSCPI("APPLY",isCommand,NULL,NULL,ConfApply,nixCmd);
    
//    ConfigurationLogFileSize=new cNodeSCPI("SIZE",isQuery | isCommand,NULL,NULL,SetConfLogFileSize,GetConfLogFileSize);
//    ConfigurationLogFile=new cNodeSCPI("LOGFILE",isNode,ConfigurationApply,ConfigurationLogFileSize,nixCmd,nixCmd);
    ConfigurationEN61850Appid=new cNodeSCPI("APPID",isQuery | isCommand,NULL,NULL,SetConfEnAppid,GetConfEnAppid);
    ConfigurationEN61850Vid=new cNodeSCPI("VID",isQuery | isCommand,ConfigurationEN61850Appid,NULL,SetConfEnVid,GetConfEnVid);
    ConfigurationEN61850Cfi=new cNodeSCPI("CFI",isQuery | isCommand,ConfigurationEN61850Vid,NULL,SetConfEnCfi,GetConfEnCfi);
    ConfigurationEN61850UserPriority=new cNodeSCPI("UPRIORITY",isQuery | isCommand,ConfigurationEN61850Cfi,NULL,SetConfEnUPriority,GetConfEnUPriority);

    ConfigurationEN61850LAsdu=new cNodeSCPI("LASDU",isQuery | isCommand,ConfigurationEN61850UserPriority,NULL,SetConfENLAsdu,GetConfENLAsdu);


    ConfigurationEN61850FAsdu=new cNodeSCPI("FASDU",isQuery | isCommand,ConfigurationEN61850LAsdu,NULL,SetConfENFAsdu,GetConfENFAsdu);
    ConfigurationEN61850DataSet=new cNodeSCPI("DATASET",isQuery | isCommand,ConfigurationEN61850FAsdu,NULL,SetConfEnDSet,GetConfEnDSet);
    ConfigurationEN61850MacAdressWM3000=new cNodeSCPI("WM3000",isQuery | isCommand,NULL,NULL,SetConfENMAdrWM3000,GetConfENMAdrWM3000);
    ConfigurationEN61850MacAdressMergingUnit=new cNodeSCPI("MERGINGUNIT",isQuery | isCommand,ConfigurationEN61850MacAdressWM3000,NULL,SetConfENMAdrMU,GetConfENMAdrMU);
    ConfigurationEN61850MacAdress=new cNodeSCPI("MACADRESS",isNode,ConfigurationEN61850DataSet,ConfigurationEN61850MacAdressMergingUnit,nixCmd,nixCmd);
    ConfigurationEN61850=new cNodeSCPI("EN61850",isNode,ConfigurationApply,ConfigurationEN61850MacAdress,nixCmd,nixCmd);
    ConfigurationRatioEVT=new cNodeSCPI("EVT",isQuery | isCommand,NULL,NULL,SetConfRatioEvt,GetConfRatioEvt);
    ConfigurationRatioX=new cNodeSCPI("X",isQuery | isCommand,ConfigurationRatioEVT,NULL,SetConfRatioChx,GetConfRatioChx);
    ConfigurationRatioN=new cNodeSCPI("N",isQuery | isCommand,ConfigurationRatioX,NULL,SetConfRatioChn,GetConfRatioChn);
    ConfigurationRatio=new cNodeSCPI("RATIO",isNode,ConfigurationEN61850,ConfigurationRatioN,nixCmd,nixCmd);
    ConfigurationSynchronizationStrong=new cNodeSCPI("STRONG",isQuery | isCommand,NULL,NULL,SetConfSyncStrong,GetConfSyncStrong);
    ConfigurationSynchronizationPeriod=new cNodeSCPI("PERIOD",isQuery | isCommand,ConfigurationSynchronizationStrong,NULL,SetConfSyncPeriod,GetConfSyncPeriod);
    ConfigurationSynchronizationSource=new cNodeSCPI("SOURCE",isQuery | isCommand,ConfigurationSynchronizationPeriod,NULL,SetConfSyncSource,GetConfSyncSource);
    ConfigurationSynchronization=new cNodeSCPI("SYNCRONISATION",isNode,ConfigurationRatio,ConfigurationSynchronizationSource,nixCmd,nixCmd);
    ConfigurationMeasureIntegrationTime=new cNodeSCPI("INTEGRATIONTIME",isQuery | isCommand,NULL,NULL,SetConfMeasTInt,GetConfMeasTInt);
    ConfigurationMeasureMPeriod=new cNodeSCPI("MPERIOD",isQuery | isCommand,ConfigurationMeasureIntegrationTime,NULL,SetConfMeasMPeriod,GetConfMeasMPeriod);
    ConfigurationMeasureSRate=new cNodeSCPI("SRATE",isQuery | isCommand,ConfigurationMeasureMPeriod,NULL,SetConfMeasSRate,GetConfMeasSRate);
    ConfigurationMeasureSigFrequency=new cNodeSCPI("SIGFREQUENCY",isQuery | isCommand,ConfigurationMeasureSRate,NULL,SetConfMeasSFreq,GetConfMeasSFreq);
    ConfigurationMeasure=new cNodeSCPI("MEASURE",isNode,ConfigurationSynchronization,ConfigurationMeasureSigFrequency,nixCmd,nixCmd);
    ConfigurationComputationPHCorrectionTime=new cNodeSCPI("TIME",isQuery | isCommand,NULL,NULL,SetConfCompPhcTime,GetConfCompPhcTime);
    ConfigurationComputationPHCorrectionPhase=new cNodeSCPI("PHASE",isQuery | isCommand,ConfigurationComputationPHCorrectionTime,NULL,SetConfCompPhcPhase,GetConfCompPhcPhase);
    ConfigurationComputationPHCorrection=new cNodeSCPI("PHCORRECTION",isNode,NULL,ConfigurationComputationPHCorrectionPhase,nixCmd,nixCmd);
    ConfigurationComputationOECorrectionFile=new cNodeSCPI("FILE",isQuery | isCommand,NULL,NULL,SetConfCompOecFile,GetConfCompOecFile);
    ConfigurationComputationOECorrectionOn=new cNodeSCPI("ON",isQuery | isCommand,ConfigurationComputationOECorrectionFile,NULL,SetConfCompOecOn,GetConfCompOecOn);
    ConfigurationComputationOECorrection=new cNodeSCPI("OECORRECTION",isNode,ConfigurationComputationPHCorrection,ConfigurationComputationOECorrectionOn,nixCmd,nixCmd);
    ConfigurationComputationXOffset=new cNodeSCPI("XOFFSET",isQuery | isCommand,ConfigurationComputationOECorrection,NULL,SetConfCompOffskX,GetConfCompOffskX);
    ConfigurationComputationNOffset=new cNodeSCPI("NOFFSET",isQuery | isCommand,ConfigurationComputationXOffset,NULL,SetConfCompOffskN,GetConfCompOffskN);

    ConfigurationComputation=new cNodeSCPI("COMPUTATION",isNode,ConfigurationMeasure,ConfigurationComputationNOffset,nixCmd,nixCmd);
    ConfigurationOperationSignalCatalog=new cNodeSCPI("CATALOG",isQuery,NULL,NULL,nixCmd,GetConfOperSignalCatalog);
    ConfigurationOperationSignal=new cNodeSCPI("SIGNAL",isNode | isQuery | isCommand,ConfigurationComputation,ConfigurationOperationSignalCatalog,SetConfOperSignal,GetConfOperSignal);
    ConfigurationOperationModeCatalog=new cNodeSCPI("CATALOG",isQuery,NULL,NULL,nixCmd,GetConfOperModeCatalog);
    ConfigurationOperationMode=new cNodeSCPI("MODE",isNode | isQuery | isCommand,ConfigurationOperationSignal,ConfigurationOperationModeCatalog,SetConfOperMode,GetConfOperMode);
    ConfigurationOperation=new cNodeSCPI("OPERATION",isNode,ConfigurationComputation,ConfigurationOperationMode,nixCmd,nixCmd);
    Configuration=new cNodeSCPI("CONFIGURATION",isNode | isCommand,Measure,ConfigurationOperation,MeasurementConfigure,nixCmd);  
    
    return (Configuration);
}

