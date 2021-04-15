#include <qstringlist.h>

#include "devserver.h"
#include "scpiface.h"
#include "scpi.h" // für scpi knoten
// #include "wm3000u.h"


scpiErrorType SCPIError[scpiLastError] = {  {0,(char*)"No error"},
                                            {-100,(char*)"Command error"},
                                            {-103,(char*)"Invalid separator"},
                                            {-108,(char*)"Parameter not allowed"},
                                            {-109,(char*)"Missing parameter"},
                                            {-113,(char*)"Undefined header"},
                                            {-120,(char*)"Numeric data error"},
					
                                            {-200,(char*)"Execution error"},
                                            {-203,(char*)"Command protected"},
                                            {-240,(char*)"Hardware error"},
                                            {-241,(char*)"Hardware missing"},
                                            {-257,(char*)"File name error"},
					
                                            {-300,(char*)"Device-specific error"},
                                            {-310,(char*)"System error"},
                                            {-350,(char*)"Queue overflow"},

                                            {-400,(char*)"Query error"},
					
                                            {-500,(char*)"Power on"} };


cSCPIStatSyst::cSCPIStatSyst(cSCPIFace* ip, uchar bit)  // callback und bit fürs statusbyte
{
    m_myIFace = ip;
    m_nStatBit = bit;
    m_nCondition = 0;
    m_nPTransition = 0;
    m_nNTransition = 0;
    m_nEnable = 0;
    m_nEvent = 0;
}


void cSCPIStatSyst::SetCondition(ushort w)  // überschreibt alle bits ...also ev. vorher lesen und 
{
    ushort oldcond, Edge;
    oldcond = m_nCondition;
    m_nCondition = w;
    Edge = (oldcond ^ m_nCondition) & m_nCondition; // pos. flanken
    m_nEvent |= Edge & m_nPTransition;
    if (Edge & m_nEnable)
	m_myIFace->SetSTB(m_nStatBit);
    Edge = (oldcond ^ m_nCondition) & ~m_nCondition; // neg. flanken
    m_nEvent |= Edge & m_nPTransition;
    if (Edge & m_nEnable)
	m_myIFace->SetSTB(m_nStatBit);
}   


ushort cSCPIStatSyst::GetCondition() {
    return m_nCondition;
}


void cSCPIStatSyst::SetNTransition(ushort w) {
    m_nNTransition = w;
}
 

ushort cSCPIStatSyst::GetNTransition() {
    return m_nNTransition;
}
 

void cSCPIStatSyst::SetPTransition(ushort w) {
    m_nPTransition = w;
}
 

ushort cSCPIStatSyst::GetPTransition() {
    return m_nPTransition;
}


void cSCPIStatSyst::SetEnable(ushort w) {
    m_nEnable = w;
    SetCondition( GetCondition() ); // 3x darfs´ te raten 
}
 

ushort cSCPIStatSyst::GetEnable() {
    return m_nEnable;
}
 

ushort cSCPIStatSyst::GetEvent() {
    ushort oldEvent = m_nEvent;
    m_nEvent = 0; // events werden nach dem lesen gelöscht
    return oldEvent;
}
    

void cSCPIStatSyst::SetConditionBit(ushort bp) {
    SetCondition( GetCondition() | bp );
}
 

void cSCPIStatSyst::ResetConditionBit(ushort bp) {
    SetCondition( GetCondition() & ~bp );
}


cCmdTimer::cCmdTimer(QObject * parent, const char * name)
    :QTimer(parent, name)
{
    connect(this, SIGNAL(timeout()), this, SLOT(TimeExpired()));
}


int cCmdTimer::start(int t, QString& s)
{
    m_sCmd = s;
    return QTimer::start(t, true); // immer single shot
}
	

void cCmdTimer::TimeExpired()
{
    emit Command2Execute(m_sCmd);
}


cSCPIFace::cSCPIFace(cClientIODevice* ciod,short l) { // länge für die eventqueue
    m_nPriority = 0; // falls nix anderes mehr kommt haben wir die höchste
    m_pCIOD = ciod; // unser io device
    m_nQueueLen = l;
    m_pCmdTimer = new cCmdTimer();
    m_pSMachineTimer = new(cSMTimer);
    m_pParser = new cParse(); // default parser
    m_pCmdInterpreter = new cCmdInterpreter(m_pParser);
    m_pOperationStat = new cSCPIStatSyst(this ,STBoper); // events landen in bit7 d. statusbytes
    m_pQuestionableStat = new cSCPIStatSyst(this, STBques); // events landen in bit3 d. statusbytes
    m_nSTB = 0; // status byte
    m_nSRE = 0; // service reguest enable
    m_nESR = 0; // standard event status register 
    m_nESE = 0; // standard event status enable register
    
    m_nOPCState = OCIS; // operation complete idle state
    m_nOPCQState = OQIS; // operation complete query idle state
    m_bnoOperationPendingFlag = true;
    m_pCommands = InitCommonCmdTree(0); // kommando liste für common commands
    
    connect(m_pCIOD,SIGNAL(SendCommand( QString&)),SLOT(ReceiveCommand(QString&)));
    connect(this, SIGNAL(SendAnswer(QString&)), m_pCIOD, SLOT(ReceiveAnswer( QString&)) );
    connect(m_pCmdTimer,SIGNAL(Command2Execute(QString&)),this, SLOT(CmdExecution(QString&)));
    connect(m_pSMachineTimer,SIGNAL(timeout(int)),this, SLOT(ExecuteCommand(int)));
}
 

cSCPIFace::~cSCPIFace()
{
    delete m_pCIOD;
    delete m_pCmdTimer;
    delete m_pSMachineTimer;
    delete m_pParser;
    delete m_pCmdInterpreter;
    delete m_pOperationStat;
    delete m_pQuestionableStat;
}


cClientIODevice* cSCPIFace::GetClientIODevice()
{
    return m_pCIOD;
}

void cSCPIFace::AddEventError(int e)  // fehler bzw. event eintrag in queue
{
    m_bCmdError = true; // wir merken uns dass ein fehler aufgetreten ist
    SetSTB(STBeeQueueNotEmpty); // wir haben was in der queue -> STB 
    if ( m_ErrEventQueue.count() == m_nQueueLen ) {
	m_ErrEventQueue.pop_back();
	m_ErrEventQueue.append(QueueOverflow);
    }
    else
	m_ErrEventQueue.append(e);
}


void cSCPIFace::ClearEventError()
{
    ResetSTB(STBmav); // message avail im status byte löschen
    m_ErrEventQueue.clear(); // alle einträge in liste löschen
}


void cSCPIFace::ReceiveCommand( QString& s)
{
    m_sCommand = s;
    ExecuteCommand(ExecCmdStart); // statemachine starten für neues kommando
    /*
    if (Answer != "") // wir haben eine Antwort wenn es eine query war und kein fehler aufgetreten ist
	emit SendAnswer(Answer);
	*/
}


void cSCPIFace::ReceivePriority(int p)
{
    m_nPriority = p;
}


void cSCPIFace::AffectSCPIStatus(uchar action, ushort stat)
{
    switch (action)
    {
    case SetOperStat:
	m_pOperationStat->SetConditionBit(stat);
	break;
	
    case ResetOperStat:
	m_pOperationStat->ResetConditionBit(stat);
    if (stat == OperConfiguring)
    {
        SetnoOperFlag(true);
        if (m_nOPCQState == OQAS)
        // wenn wir in opc query active state sind und die Configuration fertig war
        {
            // d.h. es war eine *opc? abfrage während wir dabei waren zu konfigurieren
            {
                //QString s;
                //emit SendAnswer(s = QString("+1")); // dann senden wir jetzt die antwort darauf
                setOPCQState(OQIS); // und wechseln wieder in den operation query idle state
            }
        }
        //else
            //SetnoOperFlag(true);
    }
	break;
	
    case SetQuestStat:
	m_pQuestionableStat->SetConditionBit(stat);
	break;
	
    case ResetQuestStat:
	m_pQuestionableStat->ResetConditionBit(stat);
	break;
    }
}


void cSCPIFace::ResetDevice()
{
    m_nOPCState = OCIS; // bei rst auch opcidle
    m_nOPCQState = OQIS; 
    SetnoOperFlag(true);
}


void cSCPIFace::DeviceClearStatus()
{
    ResetESR(255); // standard event status register = 0
    ResetSTB(255); // status byte = 0
    m_pOperationStat->SetCondition(0); // oper status 0 setzen
    m_pQuestionableStat->SetCondition(0); // quest.  status 0 setzen
    ClearEventError(); // error event queue löschen
    m_nOPCState = OCIS; // opcidle    
    m_nOPCQState = OQIS;
}


void cSCPIFace::OPCCommand()
{   // kann falls anders sein soll überschrieben werden
    m_nOPCState = OCAS; // standardmäßig nur seq. kommando bearbeitung
    SetnoOperFlag(true); // wir setzen ocas, setzen das nooperpending flag => setzen opc im sesr
}


char* cSCPIFace::OPCQuery()
{   // kann falls anders sein soll überschrieben werden, wir lassen nur seq. kommandos zu
    /*
    char* out;
    out = (char*) malloc(3);
    strcpy(out,"+1");
    return out;
    */

    // es wurde überschrieben
    char* out;

    out = 0; // kein output
    if (m_bnoOperationPendingFlag) // wenn keine konfiguration pending ist geben wir direkt eine anrwort
    {
        out = (char*) malloc(3);
        strcpy(out,"+1");
    }
    else
    {
        //m_bnoOperationPendingFlag = true;
        setOPCQState(OQAS); // ansonsten merken wir uns daß eine *opc? anfrage war
    }

    return out;
}


void cSCPIFace::DeviceReset(char* s) 
{
    QString keyw = m_pParser->GetKeyword(&s);
    if ( keyw.isEmpty() ) // hier darf nichts stehen 
	ResetDevice();
    else
	AddEventError(ParameterNotAllowed);
}


void cSCPIFace::SetDeviceOPC(char* s)
{
    QString keyw = m_pParser->GetKeyword(&s);
    if ( keyw.isEmpty() ) // hier darf nichts stehen 
	OPCCommand();
    else
	AddEventError(ParameterNotAllowed);
}


void cSCPIFace::DeviceClearStatus(char* s)
{
    QString keyw = m_pParser->GetKeyword(&s);
    if ( keyw.isEmpty() ) // hier darf nichts stehen 
	DeviceClearStatus();
    else
	AddEventError(ParameterNotAllowed);
}


void cSCPIFace::SetDeviceESE(char* s)
{
    SetIEEE488Register(s, m_nESE);
}
 

void cSCPIFace::SetDeviceSRE(char* s)
{
    SetIEEE488Register(s, m_nSRE);
}
 

void cSCPIFace::SetIEEE488Register(char* s, uchar& reg)
{
    bool ok; 
    QString par = m_pParser->GetKeyword(&s); // holt den parameter aus dem kommando
    int nPar = par.toInt(&ok);
    if ( (ok) && (nPar < 256) ) {
	par = m_pParser->GetKeyword(&s);
	if ( par.isEmpty() )
	    reg = nPar;
	else
	    AddEventError(InvalidSeparator); // macht agilent auch so
    }
    else 
	AddEventError(NumericDataError);
}   
   

void cSCPIFace::SetnoOperFlag(bool b)
{
    m_bnoOperationPendingFlag = b;
    if ( (b) && (m_nOPCState == OCAS))
    { // wenn operation complete active state
        m_nOPCState = OCIS; // dann simmer widder idle
        SetESR(SESROperationComplete);
    }
}	
	
    
void cSCPIFace::SetSTB(uchar b) // status register setzen
{
    m_nSTB |= b;
    if (m_nSTB & m_nSRE & 0xBF) // wenn enabled
	m_nSTB |= STBrqs; // request service message ins stb
}
 

void cSCPIFace::ResetSTB(uchar b) // bzw. rücksetzen
{
    m_nSTB &= ~b;
}
 

void cSCPIFace::SetSRE(uchar b) // service request register setzen
{
    m_nSRE |= b;
    SetSTB(m_nSTB); 
}


void cSCPIFace::ResetSRE(uchar b) // bzw. rücksetzen
{
    m_nSRE &= ~b;
}


void cSCPIFace::SetESR(uchar b) // standard event status register setzen
{
    m_nESR |= b;
    if (m_nESR & m_nESE) // falls enabled setzen wir 
	SetSTB(STBesb);
}


void cSCPIFace::ResetESR(uchar b) // bzw. rücksetzen
{
    m_nESR &= ~b;
}


void cSCPIFace::SetESE(uchar b) // standard event enable register setzen
{
    m_nESE |= b;
    SetESR(m_nESR); 
}


void cSCPIFace::ResetESE(uchar b) // bzw. rücksetzen    
{
    m_nESE &= ~b;
}


char* cSCPIFace::GetDeviceIdentification()
{
    char* out;
    out = (char*) malloc(12);
    strcpy(out,"DeviceIdent");
    return out;    
}


char* cSCPIFace::DeviceSelfTest()
{
    char* out;
    out = (char*) malloc(11);
    strcpy(out,"DeviceTest");
    return out;    
}


char* cSCPIFace::GetDeviceOPC()
{
    return OPCQuery();
}


char* cSCPIFace::RegConversion(uchar reg)
{
    QString out = "+" + QString::number(reg);
    char* outp = (char*) malloc(out.length()+1);
    strcpy(outp, out.latin1() );
    return outp;
}
    

char* cSCPIFace::GetDeviceESE()
{
    return RegConversion(m_nESE);
}


char* cSCPIFace::GetDeviceESR()
{
    return RegConversion(m_nESR);
}
 

char* cSCPIFace::GetDeviceSRE()
{
    return RegConversion(m_nSRE);
}
 

char* cSCPIFace::GetDeviceSTB()
{
    return RegConversion(m_nSTB);
}


void cSCPIFace::setOPCQState(opcStates state)
{
    if ((m_nOPCQState == OQAS) && (state == OQIS))
    // d.h. es war eine *opc? abfrage während wir dabei waren zu konfigurieren, was jetzt abgeschlossen ist
    {
        QString s;
        emit SendAnswer(s = QString("+1")); // dann senden wir jetzt die antwort darauf
    }

    m_nOPCQState = state;
}


bool cSCPIFace::isAuthorized()
{
    return (m_nPriority == 0);
}


void cSCPIFace::SCPICmd( int cmd,char* s)
{
    switch ((int)cmd)
    {
    case ResetCmd: DeviceReset(s); break; // *RST
    case OperationCompleteCmd: SetDeviceOPC(s); break; // *OPC
    case ClearStatusCmd: DeviceClearStatus(s); break; // *CLS
    case StdEventStatEnableCmd: SetDeviceESE(s); break; // *ESE
    case ServiceRequestEnableCmd: SetDeviceSRE(s); break;// *SRE
    default: qDebug("ProgrammierFehler"); // hier sollten wir nie hinkommen
    }
}


char* cSCPIFace::SCPIQuery( int cmd, char* s) {
    QString keyw = m_pParser->GetKeyword(&s);
    if ( keyw.isEmpty() ) // hier darf nichts stehen 
    {
	switch ((int)cmd)
	{
	case IdentQuery: return GetDeviceIdentification(); // *IDN?
	case SelfTestQuery: return DeviceSelfTest(); // *TST?
	case OperationCompleteQuery: return GetDeviceOPC(); // *OPC?
	case StdEventStatEnableQuery: return GetDeviceESE(); // *ESE?
	case StdEventStatRegQuery: return GetDeviceESR();// *ESR?
	case ServiceRequestEnableQuery: return GetDeviceSRE();// *SRE?
	case StatusByteQuery: return GetDeviceSTB(); // *STB?
	default: qDebug("ProgrammierFehler"); // hier sollten wir nie hinkommen
	}
    }
    else
    {
	AddEventError(ParameterNotAllowed);
    }	
    
    return 0;
}

// hier stehen alle common commands

cNode* CommonIdentification;
cNode* CommonReset;
cNode* CommonSelfTest;
cNode* CommonOperComplete;
cNode* CommonClearStatus;
cNode* CommonStdEventStatusEnable;
cNode* CommonStdEventStatus;
cNode* CommonServiceRequestEnable;
cNode* CommonStatusByte;


cNode* cSCPIFace::InitCommonCmdTree(cNode* cn) {
    CommonIdentification=new cNodeSCPI("*IDN", isQuery,cn,NULL,nixCmd,IdentQuery);  
    CommonReset=new cNodeSCPI("*RST", isCommand,CommonIdentification,NULL,ResetCmd,nixCmd);  
    CommonSelfTest=new cNodeSCPI("*TST", isQuery,CommonReset,NULL,nixCmd,SelfTestQuery);  
    CommonOperComplete=new cNodeSCPI("*OPC", isCommand | isQuery,CommonSelfTest,NULL,OperationCompleteCmd,OperationCompleteQuery);  
    CommonClearStatus=new cNodeSCPI("*CLS", isCommand,CommonOperComplete,NULL,ClearStatusCmd,nixCmd);
    CommonStdEventStatusEnable=new cNodeSCPI("*ESE", isCommand | isQuery,CommonClearStatus,NULL,StdEventStatEnableCmd,StdEventStatEnableQuery);  
    CommonStdEventStatus=new cNodeSCPI("*ESR", isQuery,CommonStdEventStatusEnable,NULL,nixCmd,StdEventStatRegQuery);  
    CommonServiceRequestEnable=new cNodeSCPI("*SRE", isCommand | isQuery,CommonStdEventStatus,NULL,ServiceRequestEnableCmd,ServiceRequestEnableQuery);  
    CommonStatusByte=new cNodeSCPI("*STB", isQuery,CommonServiceRequestEnable,NULL,nixCmd,StatusByteQuery);  

    return CommonStatusByte;
}


