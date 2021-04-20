// definition wm3000u

#include <qapplication.h>
#include <q3ptrlist.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qfile.h>
#include <q3textstream.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qaction.h>
#include <q3mainwindow.h>
#include <qsize.h>
#include <qwidget.h>
#include <q3textbrowser.h>
#include <q3dockwindow.h>
#include <qdir.h>
#include <q3socket.h>
#include <qmessagebox.h>
#include <math.h>
#include <stdlib.h>
#include <qdom.h>
#include <qdatetime.h>
#include <q3valuelist.h>
#include <qfileinfo.h>

#include "wmviewbase.h"
#include "logfile.h"
#include "range.h"
#include "wmglobal.h"
#include "wm3000u.h"
#include "eparameter.h"
#include "tools.h"

extern WMViewBase *g_WMView;
extern char* MModeName[];

const double PI = 3.141592654;
const double PI_180 = 1.74532925e-2;

const float PhaseJustFreq[4] = {16.66, 30.0, 50.0, 60.0}; // feste frequenzwerte zur phasenjustage


cWMessageBox::cWMessageBox ( const QString & caption, const QString & text, Icon icon, int button0, int button1, int button2, QWidget * parent, const char * name , bool modal, Qt::WFlags f ) : QMessageBox ( caption, text, icon, button0, button1, button2, parent, name, modal, f)
{
}

cWMessageBox::~cWMessageBox()
{
    this->QMessageBox::~QMessageBox();
}


void cWMessageBox::done(int r)
{
    emit WMBoxClosed();
    QMessageBox::done(r);
}


cWM3000U::cWM3000U()
{
    SerialVersions.DeviceVersion = WMVersion;
    SerialVersions.PCBSerialNr = "Unknown"; // wird ggf. später aus hardware gelesen
    SerialVersions.PCBVersion = "Unknown";
    SerialVersions.PCBServer = "Unknown";
    SerialVersions.DSPVersion = "Unknown";
    SerialVersions.DSPServer = "Unknown";
    SerialVersions.JDataChksum = "Unknown";
    m_pOwnError = new cOwnError(this); // eigenfehler klasse erzeugen

    // wenn device confdata sendet -> auch  an eigenfehler klasse
    connect(this,SIGNAL(SendConfDataSignal(cConfData*)),m_pOwnError,SLOT(SetConfInfoSlot(cConfData*)));
    connect(this,SIGNAL(SendActValuesSignal(cwmActValues*)),m_pOwnError,SLOT(SetActualValuesSlot(cwmActValues*)));
    connect(m_pOwnError,SIGNAL(SendAnalizeDone(void)),this,SLOT(GetOETAnalizeDone(void)));

    // !!!!! die bereiche in den kanälen n und x sind gleich und müssen gleich bleiben !!!!!
    // bis auf die information welche offsetjustagedaten genutzt werden sollen
    // es gab früher nur 1!!!! liste weil die bereiche wirklich gleich sind
    // aber dadurch dass für jeden bereich in jedem kanal offsetabgleichwerte abgelegt werden müssen
    // brauchen wir unabhängige listen. wenn die bereiche in den kanälen n und x unterschiedlich
    // werden sollen....bedarf es mehr aufwand

    // n kanal anlegen
    m_sNRangeList.setAutoDelete( TRUE ); // the list owns the objects
    m_sNRangeList.append( new CWMRange("480V","480V",480.0,4730418,0.1, "N480V") ); //name,selectname,wert,aussteuerung
    m_sNRangeList.append( new CWMRange("240V","240V",240.0,4730418,0.1, "N240V") );
    m_sNRangeList.append( new CWMRange("120V","120V",120.0,4730418,0.1, "N120V") );
    m_sNRangeList.append( new CWMRange("60V","60V",60.0,4730418,0.1, "N60V") );
    m_sNRangeList.append( new CWMRange("30V","30V",30.0,4730418,0.1, "N30V") );
    m_sNRangeList.append( new CWMRange("15V","15V",15.0,4730418,0.1, "N15V") );
    m_sNRangeList.append( new CWMRange("7.5V","7.5V",7.5,4730418,0.1, "N7.5V") );
    m_sNRangeList.append( new CWMRange("3.75V","3.75V",3.75,4730418,0.1, "N3.75V") );
    m_sNRangeList.append( new CWMRange("Auto","Auto",-1.0,500000,0.1, "N480V") ); // sonderfall

    // dut (x) kanal anlegen
    m_sXRangeList.setAutoDelete( TRUE ); // the list owns the objects
    m_sXRangeList.append( new CWMRange("480V","480V",480.0,4730418,0.1, "X480V") ); //name,selectname,wert,aussteuerung
    m_sXRangeList.append( new CWMRange("240V","240V",240.0,4730418,0.1, "X240V") );
    m_sXRangeList.append( new CWMRange("120V","120V",120.0,4730418,0.1, "X120V") );
    m_sXRangeList.append( new CWMRange("60V","60V",60.0,4730418,0.1, "X60V") );
    m_sXRangeList.append( new CWMRange("30V","30V",30.0,4730418,0.1, "X30V") );
    m_sXRangeList.append( new CWMRange("15V","15V",15.0,4730418,0.1, "X15V") );
    m_sXRangeList.append( new CWMRange("7.5V","7.5V",7.5,4730418,0.1, "X7.5V") );
    m_sXRangeList.append( new CWMRange("3.75V","3.75V",3.75,4730418,0.1, "X3.75V") );
    m_sXRangeList.append( new CWMRange("Auto","Auto",-1.0,500000,0.1, "X480V") ); // sonderfall

    // evt bereiche anlegen
    m_sEVTRangeList.setAutoDelete( TRUE ); // the list owns the objects
//    m_sEVTRangeList.append( new CWMRange("SafetyRange","480V",480.0,4730418,0.03125) );
//   safetyrange wurde entfernt da evt als gesonderter eingang realisiert wurde
    m_sEVTRangeList.append( new CWMRange("15.0V","E15.0V",15.0,2831155,0.1, "ECT15.0V") );
    m_sEVTRangeList.append( new CWMRange("10.0V","E10.0V",10.0,4718592,0.1, "ECT10.0V") );
    m_sEVTRangeList.append( new CWMRange("5.0V","E5.0V",5.0,4718592,0.1, "ECT5.0V") );
    m_sEVTRangeList.append( new CWMRange("2.5V","E2.5V",2.5,4718592,0.1, "ECT2.5V") );
    m_sEVTRangeList.append( new CWMRange("1.0V","E1.0V",1.0,4718592,0.1, "ECT1.0V") );
    m_sEVTRangeList.append( new CWMRange("500mV","E500mV",0.5,2995931,0.1, "ECT500mV") );
    m_sEVTRangeList.append( new CWMRange("250mV","E250mV",0.25,2995931,0.1, "ECT250mV") );
    m_sEVTRangeList.append( new CWMRange("100mV","E100mV",0.1,2995931,0.1, "ECT100mV") );
    m_sEVTRangeList.append( new CWMRange("50mV","E50mV",0.05,2995931,0.1, "ECT50mV") );
    m_sEVTRangeList.append( new CWMRange("25mV","E25mV",0.025,2995931,0.1, "ECT25mV") );
//    m_sEVTRangeList.append( new CWMRange("10mV","E10mV",0.01,2995931,0.1), "ECT15.0V" );
//    m_sEVTRangeList.append( new CWMRange("5mV","E5mV",0.005,2995931,0.1), "ECT15.0V" );
    m_sEVTRangeList.append( new CWMRange("Auto","Auto",-1.0,500000,0.1, "ECT15.0V") ); // sonderfall
    // ende evt bereiche anlegen

    DummyRange = new CWMRange("Unknown","Unknown",-1.0,0,0.1,"Dummy");

    // default konfiguration setzen
    DefaultSettings(m_ConfData);
    m_binitDone = false; // system ist noch nicht initisalisiert

    // default TCP connection
    TCPConfig.pcbHost = TCPConfig.dspHost = TheDevice; // localhost
    TCPConfig.pcbPort = 6305;
    TCPConfig.dspPort = 6310;
    // ende default TCP connection

    LoadSettings(".ses"); // liess ev. mal die einstellungen

    m_ConfData.m_sRangeNVorgabe = "Auto";
    m_ConfData.m_sRangeXVorgabe = "Auto";
    m_ConfData.m_sRangeEVTVorgabe = "Auto";

    DspIFace = new cDspIFace (m_ConfData.m_sADSPFile, TCPConfig.dspHost, TCPConfig.dspPort);
    connect(DspIFace,SIGNAL(iFaceAsync(const QString&)),this,SLOT(DspIFaceAsyncDataSlot(const QString&)));
    connect(DspIFace,SIGNAL(iFaceDone()),this,SLOT(XIFaceDoneSlot()));
    connect(DspIFace,SIGNAL(iFaceError()),this,SLOT(DspIFaceErrorSlot()));

    PCBIFace = new cPCBIFace(TCPConfig.pcbHost, TCPConfig.pcbPort);
    connect(PCBIFace,SIGNAL(iFaceDone()),this,SLOT(XIFaceDoneSlot()));
    connect(PCBIFace,SIGNAL(iFaceError()),this,SLOT(pcbIFaceErrorSlot()));

    MeasureTimer = new QTimer(this);
    connect(MeasureTimer,SIGNAL(timeout()),this,SLOT(MeasureSlot())); // aktivieren messung über timer
    RangeTimer = new QTimer(this);
    connect(RangeTimer,SIGNAL(timeout()),this,SLOT(RangeObsermaticSlot())); // aktivieren range observation + automatic über timer
    MeasureLPTimer = new QTimer(this);
    connect(MeasureLPTimer,SIGNAL(timeout()),this,SLOT(MeasureLPSlot())); // aktivieren messung über timer

    // wir starten die timer erst nach erfolgreicher initialisierung ...oder doch nicht ?

    m_MovingWindowFilter.setFilterLength(m_ConfData.m_nIntegrationTime); // volle sekunden
    MeasureTimer->start(1*1000); //  ab movingwindowfilter immer 1*1000 msec
    RangeTimer->start(500); // 1000 ms
    MeasureLPTimer->start(500);

    m_bStopped = false;
    m_bDspMeasureTriggerActive = false;
    m_bDspMeasureIgnore = false;

    m_ActTimer = new cSMTimer();
    connect(m_ActTimer,SIGNAL(timeout(int)),this,SLOT(ActionHandler(int)));
    m_AsyncTimer = new cSMTimer();
    connect(m_AsyncTimer,SIGNAL(timeout(int)),this,SLOT(ActionHandler(int)));
    connect(this,SIGNAL(StartStateMachine(int)),this,SLOT(ActionHandler(int)));

    DspIFace->ClearMemLists();
    ETHStatusHandle = DspIFace->GetMemHandle(""); // wir holen uns ein memory handle
    DspIFace->addVarItem(ETHStatusHandle, new cDspVar("ETHDATACOUNT",2,vMemory));
    DspIFace->addVarItem(ETHStatusHandle, new cDspVar("ETHERRORS",1,vMemory));
    DspIFace->addVarItem(ETHStatusHandle, new cDspVar("ETHSYNCLOSTCOUNT",1,vMemory));

    ETHStatusResetHandle = DspIFace->GetMemHandle(""); // wir holen uns ein memory handle
    DspIFace->addVarItem(ETHStatusResetHandle, new cDspVar("ETHERRORS",1,vMemory));
    DspIFace->addVarItem(ETHStatusResetHandle, new cDspVar("ETHSYNCLOSTCOUNT",1,vMemory));

    if(!QDir(QString("%1/wm3000u/log/").arg(wm3000uHome)).exists())
    {
      //create temporary object that gets deleted when leaving the control block
      QDir().mkdir(QString("%1/wm3000u/").arg(wm3000uHome));
      QDir().mkdir(QString("%1/wm3000u/log/").arg(wm3000uHome));
    }

    m_SelftestLogfile.setName(QDir(SelftestLogFilePath).absPath());
    m_PhaseJustLogfile.setName(QDir(PhaseJustLogFilePath).absPath());
    m_OffsetJustLogfile.setName(QDir(OffsetJustLogFilePath).absPath());
    m_OffsetDatafile.setName(QDir(OffsetJustDataFilePath).absPath());
    m_NSAOffsetDatafile.setName(QDir(NSAOffsetJustDataFilePath).absPath());

    ulong* pdata = (ulong*) ETHStatusResetHandle->data();
    *pdata = 0;
    pdata++;
    *pdata = 0;
    m_pProgressDialog = 0; // dat muss

    ActValues.RMSNSek = 0.0;  // wir benötigen definierte istwerte, damit wir die korrekturwerte
    ActValues.RMSXSek = 0.0; // lesen können
    ActValues.VekN = 0.0;
    ActValues.VekNSek = 0.0;
    ActValues.VekX = 0.0;
    ActValues.VekXSek = 0.0;
    ActValues.PHIN = 0.0;
    ActValues.PHIX = 0.0;
    ActValues.Frequenz = 50.0;


    m_OVLMsgBox = new cWMessageBox ( trUtf8("Übersteuerung"), trUtf8("Es ist eine Übersteuerung im grössten Bereich\naufgetreten. Bitte überprüfen Sie die Messgrössen"), QMessageBox::Critical, QMessageBox::Ok, Qt::NoButton, Qt::NoButton, 0, 0, false ) ;
    connect(m_OVLMsgBox,SIGNAL(WMBoxClosed()),this,SLOT(OverLoadMaxQuitSlot()));
    m_SelftestMsgBox = new cWMessageBox ( trUtf8("Selbstest"), trUtf8("Test beendet\nDetails stehen im Logfile"), QMessageBox::Information, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, 0, 0, false ) ;
}


cWM3000U::~cWM3000U()
{
    WriteSettings(".ses");
    delete DspIFace;
    delete PCBIFace;
}


void cWM3000U::ActionHandler(int entryAHS)
{
    const char* SenseModeText[anzSenseMode] = {"sensNsensX", "adcNadcX", "sensNadcX", "sensXadcN", "sensNsensX0V"};

    static int AHS = wm3000Idle; // action handler state
    static bool bOverloadMax = false;
    static bool bOverloadMaxOld = false;
    static bool bOverload = false;
    static int lprogress;
    static int N;
    static int mCount;
    static cCalcInfo *PhaseCalcInfo;
    static cCalcInfo *OffsetCalcInfo;
    static cJustMeasInfo *PhaseNodeMeasInfo;
    static cJustMeasInfo *OffsetMeasInfo;
    static float ph0,ph1;
    static float offs0, offs1;
    static complex SenseVektor, ADCVektor;


    if (entryAHS == EnterSimulationMode)
    { // wenn fehler aufgetreten und bediener sim. gewählt hat
        m_ConfData.m_bSimulation = true;
        emit SendConfDataSignal(&m_ConfData);
        AHSFifo.clear();
        if (m_pProgressDialog)
            delete m_pProgressDialog;
        m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
        AHS = wm3000Idle;
        return;
    }

    if ( (entryAHS != wm3000Continue) && (entryAHS != wm3000Repeat) )
    { // wir sollen was neues starten
    if (AHS != wm3000Idle) { // sind aber noch nicht fertig
     if (AHSFifo.contains(entryAHS) ==0 ) // wir haben dieses event noch nicht in der liste
        AHSFifo.push_back(entryAHS); // dann merken wir uns was starten wir sollten starten sollten
        return; // und sind fertig
    }
    else // oder wir sind fertig und
        AHS = entryAHS; // wir starten es
    }
    else // es ist continue oder repeat
    {
    if (entryAHS == wm3000Repeat)
        AHS--; // für repeat den state dekrementieren
    }

    switch (AHS)
    {

    case InitializationStart:
    StopMeasurement();
    m_binitDone = false; // wir initialisieren !!
    emit SendConfDataSignal(&m_ConfData);
    emit SendRangeListSignal(m_sNRangeList,m_sEVTRangeList);
    AHS++;
    m_ActTimer->start(0,wm3000Continue); // wir starten uns selbst
    break; // InitializationStart

    case InitializationConnect2pcbHost:
    PCBIFace->connectYourself();
    AHS++;
    break; // InitializationConnect2pcbHost

    case InitializationTestDC:
    {
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
            m_ActTimer->start(10,RestartMeasurementStart); // falls wir simuliert gestartet sind ...
        }
        else
        {
            if (m_bDC) // wm3000 wurde mit dc option gestartet
            {
                PCBIFace->ReadJustDataVersion();
                AHS++;
            }
            else
            {
                m_bNoDCAdjust = false; // wenn kein dc -> ist es egal wenn dc nicht justiert wäre
                AHS = InitializationOpenChannel0;
                m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft
            }
        }

        break;
    } // InitializationTestDC

    case InitializationReadJustdataVersion:
    {
        QString vs;

        if (m_ConfData.m_bSimulation)
            AHS = wm3000Idle;
        else
        {
            vs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            vs = vs.right(4);
            m_bNoDCAdjust = (vs.toDouble() < 2.12);
            AHS++;
            m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft
        }
        break;
    }

    case InitializationOpenChannel0:
    if (m_ConfData.m_bSimulation)
    {
        AHS = wm3000Idle;
        m_ActTimer->start(10,RestartMeasurementStart); // falls wir simuliert gestartet sind ...
    }
    else
    {
        PCBIFace->openChannel(0); // kanal 0 öffnen
        AHS++;
    }
    break; // InitializationOpenChannel0

    case InitializationOpenChannel1:
    if (m_ConfData.m_bSimulation)
        AHS = wm3000Idle;
    else
    {
        PCBIFace->openChannel(1); // kanal 1 öffnen
        AHS++;
    }
    break; // InitializationOpenChannel1

    case SetRangeSetRanges:
    case ConfigurationSetRanges:
    case InitializationSetRanges:
    // abgleich der vorgaben
    if  ( m_ConfData.m_sRangeNVorgabe != QString("Auto") ) m_ConfData.m_sRangeNSoll = m_ConfData.m_sRangeNVorgabe;
    if  ( m_ConfData.m_sRangeXVorgabe != QString("Auto") ) m_ConfData.m_sRangeXSoll = m_ConfData.m_sRangeXVorgabe;
    if  ( m_ConfData.m_sRangeEVTVorgabe != QString("Auto") ) m_ConfData.m_sRangeEVTSoll = m_ConfData.m_sRangeEVTVorgabe;

    // das hier war vorher syncrange .... jetzt angepasst auf statemachine
    if (m_ConfData.m_bSimulation) { // wir sind durch fehler oder sonst wie in sim.
        m_ConfData.m_sRangeN = m_ConfData.m_sRangeNSoll; // wir tun so als ob alles gesetzt worden wäre
        m_ConfData.m_sRangeX = m_ConfData.m_sRangeXSoll;
        m_ConfData.m_sRangeEVT = m_ConfData.m_sRangeEVTSoll;
        emit SendConfDataSignal(&m_ConfData); // und teilen dies mit
        AHS = wm3000Idle;
    }
    else
    {
        m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft
        AHS++;
    }
    break; // InitializationSetRanges

    case RangeObsermaticSyncRange0:
    case SetRangeSyncRange0:
    case ConfigurationSyncRange0:
    case InitializationSyncRange0:
    if (bOverloadMax) // nach übersteurung im grössten bereich kein bereiche schalten mehr
    {
        AHS+=6;
        m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft
    }
    else
    {
        PCBIFace->readRange(0); // bereich abfrage starten
        AHS++;
    }

    break; // InitializationSyncRange0


    case RangeObsermaticSyncRange01:
    case SetRangeSyncRange01:
    case ConfigurationSyncRange01:
    case InitializationSyncRange01:
    {
        cRSelectString rss;
        CWMRange* rng;

        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        rss = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
        rng = Range(rss,m_sNRangeList); // sucht den bereich über den selektor aus der liste
        m_ConfData.m_sRangeN = rng->Name(); // istbereich
        if (m_ConfData.m_sRangeN != m_ConfData.m_sRangeNSoll)  // muss was getan werden?
        {
            StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
            PCBIFace->switchRange(0,Range(m_ConfData.m_sRangeNSoll,m_sNRangeList)->Selector());
        }
        else
            m_ActTimer->start(0,wm3000Continue); // event, damit statemachine weiterläuft wenn wir nichts gesendet haben
            AHS++;
        }
        break; // InitializationSwitchRange01
    }

    case RangeObsermaticSyncRange02:
    case SetRangeSyncRange02:
    case ConfigurationSyncRange02:
    case InitializationSyncRange02:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        m_ConfData.m_sRangeN = m_ConfData.m_sRangeNSoll; // bereiche sind jetzt gleich
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
    }
    break; // InitializationSyncRange02

    case RangeObsermaticSyncRange1:
    case SetRangeSyncRange1:
    case ConfigurationSyncRange1:
    case InitializationSyncRange1:
    PCBIFace->readRange(1); // bereich abfrage starten
    AHS++;
    break; // InitializationSyncRange1

    case RangeObsermaticSyncRange11:
    case SetRangeSyncRange11:
    case ConfigurationSyncRange11:
    case InitializationSyncRange11:
    {
        cRSelectString rss;
        CWMRange* rng;

        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        break; // InitializationSwitchRange1
        }
        else
        {
        rss = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
        switch (m_ConfData.m_nMeasMode)
        {
        case Un_UxAbs:
            rng = Range(rss,m_sNRangeList); // sucht den bereich über den selektor aus der liste
            m_ConfData.m_sRangeX = rng->Name(); // istbereich
            if (m_ConfData.m_sRangeX != m_ConfData.m_sRangeXSoll)
            {
            StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
            PCBIFace->switchRange(1,Range(m_ConfData.m_sRangeXSoll,m_sNRangeList)->Selector());
            }
            else
            m_ActTimer->start(0,wm3000Continue);
            break;
        case Un_EVT:
            rng = Range(rss,m_sEVTRangeList);
            m_ConfData.m_sRangeEVT = rng->Name();
            if (m_ConfData.m_sRangeEVT != m_ConfData.m_sRangeEVTSoll)
            {
            StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
            PCBIFace->switchRange(1,Range(m_ConfData.m_sRangeEVTSoll,m_sEVTRangeList)->Selector());
            }
            else
            m_ActTimer->start(0,wm3000Continue);
            break;
        case Un_nConvent:
            if (rss != m_sNRangeList.first()->Selector())
            {
            StopMeasurement(); // wir stoppen die messung wenn wir bereiche schalten
            PCBIFace->switchRange(1,m_sNRangeList.first()->Selector());
            }
            else
            m_ActTimer->start(0,wm3000Continue);
            break;

        default :
            m_ActTimer->start(0,wm3000Continue);
                  }
        }

        AHS++;
        break; //InitializationSyncRange11
    }

    case RangeObsermaticSyncRange12:
    case SetRangeSyncRange12:
    case ConfigurationSyncRange12:
    case InitializationSyncRange12:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        switch (m_ConfData.m_nMeasMode)
        {
        case Un_UxAbs:
        m_ConfData.m_sRangeX = m_ConfData.m_sRangeXSoll;
        break;
        case Un_EVT:
        m_ConfData.m_sRangeEVT = m_ConfData.m_sRangeEVTSoll;
        break;
        default :
        break;
        }
        emit SendConfDataSignal(&m_ConfData); // wir informieren die anderen dass bereiche synchronisiert sind
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
    }

    break; // InitializationSyncRange12

    case InitializationConnectDspIFace:
    // hier kein test auf sim nötig, weil wir nur m_ConfData sync. haben !!!
    DspIFace->connectYourself(); // interface verbindet sich mit seinem server
    AHS++;
    break; // InitializationConnect2dspHost

    case InitializationBootDsp:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        DspIFace->BootDsp();
        AHS++;
    }
    break; // InitializationBootDsp

    case InitializationBootDspPause:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        AHS++;
        m_ActTimer->start(50,wm3000Continue);
        }
        break; // InitializationBootDspPause

    case ConfigurationSetSamplingSystem:
    case InitializationSetSamplingSystem:
    {
        int nSPSP, nSPMP;
        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        switch (m_ConfData.m_nSRate) {
        case S80: nSPSP = 80;break;
        case S256: nSPSP = 256;
        }
        nSPMP = nSPSP /* * m_ConfData.m_nMeasPeriod */;
        DspIFace->SetSamplingSystem(3, nSPSP, nSPMP); // messperiode = signalperiode
        AHS++;
        }
        break; // InitializationSetSamplingSystem
    }

    case ConfigurationSetDspVarList:
    case InitializationSetDspVarList:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SetDspWMVarList(); // dsp user variable definieren
        DspIFace->VarList2Dsp(); // und an den dsp senden
        AHS++;
    }
    break; // InitializationSetDspVarList

    case ConfigurationSetDspCmdLists:
    case InitializationSetDspCmdLists:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SetDspWMCmdList(); // dsp kommando listen defineren
        DspIFace->CmdLists2Dsp(); // und senden
        AHS++;
    }
    break; // InitializationSetDspCmdList

    case ConfigurationActivateCmdLists:
    case InitializationActivateCmdLists:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        DspIFace->ActivateInterface();
        AHS++;
    }
    break; // InitializationActivateCmdLists

    case ConfigurationSetSamplingFrequency:
    case InitializationSetSamplingFrequency:
    {
        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        PCBIFace->setSamplingFrequency(m_ConfData.m_fSFreq);
        AHS++;
        }
        break; // InitializationSetSamplingFrequency
    }

    case ConfigurationSetSamplingPSamples:
    case InitializationSetSamplingPSamples:
    {
        int ps;
        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        switch (m_ConfData.m_nSRate) {
        case S80: ps = 80;break;
        case S256: ps = 256;
        }
        PCBIFace->setSamplingPSamples(ps);
        AHS++;
        }
        break; // InitializationSetSamplingPSamples
    }

    case ConfigurationSetSenseMode:
    case InitializationSetSenseMode:
    {
        int sm;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            sm = m_ConfData.m_nSenseMode;
            PCBIFace->setSenseMode(sm);
            AHS++;
        }
        break; // InitializationSetSenseMode
    }

    case ConfigurationSetSyncSource:
    case InitializationSetSyncSource:
    {
        int sm;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            sm = (m_ConfData.m_nSyncSource == Intern) ? 1 : 0;
            PCBIFace->setSyncSource(sm);
            AHS++;
        }
        break; // InitializationSetSyncSource
    }

    case ConfigurationSetSyncTiming:
    case InitializationSetSyncTiming:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        PCBIFace->setSyncTiming(m_ConfData.m_nTSync);
        AHS++;
    }
    break; // InitializationSetSyncTiming

    case ConfigurationSetDspSignalRouting:
    case InitializationSetDspSignalRouting:
    {
        ulong ethroute[16];
        int i;
        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        for (i = 0; i < 16; i++) ethroute[i] = 0;
        if (m_ConfData.m_nMeasMode == Un_nConvent) // ersetzt kanal 1 daten durch eth. daten
        {
            int asdu;
            int n = m_ConfData.LastASDU - m_ConfData.FirstASDU +1;
            for (i = 0, asdu = m_ConfData.FirstASDU; i < n; i++, asdu++ )
                ethroute[i >> 1] |= ((1 << 8) | (asdu << 4) | m_ConfData.DataSet) << ((1-(i&1))*16);
        }
        DspIFace->SetSignalRouting(ethroute);
        AHS++;
        }
        break; // InitializationSetDspSignalRouting
    }

    case ConfigurationSetDsp61850SourceAdr:
    case InitializationSetDsp61850SourceAdr:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        DspIFace->SetDsp61850SourceAdr(m_ConfData.m_MacSourceAdr);
        AHS++;
    }
    break; // InitializationSetDsp61850SourceAdr

    case ConfigurationSetDsp61850DestAdr:
    case InitializationSetDsp61850DestAdr:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        DspIFace->SetDsp61850DestAdr(m_ConfData.m_MacDestAdr);
        AHS++;
    }
    break; // InitializationSetDsp61850DestAdr

    case ConfigurationSetDsp61850PriorityTagged:
    case InitializationSetDsp61850PriorityTagged:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        DspIFace->SetDsp61850PriorityTagged(m_ConfData.m_nPriorityTagged);
        AHS++;
    }
    break; // InitializationSetDsp61850PriorityTagged

    case ConfigurationSetDsp61850EthTypeAppId:
    case InitializationSetDsp61850EthTypeAppId:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        DspIFace->SetDsp61850EthTypeAppId(m_ConfData.m_nEthTypeHeader);
        AHS++;
    }
    break; // InitializationSetDsp61850EthTypeAppId


    case ConfigurationSetDsp61850EthSynchronisation:
    case InitializationSetDsp61850EthSynchronisation:
    {
        ulong p = 0;
        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        if ( m_ConfData.m_bStrongEthSynchronisation)
            p = m_ConfData.FirstASDU;

        DspIFace->SetDsp61850EthSynchronisation(p);
        AHS++;
        }
        break;
    }

    case SenseProtectionOffResetMaximum:
    case RangeObsermaticResetMaximum:
    case SetRangeResetMaximum:
    case InitializationResetMaximum:
    case RestartMeasurementResetMaximum:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        MaxValues.maxRdy = 0.0; // intern maxima gelöscht
        DspIFace->ResetMaximum();
        AHS++;
    }
    break;

    case SenseProtectionOff:
    case InitializationProtectionOff:

    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        PCBIFace->SetSenseProtection(0); // overloadmax -> schutzschaltung deaktivieren
        AHS++;
    }

    break;



    case InitializationReadPCBDeviceVersion:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        PCBIFace->ReadDeviceVersion();
        AHS++;
    }
    break;

    case InitializationReadPCBServerVersion:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SerialVersions.PCBVersion = PCBIFace->iFaceSock->GetAnswer();
        PCBIFace->ReadServerVersion();
        AHS++;
    }
    break;

    case InitializationReadPCBSerialNr:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SerialVersions.PCBServer = PCBIFace->iFaceSock->GetAnswer();
        PCBIFace->ReadSerialNr();
        AHS++;
    }
    break;

    case InitializationReadDSPDeviceVersion:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SerialVersions.PCBSerialNr = PCBIFace->iFaceSock->GetAnswer();
        DspIFace->ReadDeviceVersion();
        AHS++;
    }
    break;

    case InitializationReadDSPServerVersion:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SerialVersions.DSPVersion = DspIFace->iFaceSock->GetAnswer();
        DspIFace->ReadServerVersion();
        AHS++;
    }
    break;

    case  InitializationReadJustdataChksum:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SerialVersions.DSPServer = DspIFace->iFaceSock->GetAnswer();
        PCBIFace->JustFlashGetChksum();
        AHS++;
    }
    break;

    case InitializationTestAdjustment:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        SerialVersions.JDataChksum = PCBIFace->iFaceSock->GetAnswer();
        emit SendVersionInfo(&SerialVersions);
        PCBIFace->GetAdjustmentStatus();
        AHS++;
    }
    break; // InitializationTestAdjustment

     case InitializationFinished:
     {
     QString s;
     int stat;
     if (m_ConfData.m_bSimulation)
     {
     }
    else
    {
        s = PCBIFace->iFaceSock->GetAnswer();
        stat = s.toInt();
        if ( (stat  > 0) || m_bNoDCAdjust ) // nicht justiert
        {
            m_bJust = false;
            emit JustifiedSignal(false);
            emit AffectStatus(SetQuestStat, QuestNotJustified);

            s = tr("Achtung !");
            s+="\n";
            if (stat & 7 || m_bNoDCAdjust)
                s += trUtf8("Gerät ist nicht justiert !");
            if (stat & 2)
                s += trUtf8("\nNicht identische Versionsnummer !");
            if (stat & 4)
                s += trUtf8("\nNicht identische Seriennummer !");
            if (m_bNoDCAdjust)
                s += trUtf8("\nKeine DC Justage Daten !");

            QMessageBox::critical( 0, "Justage", s);
        }
        else
        {
            m_bJust = true;
            emit JustifiedSignal(true);
            emit AffectStatus(ResetQuestStat, QuestNotJustified);
        }

        m_binitDone = true; // wir hatten keinen fehler
        m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
    }

    AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
    break;
              }
     // konfigurations änderungen und zugehörige auszuführende aktionen
     // modus messung
     // 	bereiche neu setzen, justierwerte im dsp aktualisieren, neues dsp programm, ethroutingtab im dsp
     // signalfrequenz
     //	wenn nConventional neues dsp programm, messplatine init.(signal frequenz)
     // samples/periode
     //	neue dsp varliste, neues dsp programm, messplatine init. (samples/per), sampling system im dsp
     // messintervall
     //	neue dsp varliste, neues dsp program, sampling system im dsp
     // synchronisation
     //	messplatine init.(syncmode bzw. synctiming)
     // mac adressen
     //	ethsourcedest adr im dsp setzen
     // datensatz
     //	asdu u. dataset im dsp setzen (ethrouting)
     // integrationszeit
     //	Timer neu setzen

     // reihenfolge der aktionen wie in !!!! initWM3000 aber nicht alles und selektiv um traffic zu vermeiden
     //	bereiche, varlisten, cmdlisten, messplatine (signalfrequenz, samples/per, syncmode, synctiming)
     // 	signalrouting, mac adressen,
     //	integrationszeit


     case ConfigurationStart:
     m_binitDone = false; // wir konfigurieren
     emit AffectStatus(SetOperStat, OperConfiguring);
     if (m_ConfDataCopy.m_bSimulation && !m_ConfData.m_bSimulation) { // wenn programmkontrolle  -> /simulation
         InitWM3000(); // => alle Initialisierungen vornehmen
         AHS = wm3000Idle;
     }
     else
     {
         // bereiche beim configurieren immer setzen
         AHS++; // wir kommen in ConfigurationSetRanges !!!! die sind gleich mit Initialization !!
         m_ActTimer->start(0,wm3000Continue);
     }

     break; // ConfigurationStart

     case ConfigurationTestIntegrationTime: //  gestartet wird der timer wenn config fertig
     if (m_ConfDataCopy.m_nIntegrationTime != m_ConfData.m_nIntegrationTime)
     {
         StopMeasurement();
         AHS++;
     }

    case ConfigurationTestDspVarList:
     if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if ( (m_ConfDataCopy.m_nSRate != m_ConfData.m_nSRate) ||
         (m_ConfDataCopy.m_nMeasPeriod != m_ConfData.m_nMeasPeriod) )
        {
        StopMeasurement();
        AHS++; // dsp varlisten und sampling system im dsp
        }
        else
        AHS = ConfigurationTestCmdList;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestDspVarList

    case ConfigurationTestCmdList:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if ( (m_ConfDataCopy.m_nSRate != m_ConfData.m_nSRate) ||
         (m_ConfDataCopy.m_nMeasPeriod != m_ConfData.m_nMeasPeriod) ||
         (m_ConfDataCopy.m_nMeasMode != m_ConfData.m_nMeasMode) ||
         (fabs(m_ConfDataCopy.m_fSFreq - m_ConfData.m_fSFreq) > 1e-6) ||
         (m_ConfDataCopy.m_bDCmeasurement != m_ConfData.m_bDCmeasurement) )

        {
        StopMeasurement();
        AHS++; // dsp cmdlisten setzen und übersetzen
        }
        else
        AHS = ConfigurationTestSignalFrequency;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestCmdList

    case ConfigurationTestSignalFrequency:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if (m_ConfDataCopy.m_fSFreq != m_ConfData.m_fSFreq)
        {
        StopMeasurement();
        AHS++; // messplatine signalfrequenz
        }
        else
        AHS = ConfigurationTestSRate;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestSignalFrequency

    case ConfigurationTestSRate:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if (m_ConfDataCopy.m_nSRate != m_ConfData.m_nSRate)
        {
        StopMeasurement();
        AHS++; // messplatine samples/periode
        }
        else
        AHS = ConfigurationTestSenseMode;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestSRate

case ConfigurationTestSenseMode:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if (m_ConfDataCopy.m_nSenseMode != m_ConfData.m_nSenseMode)
        {
            StopMeasurement();
            AHS++; // messplatine test modus
        }
        else
            AHS = ConfigurationTestSyncMode;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestTMode

    case ConfigurationTestSyncMode:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if (m_ConfDataCopy.m_nSyncSource != m_ConfData.m_nSyncSource)
        {
        StopMeasurement();
        AHS++; // // messplatine syncmode
        }
        else
        AHS = ConfigurationTestSyncTiming;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestSyncMode

    case ConfigurationTestSyncTiming:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if (m_ConfDataCopy.m_nTSync != m_ConfData.m_nTSync)
        {
        StopMeasurement();
        AHS++; // sync timing einstellen
        }
        else
        AHS = ConfigurationTestDspSignalRouting;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestSyncTiming

    case ConfigurationTestDspSignalRouting:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if ( (m_ConfDataCopy.m_nMeasMode != m_ConfData.m_nMeasMode)  ||
             (m_ConfDataCopy.FirstASDU != m_ConfData.FirstASDU) ||
             (m_ConfDataCopy.LastASDU != m_ConfData.LastASDU) ||
             (m_ConfDataCopy.DataSet != m_ConfData.DataSet) )
        {
            StopMeasurement();
            AHS++; // signalrouting (ethrouting tab)
        }
        else
            AHS = ConfigurationTestDsp61850SourceAdr;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestDspSignalRouting

    case ConfigurationTestDsp61850SourceAdr:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if   ( (m_ConfData.m_nMeasMode == Un_nConvent) &&
           ( m_ConfDataCopy.m_MacSourceAdr != m_ConfData.m_MacSourceAdr) )
        {
        StopMeasurement();
        AHS++; // mac adressen 61850 source setzen
        }
        else
        AHS = ConfigurationTestDsp61850DestAdr;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestDsp61850SourceAdr

    case ConfigurationTestDsp61850DestAdr:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if   ( (m_ConfData.m_nMeasMode == Un_nConvent) &&
           ( m_ConfDataCopy.m_MacDestAdr != m_ConfData.m_MacDestAdr) )
        {
        StopMeasurement();
        AHS++; // mac adressen 61850 dest
        }
        else
        AHS = ConfigurationTestDsp61850PriorityTagged;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestDsp61850DestAdr

    case ConfigurationTestDsp61850PriorityTagged:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if   ( (m_ConfData.m_nMeasMode == Un_nConvent) &&
           ( m_ConfDataCopy.m_nPriorityTagged != m_ConfData.m_nPriorityTagged) )
        {
        StopMeasurement();
        AHS++; // eth frame priority tagged
        }
        else
        AHS = ConfigurationTestDsp61850EthTypeAppId;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestDsp61850PriorityTagged

    case ConfigurationTestDsp61850EthTypeAppId:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if   ( (m_ConfData.m_nMeasMode == Un_nConvent) &&
           ( m_ConfDataCopy.m_nEthTypeHeader != m_ConfData.m_nEthTypeHeader) )
        {
        StopMeasurement();
        AHS++; // eth frame eth type + appl. id
        }
        else
        AHS = ConfigurationTestDsp61850EthSynchronisation;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestDsp61850EthTypeAppId

    case ConfigurationTestDsp61850EthSynchronisation:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if   ( (m_ConfData.m_nMeasMode == Un_nConvent) &&
           ( m_ConfDataCopy.m_bStrongEthSynchronisation != m_ConfData.m_bStrongEthSynchronisation) )
        {
        StopMeasurement();
        AHS++;
        }
        else
        AHS = ConfigurationFinished;

        m_ActTimer->start(0,wm3000Continue);
    }
    break; // ConfigurationTestDsp61850EthSynchronisation

    case ConfigurationFinished:
    if (m_ConfData.m_bSimulation) {
    }
    else
    {
        // integrationszeit starten wir in jedem fall neu nach konfiguration
        m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
        m_binitDone = true; //
        emit SendConfDataSignal(&m_ConfData); // andere objekte informieren
        // wenn's keine probleme gab sind wir hier angekommen und initialisiert
        emit ConfigReady(); // wir sync. eine ev. laufenden justage prozess
        emit AffectStatus(ResetOperStat, OperConfiguring);
        WriteSettings(".ses");
    }

    AHS = wm3000Idle;
    break; // case ConfigurationAct

    case SetRangeStart:
    emit AffectStatus(SetOperStat, OperRanging);
    AHS++; // es folgen bereich setzen und maximum rücksetzen
    m_ActTimer->start(0,wm3000Continue);
    break; // SetRangeStart

    case SetRangeFinished:
    if (m_ConfData.m_bSimulation) {
    }
    else
    {
        emit AffectStatus(ResetOperStat, OperRanging);
        emit SendConfDataSignal(&m_ConfData); // andere objekte informieren
        emit SetRangeReady(); // falls sich jemand sync. wollte
        m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
    }

    AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
    break; // SetRangeFinished

    case TriggerMeasureStart:
    m_bDspMeasureTriggerActive = true; // der trigger ist aktiviert
    DspIFace->TriggerIntHKSK(1); // die intliste hksk = 1 triggern
    AHS++;
    break; // TriggerMeasureStart

    case  TriggerMeasureFinished:
    AHS = wm3000Idle; // wir sind mit oder ohne fehler fertig
    break;

    case MeasureDataAcquisition:
    case TriggerMeasureDataAcquisition:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        emit AffectStatus(ResetOperStat, OperMeasuring);
        // wieso das hier ??
        // die schnittstellenverbindung kann erst aufgebaut werden wenn das gerät läuft.
        // das interface wird dynamisch instanziiert nach aufbau der verbindung
        // justiert oder nicht justiert wird normalerweise nur einmal beim start emitted
        // und kommt daher dann nicht an. isn workaround, sollte später begradigt werden
        if (m_bJust)
        emit AffectStatus(ResetQuestStat, QuestNotJustified);
        else
        emit AffectStatus(SetQuestStat, QuestNotJustified);

        DspIFace->DataAcquisition(ActValData); // holt die daten ins dsp interface
        emit AffectStatus(SetOperStat, OperMeasuring);
        AHS++;
    }

    break; // TriggerMeasureDataAcquisition

    case MeasureComputation:
    case TriggerMeasureComputation:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
    }
    else
    {
        float *source = DspIFace->data(ActValData);
        float *dest = (float*) &ActValues.dspActValues;
        for (uint i=0; i< sizeof(ActValues.dspActValues)/sizeof(float);i++) *dest++ = *source++;

        m_MovingWindowFilter.append(ActValues.dspActValues);
        ActValues.dspActValues = m_MovingWindowFilter.getOutput();

        // die hanningfenster korrektur findet hier statt weil bei simulation auch cmpactvalues
        // aufgerufen wird, die simulationsdaten aber ohne hanningfenster sind... weil einfacher

        ActValues.dspActValues.rmsnf *= 1.63299; // hanning fenster korrektur
        ActValues.dspActValues.rmsxf *= 1.63299; // dito
        ActValues.dspActValues.ampl1nf *= 2.0; // dito
        ActValues.dspActValues.ampl1xf *= 2.0; // dito

        CmpActFrequency(); // die brauchen wir schon mal zum lesen der justagewerte
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
    }

    break; // TriggerMeasureComputation

    case MeasureGetGainCorrCh0:
    case TriggerMeasureGetGainCorrCh0:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        PCBIFace->readGainCorrection(0, Range(m_ConfData.m_sRangeN,m_sNRangeList)->Selector(), ActValues.RMSNSek);
        AHS++;
    }

    break; // TriggerMeasureGetGainCorrCh0

    case MeasureGetGainCorrCh1:
    case TriggerMeasureGetGainCorrCh1:
    {
        QString gcs;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            gcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.GainCorrCh0 = gcs.toFloat();

            if ( m_ConfData.m_nMeasMode == Un_UxAbs)
                PCBIFace->readGainCorrection(1,  Range(m_ConfData.m_sRangeX,m_sNRangeList)->Selector(), ActValues.RMSXSek);
            else
                PCBIFace->readGainCorrection(1,  Range(m_ConfData.m_sRangeEVT,m_sEVTRangeList)->Selector(), ActValues.RMSXSek);

            AHS++;
        }

        break; // TriggerMeasureGetGainCorrCh1
    }

    case MeasureGetPhaseCorrCh0:
    case TriggerMeasureGetPhaseCorrCh0:
    {
        QString gcs;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            gcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.GainCorrCh1 = gcs.toFloat();

            PCBIFace->readPhaseCorrection(0, Range(m_ConfData.m_sRangeN,m_sNRangeList)->Selector(), ActValues.Frequenz);
            AHS++;
        }

        break; // TriggerMeasureGetPhaseCorrCh0
    }

    case MeasureGetPhaseCorrCh1:
    case TriggerMeasureGetPhaseCorrCh1:
    {
        QString pcs;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            pcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.PhaseCorrCh0 = pcs.toFloat();

            if ( m_ConfData.m_nMeasMode == Un_UxAbs)
                PCBIFace->readPhaseCorrection(1, Range(m_ConfData.m_sRangeX,m_sNRangeList)->Selector(), ActValues.Frequenz);
            else
                PCBIFace->readPhaseCorrection(1, Range(m_ConfData.m_sRangeEVT,m_sEVTRangeList)->Selector(), ActValues.Frequenz);

            AHS++;
        }

        break; // TriggerMeasureGetPhaseCorrCh1
    }

    case MeasureGetOffsetCorrCh0:
    case TriggerMeasureGetOffsetCorrCh0:
    {
        QString pcs;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            pcs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.PhaseCorrCh1 = pcs.toFloat();

            PCBIFace->readOffsetCorrection(0, Range(m_ConfData.m_sRangeN,m_sNRangeList)->Selector(), ActValues.RMSNSek);
            AHS++;
        }

        break; // TriggerMeasureGetOffsetCorrCh0
    }


    case MeasureGetOffsetCorrCh1:
    case TriggerMeasureGetOffsetCorrCh1:
    {
        QString ocs;
        if (m_ConfData.m_bSimulation)
        {
            AHS = wm3000Idle;
        }
        else
        {
            ocs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.OffsetCorrCh0 = ocs.toFloat();

            if ( m_ConfData.m_nMeasMode == Un_UxAbs)
                PCBIFace->readOffsetCorrection(1, Range(m_ConfData.m_sRangeX,m_sNRangeList)->Selector(), ActValues.RMSXSek);
            else
                PCBIFace->readOffsetCorrection(1, Range(m_ConfData.m_sRangeEVT,m_sEVTRangeList)->Selector(), ActValues.RMSXSek);

            AHS++;
        }

        break; // TriggerMeasureGetOffsetCorrCh1
    }

    case MeasureCorrection:
    case TriggerMeasureCorrection:
    {
        QString ocs;
        if (m_ConfData.m_bSimulation)
        {
        }
        else
        {
            ocs = PCBIFace->iFaceSock->GetAnswer(); // antwort lesen
            m_JustValues.OffsetCorrCh1 = ocs.toFloat();

            CorrActValues();
            CmpActValues();
            emit SendActValuesSignal(&ActValues);
            emit MeasureReady();
        }

        AHS = wm3000Idle; // wir sind so oder so fertig
        break; // TriggerMeasureCorrection
    }


    case RestartMeasurementStart:
    if (m_ConfData.m_bSimulation) // im sim. modus wir der meastimer direkt gestartet
    {
        m_MovingWindowFilter.setFilterLength(m_ConfData.m_nIntegrationTime);
        MeasureTimer->start(1*1000); //  ab movingwindowfilter immer 1*1000 msec
        MeasureLPTimer->start(500);
        RangeTimer->start(500);
        m_bDspMeasureTriggerActive = false;
        m_bStopped = false;
        AHS = wm3000Idle; // wir sind schon fertig
    }
    else
    {
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
    }

    break;

    case RestartMeasurementRestart:
    if (m_ConfData.m_bSimulation) // fehler
        AHS = wm3000Idle;
    else
    {
        // im real modus wird das filter im dsp gelöscht und darauf synchroniert der meastimer gestartet
        if (m_bStopped) // es kann sein dass wir restart aufrufen obwohl wir gar nicht gestoppt haben
        {
        m_bStopped = false;
        DspIFace->TriggerIntHKSK(3); // die intliste hksk = 3 triggern löscht das filter im dsp
        AHS++;
        }
        else
        AHS = wm3000Idle;
    }
    break; // RestartMeasurementRestart

    case RestartMeasurementFinished:
    AHS = wm3000Idle; // ob fehler oder nicht .... wir haben fertig
    break;

    case MeasureStart:
    if (m_bStopped)
        AHS = wm3000Idle;
    else
    if (m_bDspMeasureIgnore) // falls zwischen durch was geschaltet oder konf. wurde
    {
        m_bDspMeasureIgnore = false; // verwerfen wir die messergebnisse
        AHS = wm3000Idle;
    }
    else
    {
        emit AffectStatus(SetOperStat, OperMeasuring);
        AHS++; // es folgen measuredatacquisition,  measurecomputation
        m_ActTimer->start(0,wm3000Continue);
    }
    break; // MeasureStart


    case MeasureLPStart:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        DspIFace->DataAcquisition(RMSValData); // holt die daten ins dsp interface
        AHS++;
    }
    break; // MeasureLPStart


    case MeasureLPComputation:
    if (m_ConfData.m_bSimulation)
    {
        AHS = wm3000Idle; // wenn fehler war sind wir idle -> ok
    }
    else
    {
        float *source = DspIFace->data(RMSValData);
        float *dest = (float*) &ActValues.dspRMSValues;
        for (uint i=0; i< sizeof(ActValues.dspRMSValues)/sizeof(float);i++) *dest++ = *source++;

        ActValues.dspRMSValues.fastRMSN *= 1.63299; // hanning fenster korrektur
        ActValues.dspRMSValues.fastRMSN1 = ActValues.dspRMSValues.fastRMSN1 * 2.0 / 1.41421356; // korrektur einheitswurzeln, ammplitude->rms
        ActValues.dspRMSValues.fastRMSX *= 1.63299; // hanning fenster korrektur
        ActValues.dspRMSValues.fastRMSX1 = ActValues.dspRMSValues.fastRMSX1 * 2.0 / 1.41421356; // korrektur einheitswurzeln, ammplitude->rms

        CorrRMSValues();
        CmpRMSValues();
        emit SendLPSignal(&ActValues);

        AHS = wm3000Idle; // fertig
    }

    break; // MeasureLPComputation

    case RangeObsermaticStart:
    DspIFace->DataAcquisition(MaxValData); // holt die daten ins dsp interface
    AHS++;
    break;

    case RangeObsermaticTest:
    {
        bool mustDo = false;
        if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
        }
        else
        {
        float *source = DspIFace->data(MaxValData);
        float *dest = (float*) &MaxValues;
        for (uint i=0; i< sizeof(MaxValues)/sizeof(float);i++) *dest++ = *source++;
        if (MaxValues.maxRdy > 0) { // sind die aktuell ?
            bool bOvln = false;
            bool bOvlx = false;

            bOverloadMaxOld = bOverloadMax;

            mustDo |= SelectRange(m_sNRangeList,m_ConfData.m_sRangeN,m_ConfData.m_sRangeNSoll,m_ConfData.m_sRangeNVorgabe,MaxValues.maxn,bOvln);

            if ( bOvln && (m_ConfData.m_sRangeN == m_sNRangeList.first()->Name()) )
            bOverloadMax = true;

            switch (m_ConfData.m_nMeasMode)
            {
            case Un_UxAbs:
            mustDo |= SelectRange(m_sNRangeList,m_ConfData.m_sRangeX,m_ConfData.m_sRangeXSoll,m_ConfData.m_sRangeXVorgabe,MaxValues.maxx, bOvlx );
            if ( bOvlx && (m_ConfData.m_sRangeX == m_sNRangeList.first()->Name()) )
                bOverloadMax = true;
            break;
            case Un_EVT:
            mustDo |= SelectRange(m_sEVTRangeList,m_ConfData.m_sRangeEVT,m_ConfData.m_sRangeEVTSoll,m_ConfData.m_sRangeEVTVorgabe,MaxValues.maxx, bOvlx);
            if ( bOvlx && (m_ConfData.m_sRangeEVT == m_sEVTRangeList.first()->Name()) )
                bOverloadMax = true;
            break;
            default : break;
            }

                    if ((bOverload = (bOvln || bOvlx) ))
                emit AffectStatus(SetQuestStat, QuestOverLoad); // und an status system melden
            else
                emit AffectStatus(ResetQuestStat, QuestOverLoad );

           if (mustDo)
            AHS++; //  weiter mit bereiche synchronisieren
            else
            AHS = RangeObsermaticOverload; // bzw. direkt overload behandlung

            m_ActTimer->start(0,wm3000Continue);
        }
        else
        {
            AHS = wm3000Idle; // keine gültigen daten -> fertig
        }
        }
        break; // case RangeObsermaticTest
    }


    case RangeObsermaticOverload:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        if (bOverloadMax && (!bOverloadMaxOld) ) // nur  nach positiver flanke !!!
        {
        m_OVLMsgBox->show();
        emit AffectStatus(SetQuestStat, QuestOverLoadMax);
        PCBIFace->SetSenseProtection(1); // overloadmax -> schutzschaltung aktivieren
        }
        else
        m_ActTimer->start(0,wm3000Continue); // rangeobsermaticresetmaximum

        AHS++;
    }

    break;

     //  case RangeObsermaticResetMaximum: // weiter oben realisiert !!!!

     // RangeObsermaticResetOVL findet hier statt


    case RangeObsermaticFinished:
    m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
    emit RangeAutomaticDone();
    AHS = wm3000Idle; // egal ob fehler oder nicht -> obsermatic ist fertig
    break;


    // case SenseProtectionOff weiter oben realisiert (gleich wie init....)

    case SenseProtectionOffRM:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        StopMeasurement(); // messung stop
        AHS++;
        m_ActTimer->start(0,wm3000Continue); // rangeobsermaticresetmaximum
    }

    break;

    // maxima rücksetzten findet hier statt //

    case SenseProtectionOffFinished:
    if (m_ConfData.m_bSimulation) {
        AHS = wm3000Idle;
    }
    else
    {
        m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
        bOverloadMax = false; // wir nehmen die übersteuerung zurück -> ab jetzt können wieder bereiche geschaltet werden
        AHS = wm3000Idle;
    }

    break;


    case CmpPhaseCoeffStart:
        StopMeasurement(); // die kumulieren jetzt nur
        m_pProgressDialog = new Q3ProgressDialog( trUtf8("Berechnung läuft ..."), 0, 4, g_WMView, 0, FALSE, 0 ); // ein progress dialog
        m_pProgressDialog->setCaption("Phasenkorrekturkoeffizienten"); //überschrift
        m_pProgressDialog->setMinimumDuration(0); // sofort sichtbar
        lprogress = 0; // int. progress counter
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break; // CmpPhaseCoeffStart


    case CmpPhaseCoeffCh0:
    case CmpOffsetCoeffCh0:
        lprogress++;
        m_pProgressDialog->setProgress(lprogress);
        AHS++;
        m_ActTimer->start(3000,wm3000Continue);
        break;

    case CmpPhaseCoeffCh1:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            PCBIFace->cmpPhaseCoefficient("ch0"); // phasenkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpPhaseCoeffCh2:
        if (m_ConfData.m_bSimulation)
        {   // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            PCBIFace->cmpPhaseCoefficient("ch1"); // phasenkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpPhaseSetStatus:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            PhaseCalcInfo = m_CalcInfoList.first();
            PCBIFace->setPhaseStatus(PhaseCalcInfo->m_sChannel, PhaseCalcInfo->m_sRange, 1); // wir setzen den status auf justiert
            AHS++;
        }
        break; // CmpPhaseSetStatus

    case CmpPhaseSetStatus2:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            m_CalcInfoList.removeFirst();
            if (m_CalcInfoList.isEmpty())
            {
                AHS++; // dann sind wir fertig
            }
            else
            {
                AHS--; // sonst zum nächsten
            }
            m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
        }
        break; // CmpPhaseSetStatus2

    case CmpPhaseCoeffFinished:
    case CmpOffsetCoeffFinished:
        if (m_ConfData.m_bSimulation)
        { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            m_ActTimer->start(0,RestartMeasurementStart); // messung reaktivieren
            delete m_pProgressDialog;
            AHS = wm3000Idle;
        }
        break; // CmpOffsetCoeffFinished

    case CmpOffsetCoeffStart:
        StopMeasurement(); // die kumulieren jetzt nur
        m_pProgressDialog = new Q3ProgressDialog( trUtf8("Berechnung läuft ..."), 0, 4, g_WMView, 0, FALSE, 0 ); // ein progress dialog
        m_pProgressDialog->setCaption("Offsetkorrekturkoeffizienten"); //überschrift
        m_pProgressDialog->setMinimumDuration(0); // sofort sichtbar
        lprogress = 0; // int. progress counter
        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break; // CmpOffsetCoeffStart

    case CmpOffsetCoeffCh1:
        if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            PCBIFace->cmpOffsetCoefficient("ch0"); // offsetkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpOffsetCoeffCh2:
        if (m_ConfData.m_bSimulation)
        {   // fehler aufgetreten -> abbruch
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            PCBIFace->cmpOffsetCoefficient("ch1"); // offsetkorrektur koeffizienten berechnen lassen
            AHS++;
        }
        break;

    case CmpOffsetSetStatus:
    if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
        AHS = wm3000Idle;
    }
    else
    {
        OffsetCalcInfo = m_CalcInfoList.first();
        PCBIFace->setOffsetStatus(OffsetCalcInfo->m_sChannel, OffsetCalcInfo->m_sRange, 1); // wir setzen den status auf justiert
        AHS++;
    }
    break; // CmpoffsetSetStatus

    case CmpOffsetSetStatus2:
    if (m_ConfData.m_bSimulation) { // fehler aufgetreten -> abbruch
        AHS = wm3000Idle;
    }
    else
    {
        m_CalcInfoList.removeFirst();
        if (m_CalcInfoList.isEmpty())
        {
            AHS++; // dann sind wir fertig
        }
        else
        {
            AHS--; // sonst zum nächsten
        }
        m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
    }
    break; // CmpOffsetSetStatus2

    case PhaseNodeMeasStart:
    m_PhaseJustLogfile.remove(); // beim starten wird das log file gelöscht
    StopMeasurement(); // die kumulieren jetzt nur
    m_pProgressDialog = new Q3ProgressDialog( trUtf8("Koeffizienten 0 setzen ..."), 0, m_PhaseNodeMeasInfoList.count()+1, g_WMView, 0, FALSE, 0 ); // ein progress dialog 100% entspricht alle justierpunkte +1 für das 0 setzen der koeffizienten

    m_pAbortButton = new QPushButton(trUtf8("Abbruch"),0,0);
    m_pProgressDialog->setCancelButton(m_pAbortButton);
    m_pProgressDialog->setCaption(trUtf8("Phasenkorrekturkoeffizienten"));
    m_pProgressDialog->setMinimumDuration(0); // sofort sichtbar
    lprogress = 0; // int. progress counter
    m_pProgressDialog->setProgress(lprogress);
    QObject::connect(m_pAbortButton,SIGNAL(pressed()),this,SLOT(JustAbortSlot()));
    AHS++;
    m_ActTimer->start(0,wm3000Continue);
    N = 0; // durchlaufzähler
    break; // PhaseNodeMeasStart

    case PhaseNodeMeasCoefficientClearN:
        PhaseCalcInfo = m_CalcInfoList.first();
        PCBIFace->setPhaseNodeInfo(PhaseCalcInfo->m_sChannel, PhaseCalcInfo->m_sRange, 0, 0.0, 0.0); // wir setzen jeweils die 1. stützstelle und lassen im anschluss daran die koeffizienten berechnen
        AHS++;
        break; // PhaseNodeMeasCoefficientClearN

    case PhaseNodeMeasCoefficientClearN2:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            m_CalcInfoList.removeFirst();
            if (m_CalcInfoList.isEmpty())
            {
                if (m_pAbortButton->isEnabled())
                {
                    PCBIFace->cmpPhaseCoefficient("ch0"); // berechnung der koeffizienten
                    AHS++;
                }
                else
                {
                    m_ActTimer->start(0,RestartMeasurementStart); // die hatten wir gestoppt
                    AHS = wm3000Idle;
                    delete m_pProgressDialog;
                    m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
                }
            }
            else
            {
                AHS--;
                m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
            }
        }
        break;

    case PhaseNodeMeasCoefficientClearNFinished:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->cmpPhaseCoefficient("ch1"); // berechnung der koeffizienten
            AHS++;
        }
        break;

    case PhaseNodeMeasBaseConfiguration:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
            break;
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            NewConfData = m_ConfData; // zum umsetzen
            SaveConfData = m_ConfData; // wir haben eine kopie der aktuellen konfiguration
            NewConfData.m_bOECorrection = false; // nix korrigieren
            NewConfData.m_fxPhaseShift = 0.0; // auch hier keine korrekturen
            NewConfData.m_fxTimeShift = 0.0; // dito
            NewConfData.m_nSyncSource = Intern; // es muss intern synchronisiert sein
            NewConfData.m_nTSync = 1000; // 1 sec. ist ok
            NewConfData.m_nIntegrationTime = 1; // 1 sec.
            NewConfData.m_nMeasPeriod = 8; //
            NewConfData.m_bDCmeasurement = false;
            N = 0; // 1. stützstelle
            AHS++;
            // kein break wir laufen einfach drüber
        }

    case PhaseNodeMeasNodeConfig:
    {
        StopMeasurement(); // das kumuliert nur ....
        m_pProgressDialog->setLabelText (trUtf8("Konfiguration setzen ..." ));
        PhaseNodeMeasInfo = m_PhaseNodeMeasInfoList.first(); // info was zu tun ist

        if (m_PhaseJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
        {
             Q3TextStream stream( &m_PhaseJustLogfile );
             stream << QString("RangeN=%1 RangeX=%2 Mode=%3 ")
                                .arg(PhaseNodeMeasInfo->m_srngN)
                                .arg(PhaseNodeMeasInfo->m_srngX)
                                .arg(MModeName[PhaseNodeMeasInfo->m_nMMode]);
             stream << QString("SenseMode=%1 nS=").arg(SenseModeText[PhaseNodeMeasInfo->m_nSMode]);
             if (PhaseNodeMeasInfo->m_nnS == S80)
             stream << "80\n";
             else
             stream << "256\n";

             m_PhaseJustLogfile.flush();
             m_PhaseJustLogfile.close();
        }

        NewConfData.m_nSenseMode = PhaseNodeMeasInfo->m_nSMode; // sense modus
        NewConfData.m_nSRate = PhaseNodeMeasInfo->m_nnS; // samples / periode
        NewConfData.m_fSFreq = PhaseJustFreq[N];
        NewConfData.m_nMeasMode  =  PhaseNodeMeasInfo->m_nMMode;
        NewConfData.m_sRangeNVorgabe = PhaseNodeMeasInfo->m_srngN; // bereich kanal n
        if (PhaseNodeMeasInfo->m_nMMode == Un_UxAbs)
            NewConfData.m_sRangeXVorgabe = PhaseNodeMeasInfo->m_srngX; // bereich kanal x
        else
            NewConfData.m_sRangeEVTVorgabe = PhaseNodeMeasInfo->m_srngX; // bereich kanal x
        m_PhaseNodeMeasState = PhaseNodeMeasExec1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(PhaseJustSyncSlot()));
        SetConfDataSlot(&NewConfData); // und die neue konfiguration
        // die messung wird neu gestartet am ende der konfiguration
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // PhaseNodeMeasNodeConfig
    }

    case PhaseNodeMeasExec1: // konfiguriert ist
        NewConfData.m_sRangeNSoll = NewConfData.m_sRangeN =NewConfData.m_sRangeNVorgabe; // bereich kanal n
        NewConfData.m_sRangeXSoll = NewConfData.m_sRangeX = NewConfData.m_sRangeXVorgabe; // bereich kanal x
        NewConfData.m_sRangeEVTSoll = NewConfData.m_sRangeEVT = NewConfData.m_sRangeEVTVorgabe; // bereich kanal x
        m_PhaseNodeMeasState = PhaseNodeMeasExec2; // hier müssen wir später weitermachen
        mCount = PhaseNodeMeasInfo->m_nIgnore; // einschwingzeit setzen in messdurchläufen
        m_sJustText = trUtf8("Einschwingzeit läuft" );
        m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(PhaseJustSyncSlot()));
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // PhaseNodeMeasExec1

    case PhaseNodeMeasExec2:
        mCount--;
        m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        if (mCount == 0)
        { // eingeschwungen
            m_PhaseNodeMeasState = PhaseNodeMeasExec3; // ab jetzt messen wir wirklich
            mCount = PhaseNodeMeasInfo->m_nnMeas; // und setzen den zähler dafür
            switch (PhaseNodeMeasInfo->m_nJMode)
            { // geht anders .... aber ist so übersichtlicher
            case adcNPhase:
                m_sJustText = trUtf8("Messung Kanal N, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            case adcXPhase:
                m_sJustText = trUtf8("Messung Kanal X, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            case sensNadcXPhase:
                m_sJustText = trUtf8("Messung Kanal N, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            case sensXadcNPhase:
            case sensEVTadcNPhase:
                m_sJustText = trUtf8("Messung Kanal X, %1 läuft").arg(PhaseNodeMeasInfo->m_srngStore);
                break;
            default:
                break;
            }
            m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
            JustValueList.clear(); // phasenwinkel werte liste leeren, zur aufnahme der neuen messwerte
        }

        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(PhaseJustSyncSlot()));  // wieder neu verbinden
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; //	PhaseNodeMeasExec2

    case PhaseNodeMeasExec3:
    {
        int i;
        ph0 = ActValues.PHIN;
        ph1 = ActValues.PHIX;
        switch (PhaseNodeMeasInfo->m_nJMode)
        {
        case adcNPhase:
            break; // wir wollten den kanal n adc messen ->fertig
        case adcXPhase:
            ph0 = ph1;
            break; // wir wollten kanal x adc messen
        case sensNadcXPhase:
            ph0 = ph0 - ph1;
            break; // wir wollten kanal n sense messen
        case sensXadcNPhase:
        case sensEVTadcNPhase:
            ph0 = ph1 - ph0;
            break; // wir wollten kanal x bzw. evt sense messen
        default:
            break;
        }

        ph0*=57.295779;
        JustValueList.append(ph0); // wir schreiben den winkel wert in die liste

        mCount--;
        m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        if (mCount == 0)
        {
            m_pProgressDialog->setLabelText (trUtf8("Berechnung und Datenübertragung ..."));
            ph0 = 0.0;
            for (i = 0; i < JustValueList.count(); i++)
                ph0 -= JustValueList[i];

            ph0 /= JustValueList.count(); // der mittelwert aus den messungen
            AHS = PhaseNodeMeasExec4;
            m_ActTimer->start(0,wm3000Continue); // wir starten wieder selbst
        }
        else
        {
            QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(PhaseJustSyncSlot()));
            AHS = wm3000Idle; // wir werden von der nächsten messung getriggert
        }

        break; // PhaseNodeMeasExec3
    }

    case PhaseNodeMeasExec4:
    {
        switch (PhaseNodeMeasInfo->m_nJMode)
        {
        case adcNPhase:
        case sensNadcXPhase:
            PCBIFace->setPhaseNodeInfo("ch0", PhaseNodeMeasInfo->m_srngStore, N, ph0, PhaseJustFreq[N]);
            break;
        case adcXPhase:
        case sensXadcNPhase:
        case sensEVTadcNPhase:
            PCBIFace->setPhaseNodeInfo("ch1", PhaseNodeMeasInfo->m_srngStore, N, ph0, PhaseJustFreq[N]);
            break;
        default:
            break;
        }

        AHS++;
        break; // PhaseNodeMeasExec4
    }

    case PhaseNodeMeasExec5:
    {
    if (m_ConfData.m_bSimulation)
    {
        AHS = wm3000Idle;
    }
    else
    {
        N++;
        if (N < 4)
        {
            AHS = PhaseNodeMeasNodeConfig;
            m_ActTimer->start(0,wm3000Continue);
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            m_PhaseNodeMeasInfoList.removeFirst();
            if (m_PhaseNodeMeasInfoList.isEmpty() || (! m_pAbortButton->isEnabled()) || bOverload ) // entweder normal fertig geworden oder abbruch oder übersteuerung (solls eigentlich nicht geben)
            { // wir sind fertig mit der ermittlung
                if (m_PhaseJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                {
                    Q3TextStream stream( &m_PhaseJustLogfile );
                    stream << "\nTerminated ";
                    if (bOverload)
                        stream << "because of overload condition !\n";
                    else
                    {
                        if (m_pAbortButton->isEnabled())
                        stream << "normally\n";
                        else
                        stream << "by user\n";
                    }

                    m_PhaseJustLogfile.flush();
                    m_PhaseJustLogfile.close();
                }

                m_PhaseNodeMeasState = PhaseNodeMeasFinished;
                QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(PhaseJustSyncSlot()));
                SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
                AHS = wm3000Idle; // statemachine kann neu gestartet werden
            }
            else
            {
                if (m_PhaseJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                {
                    Q3TextStream stream( &m_PhaseJustLogfile );
                    stream << "\n"; // für jeden block eine leerzeile
                    m_PhaseJustLogfile.flush();
                    m_PhaseJustLogfile.close();
                }

                N = 0; // durchlaufzähler für ermittlung  der stützstellen
                AHS = PhaseNodeMeasNodeConfig;
                m_ActTimer->start(0,wm3000Continue);
            }
        }
    }
    break;
    } // PhaseNodeMeasExec5

    case PhaseNodeMeasFinished:
        delete m_pProgressDialog;
        JustagePhaseBerechnungSlot(); // berechnung noch starten
        AHS = wm3000Idle;
        break;

    case OffsetMeasWM3000Start:
    case OffsetMeasWM3000StartVar:
        m_OffsetJustLogfile.remove(); // beim starten wird das log file gelöscht
        StopMeasurement(); // die kumulieren jetzt nur
        m_pProgressDialog = new Q3ProgressDialog( trUtf8("Initialisierung..."), 0, m_OffsetMeasInfoList.count(), g_WMView, 0, FALSE, 0 ); // ein progress dialog 100% entspricht alle justierpunkte +1 für das 0 setzen der koeffizienten

        m_pAbortButton = new QPushButton(trUtf8("Abbruch"),0,0);
        m_pProgressDialog->setCancelButton(m_pAbortButton);
        m_pProgressDialog->setCaption(trUtf8("Offsetkorrekturen"));
        m_pProgressDialog->setMinimumDuration(0); // sofort sichtbar
        lprogress = 0; // int. progress counter
        QObject::connect(m_pAbortButton,SIGNAL(pressed()),this,SLOT(JustAbortSlot()));

        NewConfData = m_ConfData; // zum umsetzen
        SaveConfData = m_ConfData; // wir haben eine kopie der aktuellen konfiguration
        NewConfData.m_bOECorrection = false; // nix korrigieren
        NewConfData.m_fxPhaseShift = 0.0; // auch hier keine korrekturen
        NewConfData.m_fxTimeShift = 0.0; // dito
        NewConfData.m_nSyncSource = Intern; // es muss intern synchronisiert sein
        NewConfData.m_nTSync = 1000; // 1 sec. ist ok
        NewConfData.m_nIntegrationTime = 2; // 1 sec.
        NewConfData.m_nMeasPeriod = 16; //
        NewConfData.m_bDCmeasurement = true;

        m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
        AHS++;
        break;

    case OffsetMeasWM3000CoefficientClearN:
        OffsetCalcInfo = m_CalcInfoList.first();
        PCBIFace->setOffsetNodeInfo(OffsetCalcInfo->m_sChannel, OffsetCalcInfo->m_sRange, 0, 0.0, 0.0); // wir setzen jeweils die 1. stützstelle und lassen im anschluss daran die koeffizienten berechnen
        AHS++;
        break; // OffsetMeasCoefficientClearN

    case OffsetMeasWM3000CoefficientClearN2:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            m_CalcInfoList.removeFirst();
            if (m_CalcInfoList.isEmpty())
            {
                if (m_pAbortButton->isEnabled())
                {
                    PCBIFace->cmpOffsetCoefficient("ch0"); // berechnung der koeffizienten
                    AHS++;
                }
                else
                {
                    m_ActTimer->start(0,RestartMeasurementStart); // die hatten wir gestoppt
                    AHS = wm3000Idle;
                    delete m_pProgressDialog;
                    m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
                }
            }
            else
            {
                AHS--;
                m_ActTimer->start(0,wm3000Continue); // wir starten selbst damit es weiter geht
            }
        }
        break;

    case OffsetMeasWM3000CoefficientClearNFinished:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            PCBIFace->cmpOffsetCoefficient("ch1"); // berechnung der koeffizienten
            AHS++;
        }
        break;


    case OffsetMeasWM3000BaseConfiguration:
    case OffsetMeasWM3000BaseConfigurationVar:
    {
        StopMeasurement(); // das kumuliert nur ....
        m_pProgressDialog->setProgress(lprogress); // progess bar setzen
        m_pProgressDialog->setLabelText (trUtf8("Konfiguration setzen ..." ));
        OffsetMeasInfo = m_OffsetMeasInfoList.first(); // info was zu tun ist

        if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
        {
             Q3TextStream stream( &m_OffsetJustLogfile );

             stream << QString("RangeN=%1 RangeX=%2 Mode=%3 ")
                               .arg(OffsetMeasInfo->m_srngN)
                               .arg(OffsetMeasInfo->m_srngX)
                               .arg(MModeName[OffsetMeasInfo->m_nMMode]);
             stream << QString("SenseMode=%1 nS=").arg(SenseModeText[OffsetMeasInfo->m_nSMode]);
             if (OffsetMeasInfo->m_nnS == S80)
                stream << "80\n";
             else
                stream << "256\n";

             m_OffsetJustLogfile.flush();
             m_OffsetJustLogfile.close();
         }

         NewConfData.m_nSenseMode = OffsetMeasInfo->m_nSMode; // sense modus
         NewConfData.m_nSRate = OffsetMeasInfo->m_nnS; // samples / periode
         NewConfData.m_fSFreq = PhaseJustFreq[2]; // fix 50Hz
         NewConfData.m_nMeasMode  =  OffsetMeasInfo->m_nMMode;
         NewConfData.m_sRangeNVorgabe = OffsetMeasInfo->m_srngN; // bereich kanal n
         if (OffsetMeasInfo->m_nMMode == Un_UxAbs)
            NewConfData.m_sRangeXVorgabe = OffsetMeasInfo->m_srngX; // bereich kanal x
         else
            NewConfData.m_sRangeEVTVorgabe = OffsetMeasInfo->m_srngX; // bereich kanal x
         m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
         QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(OffsetJustSyncSlot()));
         SetConfDataSlot(&NewConfData); // und die neue konfiguration
         // die messung wird neu gestartet am ende der konfiguration
         AHS = wm3000Idle; // wir sind erst mal fertig
         break; // OffsetMeasBaseConfiguration OffsetMeasBaseConfigurationVar
    }

    case OffsetMeasWM3000Exec1: // konfiguriert ist
    case OffsetMeasWM3000Exec1Var:
        NewConfData.m_sRangeNSoll = NewConfData.m_sRangeN =NewConfData.m_sRangeNVorgabe; // bereich kanal n
        NewConfData.m_sRangeXSoll = NewConfData.m_sRangeX = NewConfData.m_sRangeXVorgabe; // bereich kanal x
        NewConfData.m_sRangeEVTSoll = NewConfData.m_sRangeEVT = NewConfData.m_sRangeEVTVorgabe; // bereich kanal x
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        mCount = OffsetMeasInfo->m_nIgnore; // einschwingzeit setzen in messdurchläufen
        m_sJustText = trUtf8("Einschwingzeit läuft" );
        m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // OffsetMeasExec1 OffsetMeasExec1Var

    case OffsetMeasWM3000Exec2:
    case OffsetMeasWM3000Exec2Var:
        mCount--;
        m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        if (mCount == 0)
        { // eingeschwungen
            m_OffsetMeasState = AHS + 1; // ab jetzt messen wir wirklich
            mCount = OffsetMeasInfo->m_nnMeas; // und setzen den zähler dafür
            switch (OffsetMeasInfo->m_nJMode)
            {
            case sensNsensXOffset:
                m_sJustText = trUtf8("Messung Kanal N %1, X %2 läuft").arg(OffsetMeasInfo->m_srngN).arg(OffsetMeasInfo->m_srngX);
                break;
            case sensNOffset:
                // für den fall dass messkanäle unterschiedliche bereiche haben, was aber z.z. nicht funktioniert
                m_sJustText = trUtf8("Messung Kanal N %1 läuft").arg(OffsetMeasInfo->m_srngN);
                break;
            case sensXOffset:
            case sensEVTOffset:
                // dito
                m_sJustText = trUtf8("Messung Kanal X %1 läuft").arg(OffsetMeasInfo->m_srngX);
                break;
            default:
                break;
            }
            m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
            JustValueList.clear(); // offset werte liste leeren, zur aufnahme der neuen messwerte
        }

        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));  // wieder neu verbinden
        AHS = wm3000Idle; // wir sind erst mal fertig
        break; // OffsetMeasExec2 OffsetMeasExec2Var

    case OffsetMeasWM3000Exec3:
    case OffsetMeasWM3000Exec3Var:
    {
        int i;

        JustValueList.append(ActValues.dspActValues.ampl1nf); // wir schreiben offset wert in die liste
        JustValueList.append(ActValues.dspActValues.ampl1xf); // dito

        mCount--;
        m_pProgressDialog->setLabelText (QString("%1 %2 ...").arg(m_sJustText).arg(mCount));
        if (mCount == 0)
        {
            m_pProgressDialog->setLabelText (trUtf8("Berechnung und Datenübertragung ..."));
            offs0 = 0.0;
            offs1 = 0.0;
            int n;

            n = JustValueList.count() >> 1;
            for (i = 0; i < n; i++)
            {
                offs0 -= JustValueList[i*2];
                offs1 -= JustValueList[i*2+1];
            }

            offs0 /= n; // der mittelwert aus den messungen
            offs1 /= n;

            AHS++; // wir laufen einfach drüber und kommen damit zur datenübertragung oder ablage in hash
            m_ActTimer->start(0,wm3000Continue);
        }
        else
        {
            QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
            AHS = wm3000Idle; // wir werden von der nächsten messung getriggert
        }

        break; // OffsetMeasExec3 OffsetMeasExec3Var
    }

    case OffsetMeasWM3000Exec4:
    {
        CWMRange* lr;
        QString sel;

        switch (OffsetMeasInfo->m_nJMode)
        {
        case sensNsensXOffset:
            PCBIFace->setOffsetNodeInfo("ch0", OffsetMeasInfo->m_srngN, 0, offs0, 1.0);
            AHS++; // wir müssen noch den 2. wert speichern
            break;
        case sensNOffset:
            PCBIFace->setOffsetNodeInfo("ch0", OffsetMeasInfo->m_srngN, 0, offs0, 1.0);
            AHS += 2; // wir überspringen das speicher des 2. wertes
            break;
        case sensXOffset:
            PCBIFace->setOffsetNodeInfo("ch1", OffsetMeasInfo->m_srngX, 0, offs1, 1.0);
            AHS += 2; // dito
            break;
        case sensEVTOffset:
            lr = Range(OffsetMeasInfo->m_srngX,m_sEVTRangeList);
            sel = lr->Selector();
            PCBIFace->setOffsetNodeInfo("ch1", sel, 0, offs1, 1.0);
            AHS += 2; // dito
            break;
        default:
            break;
        }

        break; // OffsetMeasExec4
    }

    case OffsetMeasWM3000Exec4Var:
    {
        // speichern der offset justage werte in hash
        QString key;
        switch (OffsetMeasInfo->m_nJMode)
        {
        case sensNsensXOffset:
            key = Range(OffsetMeasInfo->m_srngN, m_sNRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs0;
            key = Range(OffsetMeasInfo->m_srngX, m_sXRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs1;
            break;
        case sensNOffset:
            key = Range(OffsetMeasInfo->m_srngN, m_sNRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs0;
            break;
        case sensXOffset:
            key = Range(OffsetMeasInfo->m_srngX, m_sXRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs1;
            break;
        case sensEVTOffset:
            key = Range(OffsetMeasInfo->m_srngX, m_sEVTRangeList)->getOffsKorrKey();
            adjOffsetCorrectionHash.remove(key);
            adjOffsetCorrectionHash[key] = offs1;
            break;
        default:
            break;
        }

        AHS++;
        m_ActTimer->start(0,wm3000Continue);
        break;
    }

    case OffsetMeasWM3000Exec5:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            // wir speichern noch den 2. ermittelten wert
            PCBIFace->setOffsetNodeInfo("ch1", OffsetMeasInfo->m_srngX, 0, offs1, 1.0);
            AHS++;
        }

        break;

    case OffsetMeasWM3000Exec6:
        if (m_ConfData.m_bSimulation)  // fehler oder abbruch
        {
            AHS = wm3000Idle;
        }
        else
        {
            lprogress++;
            m_pProgressDialog->setProgress(lprogress);
            m_OffsetMeasInfoList.removeFirst();

            if (m_OffsetMeasInfoList.isEmpty() || (! m_pAbortButton->isEnabled()) || bOverload ) // entweder normal fertig geworden oder abbruch oder übersteuerung (solls eigentlich nicht geben)
            { // wir sind fertig mit der ermittlung
                if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                {
                    Q3TextStream stream( &m_OffsetJustLogfile );
                    stream << "\nTerminated ";
                    if (bOverload)
                        stream << "because of overload condition !\n";
                    else
                    {
                        if (m_pAbortButton->isEnabled())
                        stream << "normally\n";
                        else
                        stream << "by user\n";
                    }

                    m_OffsetJustLogfile.flush();
                    m_OffsetJustLogfile.close();
                }

                m_OffsetMeasState = AHS + 1; // OffsetMeasFinished;
                QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(OffsetJustSyncSlot()));
                SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
                AHS = wm3000Idle; // statemachine kann neu gestartet werden
            }
            else
            {
                if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
                {
                    Q3TextStream stream( &m_OffsetJustLogfile );
                    stream << "\n"; // für jeden block eine leerzeile
                    m_OffsetJustLogfile.flush();
                    m_OffsetJustLogfile.close();
                }

                AHS = OffsetMeasWM3000BaseConfiguration;
                m_ActTimer->start(0,wm3000Continue);
            }
        }

        break;


    case OffsetMeasWM3000Finished:
        delete m_pProgressDialog;
        JustageOffsetBerechnungSlot(); // berechnung noch starten
        AHS = wm3000Idle;
        break;


    case OffsetMeasWM3000Exec5Var:
        lprogress++;
        m_pProgressDialog->setProgress(lprogress);
        m_OffsetMeasInfoList.removeFirst();

        if (m_OffsetMeasInfoList.isEmpty() || (! m_pAbortButton->isEnabled()) || bOverload ) // entweder normal fertig geworden oder abbruch oder übersteuerung (solls eigentlich nicht geben)
        { // wir sind fertig mit der ermittlung
            if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
            {
                Q3TextStream stream( &m_OffsetJustLogfile );
                stream << "\nTerminated ";
                if (bOverload)
                    stream << "because of overload condition !\n";
                else
                {
                    if (m_pAbortButton->isEnabled())
                    stream << "normally\n";
                    else
                    stream << "by user\n";
                }

                m_OffsetJustLogfile.flush();
                m_OffsetJustLogfile.close();
            }

            m_OffsetMeasState = AHS + 1; // OffsetMeasFinishedVar;
            QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(OffsetJustSyncSlot()));
            SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
            AHS = wm3000Idle; // statemachine kann neu gestartet werden
        }
        else
        {
            if (m_OffsetJustLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) ) // wir loggen das mal
            {
                Q3TextStream stream( &m_OffsetJustLogfile );
                stream << "\n"; // für jeden block eine leerzeile
                m_OffsetJustLogfile.flush();
                m_OffsetJustLogfile.close();
            }

            AHS = OffsetMeasWM3000BaseConfigurationVar;
            m_ActTimer->start(0,wm3000Continue);
        }

        break;

    case OffsetMeasWM3000FinishedVar:
        delete m_pProgressDialog;
        measOffsetCorrectionHash = adjOffsetCorrectionHash;
        offsetCorrectionHash2File();
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelNStart:
        m_pProgressDialog = new Q3ProgressDialog( trUtf8("Messung..."), 0, 2, g_WMView, 0, FALSE, 0 );
        m_pProgressDialog->setCaption(trUtf8("Offsetmessung Kanal N"));
        m_pProgressDialog->setMinimumDuration(0); // sofort sichtbar
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelNSync:
    case OffsetMeasChannelXSync:
        m_pProgressDialog->setProgress(1);
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelNFinished:
        m_pProgressDialog->setProgress(2);
        offs0 = ActValues.VekNSek.re();
        if (m_ConfData.m_bOffsetCorrectionN)
            offs0 += m_JustValues.OffsetCorrDevN;
        m_JustValues.OffsetCorrDevN = offs0;
        emit OffsetValue(offs0);
        delete m_pProgressDialog;
        emit SendJustValuesSignal(&m_JustValues);
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelXStart:
        m_pProgressDialog = new Q3ProgressDialog( trUtf8("Messung..."), 0, 2, g_WMView, 0, FALSE, 0 );
        m_pProgressDialog->setCaption(trUtf8("Offsetmessung Kanal X"));
        m_pProgressDialog->setMinimumDuration(0); // sofort sichtbar
        m_OffsetMeasState = AHS + 1; // hier müssen wir später weitermachen
        QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(OffsetJustSyncSlot()));
        AHS = wm3000Idle;
        break;

    case OffsetMeasChannelXFinished:
        m_pProgressDialog->setProgress(2);
        offs0 = ActValues.VekXSek.re();
        if (m_ConfData.m_bOffsetCorrectionX)
            offs0 += m_JustValues.OffsetCorrDevX;
        m_JustValues.OffsetCorrDevX = offs0;
        emit OffsetValue(offs0);
        delete m_pProgressDialog;
        emit SendJustValuesSignal(&m_JustValues);
        AHS = wm3000Idle;
        break;

    case JustageFlashProgStart:
        PCBIFace->JustFlashProgram();
        AHS++;
        break;

    case JustageFlashProgFinished:
        AHS = wm3000Idle; // ob fehler oder nicht wir sind fertig
        break;

    case JustageFlashExportStart:
        PCBIFace->JustFlashExport(JDataFile);
        AHS++;
        break;

    case JustageFlashExportFinished:
    AHS = wm3000Idle; // ob fehler oder nicht wir sind fertig
    break;

    case JustageFlashImportStart:
    PCBIFace->JustFlashImport(JDataFile);
    AHS++;
    break;

    case JustageFlashImportFinished:
    AHS = wm3000Idle; // ob fehler oder nicht wir sind fertig
    break;

    case EN61850ReadStatusStart:
    if (DspIFace->connected())
    {
        DspIFace->DspMemoryRead(ETHStatusHandle); // holt die daten ins dsp interface
        AHS++;
    }
    else
    {
        AHS = wm3000Idle;
    }

    break;

    case EN61850ReadStatusFinished:
    if ( !DspIFace->IFaceError() ) // wenn kein fehler aufgetreten
    {
        cEN61850Info EnStat; // holen der werte
        quint32 *source = (quint32*) DspIFace->data(ETHStatusHandle);
        quint32 *dest = &(EnStat.ByteCount[0]);
        for (uint i=0; i< sizeof(EnStat)/sizeof(quint32);i++) *dest++ = *source++;
        emit EN61850StatusSignal(&EnStat); // und senden
    }

    AHS = wm3000Idle; // wir sind so oder so fertig
    break; // EN61850ReadStatusFinished

    case EN61850WriteStatusStart:
    DspIFace->DspMemoryWrite(ETHStatusResetHandle,dInt); // schreibt die daten in den dsp
    AHS++;
    break; // EN61850WriteStatusStart

    case EN61850WriteStatusFinished:
    AHS = wm3000Idle; // wir sind fertig...ob fehler oder nicht
    break; // EN61850WriteStatusFinished

    case SelftestStart:
    StopMeasurement(); // die kumulieren jetzt nur
    m_pProgressDialog = new Q3ProgressDialog( trUtf8("Selbstest ..."), 0, m_SelftestInfoList.count(), g_WMView, 0, FALSE, 0 ); // ein progress dialog 100% entspricht aller selbsttestpunkte
    if ( m_pAbortButton )
        m_pProgressDialog->setCancelButton(m_pAbortButton);
    m_pProgressDialog->setCaption(trUtf8("Selbsttest"));
    m_pProgressDialog->setMinimumDuration(0); // sofort sichtbar
    lprogress = 0; // int. progress counter
    m_pProgressDialog->setProgress(lprogress);
    QObject::connect(m_pAbortButton,SIGNAL(pressed()),this,SLOT(SelftestAbortSlot()));
    m_SelftestLogfile.remove(); // beim starten wird das log file gelöscht
    AHS++;
    m_ActTimer->start(0,wm3000Continue);
    break; // SelftestStart

    case SelftestBaseConfiguration:
    NewConfData = m_ConfData; // zum umsetzen
    SaveConfData = m_ConfData; // wir haben eine kopie der aktuellen konfiguration
    NewConfData.m_bOECorrection = false; // nix korrigieren
    NewConfData.m_fxPhaseShift = 0.0; // auch hier keine korrekturen
    NewConfData.m_fxTimeShift = 0.0; // dito
    NewConfData.m_nSyncSource = Intern; // es muss intern synchronisiert sein
    NewConfData.m_nTSync = 1000; // 1 sec. ist ok
    NewConfData.m_nIntegrationTime = 1; // 1 sec.
    NewConfData.m_nMeasPeriod = 16; // wir begrenzen auf 16 signal perioden ....
    NewConfData.m_fSFreq = 50.0; // alles bei 50 hz
    NewConfData.m_nSRate = S80;
    NewConfData.m_nMeasMode  =  Un_UxAbs;
    NewConfData.m_nSenseMode = sensNadcX; // wir starten mit messen n gegen adcx
    AHS++;

    // kein break wir laufen einfach drüber

    case SelftestMeasConfiguration:
    m_pProgressDialog->setLabelText (trUtf8("Konfiguration setzen ..." ));
    m_SelftestState = SelftestSetRangeNX; // hier müssen wir später weitermachen
    QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
    SetConfDataSlot(&NewConfData); // und die neue konfiguration
    AHS = wm3000Idle; // wir sind erst mal fertig
    break; // SelftestMeasConfiguration


    case SelftestSetRangeNX:
    m_pProgressDialog->setLabelText (trUtf8("Bereiche setzen ..." ));
    NewConfData.m_nSenseMode = sensNadcX; // wir starten mit messen n gegen adcx
    NewConfData.m_sRangeNVorgabe = NewConfData.m_sRangeXVorgabe = m_SelftestInfoList.first();
    m_SelftestState = SelftestMeasureNSync; // hier müssen wir später weitermachen
    QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
    SetConfDataSlot(&NewConfData); // und die neue konfiguration

    AHS = wm3000Idle;
    break; // SelftestSetRangeNX


    case SelftestMeasureNSync:
    NewConfData.m_sRangeNSoll = NewConfData.m_sRangeN =NewConfData.m_sRangeNVorgabe; // bereich kanal n
    NewConfData.m_sRangeXSoll = NewConfData.m_sRangeX = NewConfData.m_sRangeXVorgabe; // bereich kanal x

    m_pProgressDialog->setLabelText (trUtf8("Messung ..." ));
    m_SelftestState = SelftestMeasureN; // hier müssen wir später weitermachen
    QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(SelftestSyncSlot()));

    AHS = wm3000Idle; // ob fehler odernicht wir sind fertig
    break; // SelftestMeasureNSync

    case SelftestMeasureN: // wir haben aktuelle messwerte
    SenseVektor = ActValues.VekN;
    ADCVektor = ActValues.VekX; //   messwerte speichern
    m_pProgressDialog->setLabelText (trUtf8("Modus setzen ..." ));

    m_SelftestState = SelftestMeasureXSync; // hier müssen wir später weitermachen
    NewConfData.m_nSenseMode = sensXadcN; // als nächstes messen wir x gegen adcn
    QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
    SetConfDataSlot(&NewConfData); // und die neue konfiguration

    AHS = wm3000Idle;
    break; // SelftestMeasureN

    case SelftestMeasureXSync:
    m_pProgressDialog->setLabelText (trUtf8("Messung ..." ));
    m_SelftestState = SelftestMeasureX; // hier müssen wir später weitermachen
    QObject::connect(this,SIGNAL(MeasureReady()),this,SLOT(SelftestSyncSlot()));

    AHS = wm3000Idle;
    break; // SelftestMeasureXSync

    case SelftestMeasureX:
    {
        if (m_SelftestLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) )
        {
        Q3TextStream stream( &m_SelftestLogfile );
        stream << QString("Range=%1").arg(m_SelftestInfoList.first())
                   << QString("  N=(%1,%2)").arg(SenseVektor.re()).arg(SenseVektor.im())
                   << QString("  X=(%1,%2)").arg(ActValues.VekX.re()).arg(ActValues.VekX.im())
                   << QString("  ADCX=(%1,%2)").arg(ADCVektor.re()).arg(ADCVektor.im())
                   << QString("  ADCN=(%1,%2)").arg(ActValues.VekN.re()).arg(ActValues.VekN.im());
        m_SelftestLogfile.flush();
        m_SelftestLogfile.close();
        }

    // achtung komplexe division !!!!!!
    if (  ((fabs (1.0 - fabs(SenseVektor/ActValues.VekX))) < 0.01) &&
          ((fabs (1.0 - fabs(ADCVektor/ActValues.VekN))) < 0.01) )
    {

        if (m_SelftestLogfile.open( QIODevice::WriteOnly  | QIODevice::Append) )
        {
        Q3TextStream stream( &m_SelftestLogfile );
        stream << "  good\n";
        m_SelftestLogfile.flush();
        m_SelftestLogfile.close();
        }

        // unjustiert lassen wir 1% abweichung zu
        lprogress++; // int. progress counter
        m_pProgressDialog->setProgress(lprogress);

        // loggen

        // prüfen ob noch mehr zu testen ist
        m_SelftestInfoList.pop_front();
        if ( m_SelftestInfoList.empty() ) // wir sind fertig mit dem selbsttest
        {
        m_SelftestState = SelftestFinished;
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
        SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
        AHS = wm3000Idle; // statemachine kann neu gestartet werden
        emit SelftestDone(0);
        }
        else
        {
        AHS = SelftestSetRangeNX; // wir machen hier weiter
        m_ActTimer->start(0,wm3000Continue);
        }
    }
    else
    {
        // fehler loggen
        // selbst test stop

        if (m_SelftestLogfile.open( QIODevice::WriteOnly | QIODevice::Append) )
        {
        Q3TextStream stream( &m_SelftestLogfile );
        stream << "  bad\n";
        m_SelftestLogfile.flush();
        m_SelftestLogfile.close();
        SelftestDone(-1); // fehler
        }

        m_SelftestState = SelftestFinished;
        QObject::connect(this,SIGNAL(ConfigReady()),this,SLOT(SelftestSyncSlot()));
        SetConfDataSlot(&SaveConfData); // wir setzen die konfiguration zurück
        AHS = wm3000Idle;
    }

    break; // SelftestMeasureX
              }
    case SelftestFinished:
        delete m_pProgressDialog; // progress dialog schliessen
        m_SelftestMsgBox->show();
        AHS = wm3000Idle;
        break; // SelftestFinished

    default: break;

    } //  switch (AHS)

    if (AHS == wm3000Idle)  // wenn wir fertig sind
    if ( !AHSFifo.empty() ) { // und das kommando fifo nicht leer ist
    int state;
    state = AHSFifo.first();
    AHSFifo.pop_front();
    m_ActTimer->start(0,state); // starten wir selbst das neue kommando
    }
}


void cWM3000U::ServerIFaceErrorHandling(int error, QString host, int port)
{
    int userRM;
    bool binitdone = m_binitDone; // wir merken uns den init. zustand
    m_binitDone = false; // so laufen keine weiteren ereignisse auf

    emit AffectStatus(SetQuestStat, QuestServerProblems); // questionable status setzen

    if (error & (myErrConnectionRefused | myErrHostNotFound | myErrSocketConnectionTimeOut) )
    {
    QString m = tr("Keine Verbindung zu %1:%2\n") .arg(host).arg(port);
    if (error & ! myErrHostNotFound)
        m+=tr("Host nicht gefunden.\n");
        else
        m+=tr("Host gefunden. Keine Verbindung zu Server.\n");

        m+=tr("Das Programm kann ohne Server nur\n"
        "im Simulations Modus betrieben werden.\n");

        userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),m,
                              tr("Programm&Abbruch"),
                    tr("&Wiederholen"),
                    tr("&Simulation"),
                    1,-1 );
    }

    else if (error & myErrSocketWrite) {
    userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),
                     tr("Fehler beim Schreiben von Daten\n"
                    "für %1:%2 .\n"
                    "Details stehen in LogFile.").arg(host) .arg(port),
                     tr("Programm&Abbruch"),
                     tr("&Wiederholen"),
                     tr("&Simulation"),
                     1,-1 );
    }

     else if (error & (myErrSocketUnexpectedAnswer | myErrSocketReadTimeOut) ) {
    userRM =  QMessageBox::warning( 0, tr("TCP-Verbindung"),
                    tr("Unerwartete Antwort beim Lesen\n"
                       "von %1:%2 erhalten.\n"
                       "Details stehen in LogFile.").arg(host).arg(port),
                    tr("Programm&Abbruch"),
                    tr("&Wiederholen"),
                    tr("&Simulation"),1,-1 );
    }

    else if (error & myErrDeviceBusy) {
    userRM = QMessageBox::warning( 0, tr("TCP-Verbindung"),
                     tr("Device ist busy\n"
                    "( %1:%2 ).\n"
                    "Details stehen in LogFile.").arg(host) .arg(port),
                     tr("Programm&Abbruch"),
                     tr("&Wiederholen"),
                     tr("&Simulation"),
                     1,-1 );
    }

    else
    {
    qDebug("Socket Error unknown\n");
    return;
    }

    switch (userRM) {
    case 0 :
    emit AbortProgramSignal(); // benutzer hat programm abruch gewählt
    break;
    case 1 :
    m_ActTimer->start(0,wm3000Repeat); // wir wiederholen den versuch
    m_binitDone = binitdone; // wir setzen init. zustand zurück
    break;
    case 2 :
    m_ActTimer->start(0,EnterSimulationMode);
    break;
    }
}


void cWM3000U::InitWM3000()
{
    if (m_bConventional)
        SerialVersions.DeviceName  = tr("Wandlermesseinrichtung WM1000U");
    else
        SerialVersions.DeviceName  = tr("Wandlermesseinrichtung WM3000U");

    float f;
    switch (m_ConfData.m_nSFreq) {  // wir setzen den realen frequenzwert
      case F16: f = 50.0/3;break;
      case F50: f = 50.0;break;
      case F60: f = 60.0;
      }

    m_ConfData.m_fSFreq = f;
    readOffsetCorrectionFile();
    emit SendConfDataSignal(&m_ConfData); // wir senden die konfiguration an alle ...
    m_ActTimer->start(0,InitializationStart);
}


void cWM3000U::setConventional(bool b)
{
    m_bConventional = b;
}


bool cWM3000U::isConventional()
{
    return m_bConventional;
}


void cWM3000U::setDC(bool b)
{
    m_bDC = b;
    //m_ConfData.m_bDCmeasurement = b;
}


bool cWM3000U::isDC()
{
    return m_bDC;
}

//------------------------------------------- ab hier stehen alle SLOTs--------------------------------------------------------

void cWM3000U::GetOETAnalizeDone(void)
{
    m_ConfData.m_bOENewLoad = false;
    emit SendConfDialogInfoSignal(m_pOwnError->GetNPrimList(),m_pOwnError->GetNSekList());
    // daten werden von cOwnError gelesen, gehalten, zur verfügung gestellt
}


void cWM3000U::JustageAmplitudeSlot(void)
{
    QMessageBox::information( 0, tr("Amplituden Justage"),tr("Noch nicht implementiert"),QMessageBox::Ok);
}


void cWM3000U::JustagePhaseSlot(void)
{
    SetPhaseCalcInfo(); // zum löschen der koeffizienten
    SetPhaseNodeMeasInfo(); // zum ermitteln der nodes
    emit StartStateMachine(PhaseNodeMeasStart);
}


void cWM3000U::JustagePhaseBerechnungSlot(void)
{
    SetPhaseCalcInfo();
    emit StartStateMachine(CmpPhaseCoeffStart);
}


void cWM3000U::JustageFlashProgSlot(void)
{
    emit StartStateMachine(JustageFlashProgStart);
}


void cWM3000U::JustageFlashExportSlot(QString s)
{
    QFileInfo fi( s );
    s.replace("."+fi.extension(false),"");
    JDataFile = s;
    emit StartStateMachine(JustageFlashExportStart);
}


void cWM3000U::JustageFlashImportSlot(QString s)
{
    QFileInfo fi( s );
    s.replace("."+fi.extension(false),"");
    JDataFile = s;
    emit StartStateMachine(JustageFlashImportStart);
}


void cWM3000U::JustageOffsetSlot()
{
    SetOffsetCalcInfo();
    SetOffsetMeasInfo(4,20); // zum ermitteln der offsetkorrekturen
    emit StartStateMachine(OffsetMeasWM3000Start);
}


void cWM3000U::JustageOffsetVarSlot()
{
    SetOffsetMeasInfo(2,4);
    emit StartStateMachine(OffsetMeasWM3000StartVar);
}


void cWM3000U::JustageOffsetBerechnungSlot()
{
    SetOffsetCalcInfo();
    emit StartStateMachine(CmpOffsetCoeffStart);
}


void cWM3000U::OffsetMessungChannelNSlot()
{
    emit StartStateMachine(OffsetMeasChannelNStart);
}


void cWM3000U::OffsetMessungChannelXSlot()
{
    emit StartStateMachine(OffsetMeasChannelXStart);
}


void cWM3000U::SelfTestManuell()
{
    SetSelfTestInfo(false);
    m_pAbortButton = new QPushButton(trUtf8("Abbruch"),0,0);
    emit StartStateMachine(SelftestStart);
}


void cWM3000U::SelfTestRemote(void)
{
    SetSelfTestInfo(true);
    m_pAbortButton = 0; // kein abbruch möglich
    emit StartStateMachine(SelftestStart);
}


void cWM3000U::OffsetMessungChannelNRemote()
{
    emit StartStateMachine(OffsetMeasChannelNStart);
}


void cWM3000U::OffsetMessungChannelXRemote()
{
    emit StartStateMachine(OffsetMeasChannelXStart);
}


void cWM3000U::EN61850ResetStatusSlot()
{
    emit StartStateMachine(EN61850WriteStatusStart);
}


void cWM3000U::EN61850InfoRequestSlot()
{
    emit StartStateMachine(EN61850ReadStatusStart);
}


void cWM3000U::DefaultSettingsSlot()
{
    cConfData tmpConfData;
    DefaultSettings(tmpConfData);
    SetConfDataSlot(&tmpConfData);
}


void cWM3000U::DefaultSettingsMeasurementSlot() // wird nach *rst aufgerufen
{
    DefaultSettingsMeasurement(m_ConfData);
    emit AffectStatus(ResetQuestStat, QuestOverLoadMax );
    m_ActTimer->start(0,SenseProtectionOff);
    m_ActTimer->start(10,InitializationStart);
    m_OVLMsgBox->hide(); // falls die überlast message box offen ist schliessen wir sie
}


void cWM3000U::DefaultSettings(cConfData& cdata) // alle einstellungen default
{
    cdata.setConfVersion();
    cdata.m_bRunning = true; // läuft oder läuft nicht
    cdata.m_bSimulation = false;
    cdata.Language = de; // default deutsch

    DefaultSettingsMeasurement(cdata);

    cdata.m_sOETFile = "";
    cdata.m_sResultFile = "";
    cdata.m_sADSPFile = "/opt/zera/bin/zdsp21262.ldr"; // dsp boot file
    cdata.m_bOENewLoad = false;
    cdata.m_nLogFileMax = 8192;

    cdata.m_sRangeN = m_ConfData.m_sRangeNVorgabe = m_ConfData.m_sRangeNSoll  = "480V";
    cdata.m_sRangeX = m_ConfData.m_sRangeXVorgabe = m_ConfData.m_sRangeXSoll  = "480V";
    cdata.m_sRangeEVT = m_ConfData.m_sRangeEVTVorgabe = m_ConfData.m_sRangeEVTSoll = "15.0V";

    cdata.FirstASDU = 1;
    cdata.LastASDU = 1;
    cdata.DataSet = 1;

    for(int i = 0; i < 6; i++) // default mac adressen
    {
    cdata.m_MacSourceAdr.MacAdrByte[i] = 58;
    cdata.m_MacDestAdr.MacAdrByte[i] = 59;
    }

    cdata.m_nPriorityTagged = (0x8100 << 16) + 0x8000; // TPID + UserPriority + CFI + VID
    cdata.m_nEthTypeHeader = (0x88BA << 16) + 0x4000; // Ethertype + APPID
    cdata.m_bStrongEthSynchronisation = false;

    // ende default konf. setzen
}


void cWM3000U::DefaultSettingsMeasurement(cConfData& cdata) // alle mess einstellungen default
{
    cdata.m_bOECorrection = false;
    cdata.m_bDCmeasurement = false;
    cdata.m_bOffsetCorrectionN = false;
    cdata.m_bOffsetCorrectionX = false;
    cdata.m_nMeasMode = Un_UxAbs;
    cdata.m_fxPhaseShift = 0.0;
    cdata.m_fxTimeShift = 0.0;
    cdata.m_nMeasPeriod = 16; // 16 perioden
    cdata.m_nIntegrationTime = 1; // 1 sec.
    cdata.m_nSFreq = F50;
    cdata.m_nSRate = S80;
    cdata.m_nSenseMode = sensNsensX; // normale messung
    cdata.m_nSyncSource = Intern;
    cdata.m_nTSync = 1000;

    cdata.m_NPrimary = "100V";
    cdata.m_NSecondary = "100V";
    cdata.m_XPrimary = "100V";
    cdata.m_XSecondary = "100V";
    cdata.m_EVTPrimary = "100V";
    cdata.m_EVTSecondary = "15.0V";
}


void cWM3000U::PhaseJustSyncSlot()
{
    QObject::disconnect(this,0,this,SLOT(PhaseJustSyncSlot()));
    emit StartStateMachine(m_PhaseNodeMeasState); // bei der justage weitermachen
}


void cWM3000U::OffsetJustSyncSlot()
{
    QObject::disconnect(this,0,this,SLOT(OffsetJustSyncSlot()));
    emit StartStateMachine(m_OffsetMeasState); // bei der justage weitermachen
}


void cWM3000U::JustAbortSlot()
{
    m_pAbortButton->setEnabled(false);
/* abbbruch in state machine behandelt
    while (m_PhaseNodeMeasInfoList.count() > 1)
    m_PhaseNodeMeasInfoList.removeLast();
*/
}


void cWM3000U::SelftestSyncSlot()
{
    QObject::disconnect(this,0,this,SLOT(SelftestSyncSlot()));
    emit StartStateMachine(m_SelftestState); // selbsttest weiter durchführen
}

void cWM3000U::SelftestAbortSlot()
{
    m_pAbortButton->setEnabled(false);
    while (m_SelftestInfoList.count() > 1)
    m_SelftestInfoList.pop_back();
}


void cWM3000U::OverLoadMaxQuitSlot()
{
    emit AffectStatus(ResetQuestStat, QuestOverLoadMax );
    emit StartStateMachine(SenseProtectionOff);
    AHSFifo.remove(RangeObsermaticStart); // alle events zur bereichüberwachung löschen
}


void cWM3000U::SetPhaseCalcInfo() // wir init. die liste damit die statemachine weiß was zu tun ist
{
    QString chn;
    m_CalcInfoList.clear();
    m_CalcInfoList.setAutoDelete( TRUE );
    chn = "ch0";
    // ad-wandler abgleich findet nicht mehr statt
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw80"));
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw256"));

    // jetzt doch wieder .....
    // wir benötigen phasenkorrekturwerte in abhängigkeit von der adwandler samplerate
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.16"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW80.60"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.16"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.50"));
    m_CalcInfoList.append(new cCalcInfo(chn,"ADW256.60"));

    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
    m_CalcInfoList.append(new cCalcInfo(chn, m_sNRangeList.at(i)->Selector()));

    chn = "ch1";
    // in ch1 (X) gibt es die bereiche nicht mehr .... die korrekturen sind für beide gleich.
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw80"));
    // m_CalcInfoList.append(new cCalcInfo(chn,"adw256"));

    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
    m_CalcInfoList.append(new cCalcInfo(chn, m_sNRangeList.at(i)->Selector()));

    for (uint i = 0; i < m_sEVTRangeList.count()-1; i++)
    m_CalcInfoList.append(new cCalcInfo(chn, m_sEVTRangeList.at(i)->Selector()));
}


void cWM3000U::SetPhaseNodeMeasInfo() // wir init. die liste damit die statemachine weiß was zu tun ist
{
    m_PhaseNodeMeasInfoList.clear();
    m_PhaseNodeMeasInfoList.setAutoDelete( TRUE );

/* wir gleichen die ad-wandler nicht mehr ab
    // zuerst die adwandler abgleichen
    m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcNadcX, Un_UxAbs, S80, 4, 10)); // bereiche optimal für hw freq messung, modus adc/adc, für 80 samples/periode und 2 messungen einschwingzeit, 6 messungen für stützstellenermittlung
    m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcNadcX, Un_UxAbs, S256, 4, 10));
    m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcXadcN, Un_UxAbs, S80, 4, 10)); // bereiche optimal für hw freq messung, modus adc/adc, für 80 samples/periode und 2 messungen einschwingzeit, 6 messungen für stützstellenermittlung
    m_PhaseNodeMeasInfoList.append(new cPhaseNodeMeasInfo( "3.75V", "3.75V", adcXadcN, Un_UxAbs, S256, 4, 10));
*/
    // jetzt doch wieder
    // aber nur für kanal ch0 bzw. N
    m_PhaseNodeMeasInfoList.append(new cJustMeasInfo( "3.75V", "3.75V", "ADW80.50", adcNadcX, Un_UxAbs, adcNPhase, S80, 4, 20)); // bereiche optimal für hw freq messung, modus adc/adc, für 80 samples/periode und 4 messungen einschwingzeit, 10 messungen für stützstellenermittlung
    m_PhaseNodeMeasInfoList.append(new cJustMeasInfo( "3.75V", "3.75V", "ADW256.50", adcNadcX, Un_UxAbs, adcNPhase, S256, 4, 20));

    // die liste für alle konv. bereiche in kanal n
    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
    m_PhaseNodeMeasInfoList.append(new cJustMeasInfo( m_sNRangeList.at(i)->Name(), "3.75V", m_sNRangeList.at(i)->Name(), sensNadcX, Un_UxAbs, sensNadcXPhase, S80, 4, 20));

    // die liste für alle konv. bereiche in kanal x
    for (uint i = 0; i < m_sXRangeList.count()-1; i++)
    m_PhaseNodeMeasInfoList.append(new cJustMeasInfo("3.75V", m_sXRangeList.at(i)->Name(), m_sXRangeList.at(i)->Name(), sensXadcN, Un_UxAbs, sensXadcNPhase, S80, 4, 20));

    // + die liste der evt bereiche in kanal x
    for (uint i = 0; i < m_sEVTRangeList.count()-1; i++) // i = 0 wäre der safety range.... jetzt nicht mehr
    m_PhaseNodeMeasInfoList.append(new cJustMeasInfo("3.75V", m_sEVTRangeList.at(i)->Name(), m_sEVTRangeList.at(i)->Selector(), sensXadcN, Un_EVT, sensEVTadcNPhase, S80, 4, 20));

}


void cWM3000U::SetOffsetCalcInfo()
{
    QString chn;
    m_CalcInfoList.clear();
    m_CalcInfoList.setAutoDelete( TRUE );
    chn = "ch0";
    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
    m_CalcInfoList.append(new cCalcInfo(chn, m_sNRangeList.at(i)->Selector()));
    chn = "ch1";
    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
    m_CalcInfoList.append(new cCalcInfo(chn, m_sNRangeList.at(i)->Selector()));
    for (uint i = 0; i < m_sEVTRangeList.count()-1; i++)
    m_CalcInfoList.append(new cCalcInfo(chn, m_sEVTRangeList.at(i)->Selector()));
}


void cWM3000U::SetOffsetMeasInfo(int te, int tm)
{
    QString key;

    m_OffsetMeasInfoList.clear();
    m_OffsetMeasInfoList.setAutoDelete(true);

    adjOffsetCorrectionHash.clear();
    measOffsetCorrectionHash.clear();

    for (uint i = 0; i < m_sNRangeList.count()-1; i++)
    {
        key = m_sNRangeList.at(i)->getOffsKorrKey();
        if (! adjOffsetCorrectionHash.contains(key))
        {
            // für diesen bereich haben wir noch keinen eintrag in die liste , was zu tun ist
            adjOffsetCorrectionHash[key] = 0.0; // platzhalter
            m_OffsetMeasInfoList.append(new cJustMeasInfo(m_sNRangeList.at(i)->Name(), m_sXRangeList.at(0)->Name(), m_sNRangeList.at(i)->Name(), sensNsensX0V, Un_UxAbs, sensNOffset, S80, te, tm));
        }
    }

    for (uint i = 0, j = adjOffsetCorrectionHash.count(); i < m_sXRangeList.count()-1; i++ )
    {
        key = m_sXRangeList.at(i)->getOffsKorrKey();
        if (!adjOffsetCorrectionHash.contains(key))
        {
            // für diesen bereich haben wir noch keine eintrag in die liste , was zu tun ist
            adjOffsetCorrectionHash[key] = 0.0; // platzhalter
            if (j > 0)
            {
                // wir nehmen einen bereits vorhanden eintrag in der offset mess liste
                cJustMeasInfo *jmi;
                jmi = m_OffsetMeasInfoList.at(i);
                jmi->m_nJMode = sensNsensXOffset;
                jmi->m_srngX = m_sXRangeList.at(i)->Name();
                j--;
            }
            else
                m_OffsetMeasInfoList.append(new cJustMeasInfo(m_sNRangeList.at(0)->Name(), m_sXRangeList.at(i)->Name(), m_sXRangeList.at(i)->Name(), sensNsensX0V, Un_UxAbs, sensXOffset, S80, te, tm));
        }
    }

    for (uint i = 0; i < m_sEVTRangeList.count()-1; i++)
    {
        key = m_sEVTRangeList.at(i)->getOffsKorrKey();
        if (! adjOffsetCorrectionHash.contains(key))
        {
            // für diesen bereich haben wir noch keine eintrag in die liste , was zu tun ist
            adjOffsetCorrectionHash[key] = 0.0; // platzhalter
            m_OffsetMeasInfoList.append(new cJustMeasInfo(m_sNRangeList.at(0)->Name(), m_sEVTRangeList.at(i)->Name(), m_sEVTRangeList.at(i)->Name(), sensNsensX0V, Un_EVT, sensEVTOffset, S80, te, tm));
        }
    }
}


void cWM3000U::offsetCorrectionHash2File()
{
    m_OffsetDatafile.remove();
    m_NSAOffsetDatafile.remove();

    if (m_OffsetDatafile.open(QIODevice::WriteOnly))
    {
        QDataStream st(&m_OffsetDatafile);
        st.setVersion(QDataStream::Qt_4_0);

        QList<QString> keylist;
        keylist = measOffsetCorrectionHash.keys();

        for (int i = 0; i < keylist.count(); i++)
        {
            QString key;

            key = keylist.at(i);
            st << key << measOffsetCorrectionHash[key];
        }
        m_OffsetDatafile.flush();
        m_OffsetDatafile.close();
    }

    if (m_NSAOffsetDatafile.open(QIODevice::WriteOnly))
    {
        QTextStream st(&m_NSAOffsetDatafile);

        QList<QString> keylist;
        keylist = measOffsetCorrectionHash.keys();

        for (int i = 0; i < keylist.count(); i++)
        {
            QString key;
            QString output;

            key = keylist.at(i);
            output = QString("%1;%2\n").arg(key).arg(measOffsetCorrectionHash[key]);
            st << output;
        }

        m_NSAOffsetDatafile.flush();
        m_NSAOffsetDatafile.close();
    }
}


void cWM3000U::readOffsetCorrectionFile()
{
    adjOffsetCorrectionHash.clear();
    measOffsetCorrectionHash.clear();

    if (m_OffsetDatafile.open(QIODevice::ReadOnly))
    {
        QString key;
        double val;
        QDataStream st(&m_OffsetDatafile);
        st.setVersion(QDataStream::Qt_4_0);
        while (!m_OffsetDatafile.atEnd())
        {
            st >> key >> val;
            adjOffsetCorrectionHash[key] = val;
        }

        measOffsetCorrectionHash = adjOffsetCorrectionHash;
    }
}


void cWM3000U::SetSelfTestInfo(bool remote)
{
    uint i;

    m_SelftestInfoList.clear();
    for (i = 0; i < m_sNRangeList.count()-1; i++)
    {
    m_SelftestInfoList.append(m_sNRangeList.at(i)->Selector());
    if (remote) break;
    }

    // wir testen nur die nx bereiche beim selbst test .... zu kompliziert die evt´s mit zu testen

    /*
    for (i = 0; i < m_sEVTRangeList.count()-1; i++)
    {
    m_SelftestInfoList.append(m_sEVTRangeList.at(i)->Selector());
    if (remote) break;
    }
    */
}


int cWM3000U::signum(double value)
{
    return (value>0)?1:((value<0)?-1:0);
}


void cWM3000U::SetConfDataSlot(cConfData *cd) // signal kommt vom konfigurations dialog oder aus interface
{ // oder aus statemachine
    // we limit the number of
    if ((cd->m_nSRate == S80) && (cd->m_nMeasPeriod > nmaxS80MeasPeriod))
        cd->m_nMeasPeriod = nmaxS80MeasPeriod;
    if ((cd->m_nSRate == S256) && (cd->m_nMeasPeriod > nmaxS256MeasPeriod))
        cd->m_nMeasPeriod = nmaxS256MeasPeriod;

    m_ConfDataCopy = m_ConfData; // alte konfiguration
    m_ConfData = *cd;
    emit SendConfDataSignal(&m_ConfData); // die anderen auch informieren
    emit StartStateMachine(ConfigurationStart);
}


void cWM3000U::SetRangeSlot(cConfData *cd)
{
    m_ConfData = *cd;
    emit StartStateMachine(SetRangeStart);
}


void cWM3000U::MeasureSlot()
{
    if (m_ConfData.m_bRunning)
    {
    if (m_ConfData.m_bSimulation) {
        SimulatedMeasurement();
    }
    else
    {
        if (m_binitDone)
        emit StartStateMachine(TriggerMeasureStart);
    }
    }
}


void cWM3000U::MeasureLPSlot()
{
    if (m_ConfData.m_bRunning)
    {
    if ( !m_ConfData.m_bSimulation && m_binitDone)
        emit StartStateMachine(MeasureLPStart); // wir holen frmsn und berechnen den lastpunkt
    }
}



void cWM3000U::RangeObsermaticSlot()
{
    if (m_binitDone)
    emit StartStateMachine(RangeObsermaticStart);
}


void cWM3000U::DspIFaceAsyncDataSlot(const QString& s) // für asynchrone meldungen vom dsp server
{
    QString sintnr;
    sintnr = s.section( ':', 1, 1);

    int service = sintnr.toInt();
    switch (service)
    {
    case 1:
      m_AsyncTimer->start(0,MeasureStart); // starten der statemachine für messwert aufnahme
      break;
    case 3:
      m_MovingWindowFilter.setFilterLength(m_ConfData.m_nIntegrationTime);
      MeasureTimer->start(1*1000); //  ab movingwindowfilter immer 1*1000 msec
      MeasureLPTimer->start(500);
      RangeTimer->start(500);
      m_bStopped = false;
      break;
    }
}


void cWM3000U::XIFaceDoneSlot() // wenn gültige daten da sind vom einen oder anderen server
{
    m_ActTimer->start(0,wm3000Continue); // starten der statemachine um weiter zu machen
}


void cWM3000U::DspIFaceErrorSlot() // wenn fehler aufgetreten sind am dsp server
{
    int error =  DspIFace->iFaceSock->GetError(); // erstmal fehler abholen
    DspIFace->iFaceSock->ResetError();
    ServerIFaceErrorHandling(error, TCPConfig.dspHost, TCPConfig.dspPort);
}


void cWM3000U::pcbIFaceErrorSlot() // dito für leitenkarten server
{
    int error =  PCBIFace->iFaceSock->GetError(); // erstmal fehler abholen
    PCBIFace->iFaceSock->ResetError();
    ServerIFaceErrorHandling(error, TCPConfig.pcbHost, TCPConfig.pcbPort);
}


bool cWM3000U::LoadSettings(QString session)
{
    bool ret;
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/wm3000u%2").arg(wm3000uHome).arg(fi.fileName());
    QFile file(ls);
    if ((ret = file.open( QIODevice::ReadOnly ) )) {
    QDataStream stream(&file);
    ret &= m_ConfData.deserialize(stream);
    file.close();
    }
    emit SendConfDataSignal(&m_ConfData); // neue conf. noch senden
    return ret;
}


void cWM3000U::WriteSettings(QString session)
{
    QFileInfo fi(session);
    QString ls = QString("%1/.wm3000u/wm3000u%2").arg(wm3000uHome).arg(fi.fileName());
    QFile file(ls);
//    file.remove();
    if ( file.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) ) {
    file.at(0);
    QDataStream stream(&file);
    m_ConfData.serialize(stream);
    file.close();
    }
}


void cWM3000U::StoreResultsSlot()
{
    if (m_ConfData.m_bRunning) { // nur wenn messung läuft speichern
    bool ok;
    QDomDocument resultDoc("WM3000ResultData");
    QFile rfile(m_ConfData.m_sResultFile);
    if (rfile.open( QIODevice::ReadOnly ) ) { // ? xml file lesen
        ok = resultDoc.setContent(&rfile);
        rfile.close();
    }

    QDomDocumentType TheDocType = resultDoc.doctype ();

    if  (!ok || ( TheDocType.name() != "WM3000ResultData")) {
        QDomDocument tmpDoc("WM3000ResultData");

        QDomElement rootTag;
        rootTag = tmpDoc.createElement( "RESULTEXPORT" );
        tmpDoc.appendChild( rootTag );

        QDomElement deviceTag = tmpDoc.createElement( "DEVICE" );
        rootTag.appendChild( deviceTag );

        QDomElement tag = tmpDoc.createElement( "Type" );
        deviceTag.appendChild( tag );
        QDomText t = tmpDoc.createTextNode( SerialVersions.DeviceName );
        tag.appendChild( t );

        tag = tmpDoc.createElement( "VersionNumber" );
        deviceTag.appendChild( tag );
        t = tmpDoc.createTextNode(SerialVersions.DeviceVersion );
        tag.appendChild( t );

        tag = tmpDoc.createElement( "SerialNumber" );
        deviceTag.appendChild( tag );
        t = tmpDoc.createTextNode( SerialVersions.PCBSerialNr );
        tag.appendChild( t );

        tag = tmpDoc.createElement( "DeviceStatus" );
        deviceTag.appendChild( tag );
        if (m_bJust)
        t = tmpDoc.createTextNode( "Justified" );
        else
        t = tmpDoc.createTextNode( "Not justified" );
        tag.appendChild( t );

        tag = tmpDoc.createElement( "RESULTLIST" );
        rootTag.appendChild( tag );

        QString s = tmpDoc.toString();
        resultDoc.setContent(tmpDoc.toString());
    }


    QDomElement docElem = resultDoc.documentElement();
    QDomNode n = docElem.lastChild();

    QDomElement resultTag = resultDoc.createElement( "RESULT" );
    n.appendChild( resultTag );

    QDomElement tag = resultDoc.createElement( "LoadPointN" );
    resultTag.appendChild( tag );
    QDomText t = resultDoc.createTextNode( QString("%1%;%2%").arg(ActValues.LoadPoint,7,'f',3).arg(ActValues.LoadPoint1,7,'f',3) ); // lastpunkt bezogen auf rms und rms grundschwingung
    tag.appendChild( t );

    tag = resultDoc.createElement( "LoadPointX" );
    resultTag.appendChild( tag );
    t = resultDoc.createTextNode( QString("%1%;%2%").arg(ActValues.LoadPointX,7,'f',3).arg(ActValues.LoadPoint1X,7,'f',3) ); // lastpunkt bezogen auf rms und rms grundschwingung
    tag.appendChild( t );

    tag = resultDoc.createElement( "AmplError" );
    resultTag.appendChild( tag );
    t = resultDoc.createTextNode( QString("%1%").arg(ActValues.AmplErrorIEC,7,'f',3) );
    tag.appendChild( t );

    tag = resultDoc.createElement( "AngleError" );
    resultTag.appendChild( tag );
        t = resultDoc.createTextNode( QString("%1rad").arg(ActValues.AngleError,8,'f',5));
    tag.appendChild( t );

    tag = resultDoc.createElement( "ANSIError" );
    resultTag.appendChild( tag );
    t = resultDoc.createTextNode( QString("%1%").arg(ActValues.AmplErrorANSI,7,'f',3) );
    tag.appendChild( t );

    tag = resultDoc.createElement( "Date" );
    resultTag.appendChild( tag );
    QDate d = QDate::currentDate();
    t = resultDoc.createTextNode(d.toString(Qt::TextDate));
    tag.appendChild( t );

    tag = resultDoc.createElement( "Time" );
    resultTag.appendChild( tag );
    QTime ti = QTime::currentTime();
    t = resultDoc.createTextNode(ti.toString(Qt::TextDate));
    tag.appendChild( t );

    QDomElement conditionsTag = resultDoc.createElement( "Conditions" );
    resultTag.appendChild( conditionsTag );

    tag = resultDoc.createElement( "Mode" );
    conditionsTag.appendChild( tag );
        char* modeName[3] = {(char*)"Un/Ux",(char*)"Un/EVT",(char*)"UN/nConvent"};
        char* simName[2] = {(char*)"real",(char*)"simulated"};
    t = resultDoc.createTextNode(QString("%1 %2").arg(QString(simName[(int)m_ConfData.m_bSimulation]))
                                            .arg(QString(modeName[m_ConfData.m_nMeasMode])));
    tag.appendChild( t );

    tag = resultDoc.createElement( "NRange" );
    conditionsTag.appendChild( tag );
    t = resultDoc.createTextNode(m_ConfData.m_sRangeN);
    tag.appendChild( t );

    tag = resultDoc.createElement( "XRange" );
    conditionsTag.appendChild( tag );

    QString s;
    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case Un_UxAbs:
        s = m_ConfData.m_sRangeX;
        break;
    case Un_EVT:
        s = m_ConfData.m_sRangeEVT;
        break;
    case Un_nConvent:
        s = "-----";
    }
    t = resultDoc.createTextNode(s);
    tag.appendChild( t );

    QDomElement uncorrTag  = resultDoc.createElement( "UNCorrection" );
    conditionsTag.appendChild( uncorrTag );

    tag = resultDoc.createElement( "Amplitude" );
    uncorrTag.appendChild( tag );
    t = resultDoc.createTextNode(QString("%1").arg(fabs(ActValues.UInCorr),7,'f',3));
    tag.appendChild( t );

    tag = resultDoc.createElement( "Angle" );
    uncorrTag.appendChild( tag );
    t = resultDoc.createTextNode(QString("%1").arg(UserAtan(ActValues.UInCorr.im(),ActValues.UInCorr.re()),7,'f',3));
    tag.appendChild( t );

    rfile.remove();
    if (rfile.open( QIODevice::WriteOnly ) ) {
        QString xml = resultDoc.toString();
        Q3TextStream stream( &rfile );
        stream << xml;
        rfile.close();
    }
    }
}


bool cWM3000U::SelectRange(cWMRangeList& RangeList, QString& sRange, QString& sRangeSoll,QString& sRangeVorgabe,float max, bool &ovl)
{
    CWMRange* prng = Range(sRange,RangeList); // zeiger auf akt. bereich
    if (max > (1.20 * 1.414 * prng->Rejection()) ) { // max überschritten -> größter bereich wird sollbereich
    ovl = true; // übersteuerung eintragen
    sRangeSoll = RangeList.first()->Name();
    if ( sRangeVorgabe != "Auto" ) sRangeVorgabe = sRangeSoll; // wir ändern auch den vorgabe wert falls wir keine automatik haben
    return (sRange != sRangeSoll); // muss bearbeitet werden
    }

    // const float LinearityLimit = 0.1;
    // kommt jetzt individuell aus bereichinfo
    // 10% linearitäts limit.... ist der wert oberhalb dieser grenze nehmen wir den wert, ansonsten nehmen wir die grenze
    if (sRangeVorgabe == "Auto") { // wir haben automatik
    ovl = false; // übersteuerung eintragen
    float minRValue = (max > (prng->LinearityLimit() * prng->Rejection()*1.414) ) ? (prng->Value() * max / (prng->Rejection() * 1.414)) : (prng->Value() * prng->LinearityLimit());
    //  der wert für den der bereich gesucht wird ist max. die linearitätsgrenze
    CWMRange* foundRange = Range(minRValue,RangeList);
    if ( foundRange->Value() < prng->Value() )
        sRangeSoll = foundRange->Name();
    return (sRange != sRangeSoll); // dito
    }

    return false;
}


CWMRange* cWM3000U::Range(cRSelectString selector,cWMRangeList &rlist)  // sucht den bereich in der liste zu selector
{
    Q3PtrListIterator<CWMRange> it(rlist);
    CWMRange *range;

    while ( (range = it.current()) != 0 ) {
    ++it;
    if ( selector == range->Selector() ) break;
    }
    if (range == 0)
    range = DummyRange;
    return range;
}


CWMRange* cWM3000U::Range(QString name,cWMRangeList &rlist) // sucht bereich in der liste zu name
{
    Q3PtrListIterator<CWMRange> it(rlist);
    CWMRange *range;

    while ( (range = it.current()) != 0 ) {
    ++it;
    if ( name == range->Name() ) break;
    }
    if (range == 0)
    range = DummyRange;
    return range;
}


CWMRange* cWM3000U::Range(float mw,cWMRangeList& rlist)
{
    Q3PtrListIterator<CWMRange> it(rlist);
    CWMRange *range;

    it.toLast(); // zeigt auf den "kleinsten bereich
    while ( (range = it.current()) != 0 ) {
    --it;
    if ( mw <= range->Value() ) break;
    }
    if (range == 0) range = rlist.first(); // der 1. bereich ist der grösste
    return range;
}


// habe die reihenfolge der variablen so geändert, dass wenn sich das samplingsystem ändert
// die indices und ergebnisse an den gleichen stellen bleiben
// alternative wäre gewesen generell das programm
void cWM3000U::SetDspWMVarList() // variablen des dsp zusammenbauen
{
    if (!m_ConfData.m_bSimulation) {
    int nSp = (m_ConfData.m_nSRate == S80) ? 80 :256;
    int nS = nSp * m_ConfData.m_nMeasPeriod;
    int schanLen = (m_ConfData.m_nMeasPeriod < 4)? 4:m_ConfData.m_nMeasPeriod;
    schanLen *= nSp;

    DspIFace->ClearVarLists();

    // maxima
    MaxValData = DspIFace->GetMVHandle(""); // wir holen uns ein handle für den maximumsucher
    DspIFace->addVarItem(MaxValData, new cDspVar("MAXN",1,vApplication | vDspIntern));
    DspIFace->addVarItem(MaxValData, new cDspVar("MAXX",1,vApplication | vDspIntern));
    DspIFace->addVarItem(MaxValData, new cDspVar("MAXRDY",1,vApplication | vDspIntern));

    // schnelle rms messung zur lastpunkt bestimmung 1x rms gesamtsignal 1x ampl 1. grundwelle
    RMSValData = DspIFace->GetMVHandle("");
    DspIFace->addVarItem(RMSValData, new cDspVar("FRMSN",1,vApplication | vDspIntern));
    DspIFace->addVarItem(RMSValData, new cDspVar("FAMPL1N",1,vApplication | vDspIntern));
    DspIFace->addVarItem(RMSValData, new cDspVar("FRMSX",1,vApplication | vDspIntern));
    DspIFace->addVarItem(RMSValData, new cDspVar("FAMPL1X",1,vApplication | vDspIntern));

    ActValData = DspIFace->GetMVHandle(""); // wir holen uns ein handle für die istwerte daten

//	nur dsp intern verwendete messdaten
    DspIFace->addVarItem(ActValData, new cDspVar("SINDEX",1,vDspIntern)); // index zur speicherung der sampledaten für die fehlermessung (variables messintervall);
    DspIFace->addVarItem(ActValData, new cDspVar("SINDEX2",1,vDspIntern)); // index zur speicherung der sampledaten für die schnelle lastpunktmessung (festes messintervall = 4 signalperioden);

    DspIFace->addVarItem(ActValData, new cDspVar("TEMP1",1,vDspIntern)); // werden nur temp. benötigt weil winkel und betrag
    DspIFace->addVarItem(ActValData, new cDspVar("TEMP2",1,vDspIntern)); // direkt ermittelt werden zwecks filterung

    // diese werte ab hier werden gefiltert
    DspIFace->addVarItem(ActValData, new cDspVar("KFKORR",1,vDspIntern)); // kreisfrequenz korrektur koeffizient
    DspIFace->addVarItem(ActValData, new cDspVar("RMSN",1,vDspIntern));
    DspIFace->addVarItem(ActValData, new cDspVar("AMPL1N",1,vDspIntern));
    DspIFace->addVarItem(ActValData,new cDspVar("RMSX",1,vDspIntern));
    DspIFace->addVarItem(ActValData, new cDspVar("AMPL1X",1,vDspIntern));
    DspIFace->addVarItem(ActValData, new cDspVar("DPHI",1,vDspIntern));
    // dphi=(phix-phin) - tdsync * (10*10^-9) * 2PI / (signalfreq* kfkorr)          							=(phix-phin) -tdsync * (2PI / (signalfreq*10^8)) * (1/kfkorr) -->

    DspIFace->addVarItem(ActValData, new cDspVar("FILTER",10,vDspIntern));
    // KFKORR wird separat gefiltert
    DspIFace->addVarItem(ActValData, new cDspVar("N",1,vDspIntern));

    // gefilterte messergebnisse
    DspIFace->addVarItem(ActValData, new cDspVar("KFKORRF",1,vApplication | vDspIntern)); // kreisfrequenz korrektur koeffizient
    DspIFace->addVarItem(ActValData, new cDspVar("RMSNF",1,vApplication | vDspIntern)); // rms wert kanal n
    DspIFace->addVarItem(ActValData, new cDspVar("AMPL1NF",1,vApplication | vDspIntern)); // amplitude 1. oberwelle kanal n
    DspIFace->addVarItem(ActValData, new cDspVar("RMSXF",1,vApplication | vDspIntern)); // rms wert kanal x
    DspIFace->addVarItem(ActValData, new cDspVar("AMPL1XF",1,vApplication | vDspIntern)); // amplitude 1. oberwelle kanal x
    DspIFace->addVarItem(ActValData, new cDspVar("DPHIF",1,vApplication | vDspIntern)); // winkel kanal x - winkel kanal n
    // nicht gefilterte messergebnisse
    DspIFace->addVarItem(ActValData, new cDspVar("TDSYNC",1,vApplication | vDspIntern)); // delay time pps -> 1. sample
    DspIFace->addVarItem(ActValData, new cDspVar("PHIN",1,vApplication | vDspIntern));
    DspIFace->addVarItem(ActValData, new cDspVar("PHIX",1,vApplication | vDspIntern));


    DspIFace->addVarItem(ActValData, new cDspVar("MESSSIGNAL0",nS,vDspIntern));
    DspIFace->addVarItem(ActValData, new cDspVar("MESSSIGNAL1",nS,vDspIntern));
    DspIFace->addVarItem(ActValData, new cDspVar("SCHAN",schanLen,vDspIntern)); // sinus, cosinus, hanning abwechselnd
    DspIFace->addVarItem(ActValData, new cDspVar("MESSSIGNAL2",4*nSp,vDspIntern));
    DspIFace->addVarItem(ActValData, new cDspVar("MESSSIGNAL3",4*nSp,vDspIntern));

    }
}

void cWM3000U::SetDspWMCmdList()
{
    if (!m_ConfData.m_bSimulation) {
    int nSPer = (m_ConfData.m_nSRate == S80) ? 80 :256;
    int nMP = m_ConfData.m_nMeasPeriod;
    int nSMeas = nSPer * nMP; // anzahl samples messperiode
    // int nSAK = nSPer * ((m_ConfData.m_nMeasPeriod - 1) >> 1); // messperiode für kreuzkorrelationsintegral
    QString s;
    DspIFace->ClearCmdList();

    DspIFace->addCycListItem( s = "STARTCHAIN(1,1,0x0100)"); // aktiv, prozessnr. (dummy),hauptkette 1 subkette 0 start
        DspIFace->addCycListItem( s = QString("CLEARN(%1,MAXN)").arg(3*nSMeas+8*nSPer+37) ); // alle variable löschen
        DspIFace->addCycListItem( s = "SETVAL(SINDEX,0)"); // index zum speichern der samples auf anfang der puffer setzen
        DspIFace->addCycListItem( s = "SETVAL(SINDEX2,0)"); // index zum speichern der samples auf anfang der puffer setzen
        DspIFace->addCycListItem( s = "RESETSYNCPPS()"); // pps sync flagge rückstellen
        DspIFace->addCycListItem( s = "SETVAL(KFKORRF,1.0)"); // vorbesetzen filterausgang
        DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0100)"); // ende prozessnr., hauptkette 1 subkette 0
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0100)"); // ende prozessnr., hauptkette 1 subkette 0

    DspIFace->addCycListItem( s = "TESTSYNCPPSSKIPEQ()"); // falls syncimpuls pps war  aktivieren wir hauptkette 2 subkette 0
    DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0200)"); // aktivieren dieser kette

    // hier setzen wir nur das sampling system zurück
    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0200)"); // nicht aktiv, prozessnr. (dummy),hauptkette 2 subkette 0 start
        DspIFace->addCycListItem( s = "SETVAL(SINDEX,0)"); // index zum speichern der samples auf anfang der puffer setzen
        DspIFace->addCycListItem( s = "RESETSYNCPPS()"); // pps sync flagge rückstellen
        DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0300)"); // aktivieren der daten aufnahme
        DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0200)"); // deaktivieren dieser kette
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0200)"); // ende prozessnr., hauptkette 2 subkette 0

    // kopieren der maxima aus dsp workspace
    DspIFace->addCycListItem( s = "COPYDU(2,MAXIMUMSAMPLE,MAXN)");
    DspIFace->addCycListItem( s = "SETVAL(MAXRDY,1.0)"); // sync maxrdy setzen

    // ab hier nehmen wir die daten für die nächste messperiode auf
    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0300)"); // nicht aktiv, prozessnr. (dummy),hauptkette 3 subkette 0 start
        // kanal 0 (n)  samples über sindex kopieren
        DspIFace->addCycListItem( s = "COPYINDDATA(CH0,SINDEX,MESSSIGNAL0)");
        // kanal 1 (x)  samples über sindex kopieren
        DspIFace->addCycListItem( s = "COPYINDDATA(CH1,SINDEX,MESSSIGNAL1)");
        // sindex inkrementieren
        DspIFace->addCycListItem( s = "INC(SINDEX)");
        DspIFace->addCycListItem( s = QString("TESTVCSKIPLT(SINDEX,%1)").arg(nMP)); // test ob die messperiode vollständig ist
        DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0400)"); // aktivieren der berechnung
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0300)"); // ende prozessnr., hauptkette 3 subkette 0


    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0400)"); // nicht aktiv, prozessnr. (dummy),hauptkette 4 subkette 0 start
        DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0300)"); // deaktivieren der daten aufnahme
        // kanal 0 (n) bearbeiten
        //DspIFace->addCycListItem( s = "BREAK(1)"); // breakpoint wenn /taster
        //DspIFace->addCycListItem( s = QString("AUTOKORRELATION(%1,MESSSIGNAL0,KFKORR)").arg(nSAK));
        DspIFace->addCycListItem( s = "SETVAL(SINDEX,0)"); // index zum speichern der samples auf anfang der puffer setzen denn wir haben eine volle messperiode
        // alternativ zur autokorrelation verwenden wir die hardware frequenzmessung
        DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(m_ConfData.m_fSFreq,0,'E'));
        DspIFace->addCycListItem( s = "COPYDU(1,TMCH0,TEMP2)"); // perioden dauer messsignal in sec.
        //DspIFace->addCycListItem( s = QString("SETVAL(TEMP2,%1)").arg(0.02,0,'E')); // für test
        //DspIFace->addCycListItem( s = "BREAK(1)"); // breakpoint wenn /taster
        DspIFace->addCycListItem( s = "MULVVV(TEMP1,TEMP2,TEMP2)");
        DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(1.0,0,'E'));
        DspIFace->addCycListItem( s = "DIVVVV(TEMP1,TEMP2,KFKORR)");
        DspIFace->addCycListItem( s = "DIVVVV(TEMP1,TEMP2,KFKORRF)");  // keine filterung

        /*
        // filterung kfkorr
        float fRueckkopplung = 0.0; //  0 bedeutet filter aus !!
        DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(fRueckkopplung,0,'E'));
        DspIFace->addCycListItem( s = QString("SETVAL(TEMP2,%1)").arg(fRueckkopplung+1.0,0,'E'));
        DspIFace->addCycListItem( s = "MULVVV(KFKORRF,TEMP1,TEMP1)"); // (rekursives tiefpassfilter für kfkorrf
        DspIFace->addCycListItem( s = "ADDVVV(KFKORR,TEMP1,TEMP1)");
        DspIFace->addCycListItem( s = "DIVVVV(TEMP1,TEMP2,KFKORRF)");
        */

        DspIFace->addCycListItem( s = "COPYUD(1,KFKORRF,KREISFREQKOEFF)");
        // wir nehmen den gefilterten wert

        //DspIFace->addCycListItem( s = QString("FLATTOP(%1,SCHAN)").arg(nSMeas)); // fensterfunktion generieren
        DspIFace->addCycListItem( s = QString("HANNING(%1,SCHAN)").arg(nSMeas)); // fensterfunktion generieren
        //DspIFace->addCycListItem( s = "BREAK(1)"); // breakpoint wenn taster
        DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL0)").arg(nSMeas)); // fenster funktion anwenden
        // korrigierte einheitswurzel berechnen und im bzw. re von kanal 0 bestimmen
        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
        else
            DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL0,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(nSMeas)); // im = integral

        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
        else
            DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL0,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(nSMeas)); // re = integral

        // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
        if (m_ConfData.m_bDCmeasurement)
        {   // bei dc interessiert uns eingentlich nur der realteil (temp2);
            DspIFace->addCycListItem( s = "COPYVAL(TEMP2,AMPL1N)");
            DspIFace->addCycListItem( s = "COPYVAL(TEMP1,PHIN)");
        }
        else
        {
            DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,AMPL1N)");
            // DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,FAMPL1N)");
            DspIFace->addCycListItem( s = "ARCTAN(TEMP1,TEMP2,PHIN)");
        }

        // rms wert berechnung
        DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL0,RMSN)").arg(nSMeas));
        //DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL0,FRMSN)").arg(nSMeas));
        // kanal 1 (x) bearbeiten

        DspIFace->addCycListItem( s = QString("HANNING(%1,SCHAN)").arg(nSMeas)); // fensterfunktion generieren
        DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL1)").arg(nSMeas)); // fenster funktion anwenden
        // korrigierte einheitswurzel berechnen und im bzw. re von kanal 1 bestimmen
        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
        else
            DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (sinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL1,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(nSMeas)); // im = integral

        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
        else
            DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(nSMeas)); // einheitswurzeln (cosinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL1,SCHAN)").arg(nSMeas)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(nSMeas)); // re = integral

        // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
        if (m_ConfData.m_bDCmeasurement)
        {   // bei dc interessiert uns eingentlich nur der realteil (temp2);
            DspIFace->addCycListItem( s = "COPYVAL(TEMP2,AMPL1X)");
            DspIFace->addCycListItem( s = "COPYVAL(TEMP1,PHIX)");
        }
        else
        {
            DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,AMPL1X)");
            DspIFace->addCycListItem( s = "ARCTAN(TEMP1,TEMP2,PHIX)");
        }

        // rms wert berechnung
        DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL1,RMSX)").arg(nSMeas)); // rmswert berechnen
        // ermitteln der zeit zwischen pps und 1. samplewert
        DspIFace->addCycListItem( s = "COPYDATA(CH2,0,MESSSIGNAL0)"); // auf  kanal 2 PPS2SampleTime ... kopiert nur die ersten 80 oder 256 samples

        // kleiner kunstgriff um das 2. datum aus den sampledaten zu bekommen
        // 2.  datum wegen phasenkorrektur puffer
        DspIFace->addCycListItem( s = "SETVAL(TEMP1,0.0)");
        DspIFace->addCycListItem( s = "ADDVVV(TEMP1,MESSSIGNAL0+2,TDSYNC)");

        // falsch falsch falsch
        // dphi=(phin-phix)+ tdsync * (10*10^-9) * 2PI / (signalfreq* kfkorr)
        // richtig
        // dphi=(phin-phix)+ tdsync * (10*10^-9) * 2PI * (signalfreq* kfkorr)          			            =(phin-phix)+ tdsync * (2PI*10^8 *  signalfreq * kfkorr)

        // die phasenkorrektur ist erforderlich weil von pps bis zum 1. sample eine totzeit liegt

        float fp = 6.283185307e-8 * m_ConfData.m_fSFreq;

        DspIFace->addCycListItem( s = QString("SETVAL(TEMP1,%1)").arg(fp));
        DspIFace->addCycListItem( s = "MULVVV(TEMP1,KFKORRF,TEMP1)"); // (2PI * (signalfreq*10^-8) * kfkorr)

        DspIFace->addCycListItem( s = "MULVVV(TEMP1,TDSYNC,TEMP2)"); // tdsync * (2PI * (signalfreq*10^-8)) * kfkorr
        DspIFace->addCycListItem( s = "ADDVVV(PHIN,TEMP2,PHIN)");
        if (m_ConfData.m_nMeasMode != Un_nConvent)   // wenn conventional
            DspIFace->addCycListItem( s = "ADDVVV(PHIX,TEMP2,PHIX)"); // auch kanal x

        DspIFace->addCycListItem( s = "SUBVVV(PHIN,PHIX,DPHI)"); // dphi=(phin-phix)  !!! vorzeichen gedreht
        DspIFace->addCycListItem( s = "NORMVC(DPHI,6.283185307)"); // auf 2PI normieren
        DspIFace->addCycListItem( s = "SYMPHI(DPHI,DPHI)"); // symmetiert den winkel auf  -pi...+pi -> zwecks filterung
        // alle messwerte noch filtern

        DspIFace->addCycListItem( s = "AVERAGE1(5,RMSN,FILTER)");

        // wir haben nach pps nur über die eingestellte anzahl signalperioden gemessen und die werte
        // ins filter übertragen.
        DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0500)"); // kette 0x0500 dient zum synchronisieren mit dem ctrl programm
        DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0400)"); // deaktivieren der berechnung
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0400)"); // ende prozessnr., hauptkette 4 subkette 0

    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0500)"); // aktiv, prozessnr. (dummy), hauptkette 5 subkette 0 start
        DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0501)"); // aktiv, prozessnr. (dummy), hauptkette 5 subkette 1 start
        DspIFace->addCycListItem( s = "CMPAVERAGE1(5,FILTER,RMSNF)");
        DspIFace->addCycListItem( s = "CLEARN(11,FILTER)");
        DspIFace->addCycListItem( s = "DSPINTTRIGGER(0x0,0x0001)"); // gibt interrupt an controler
        DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0501)"); // deaktivieren der berechnung
        DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0500)"); // deaktivieren der synch
        DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0501)"); // ende prozessnr., hauptkette 5 subkette 1
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0500)"); // ende prozessnr., hauptkette 5 subkette 0

    // kanal 0 (n)  samples über sindex2 kopieren
    DspIFace->addCycListItem( s = "COPYINDDATA(CH0,SINDEX2,MESSSIGNAL2)");
    // kanal 1 (x)  samples über sindex2 kopieren
    DspIFace->addCycListItem( s = "COPYINDDATA(CH1,SINDEX2,MESSSIGNAL3)");

    // sindex2 inkrementieren
    DspIFace->addCycListItem( s = "INC(SINDEX2)");
    DspIFace->addCycListItem( s = QString("TESTVCSKIPLT(SINDEX2,%1)").arg(4)); // test ob die messperiode vollständig ist
    DspIFace->addCycListItem( s = "ACTIVATECHAIN(1,0x0600)"); // aktivieren der berechnung

    DspIFace->addCycListItem( s = "STARTCHAIN(0,1,0x0600)"); // nicht aktiv, prozessnr. (dummy),hauptkette 6 subkette 0 start
        DspIFace->addCycListItem( s = "SETVAL(SINDEX2,0)"); // index 0 setzen
        DspIFace->addCycListItem( s = QString("HANNING(%1,SCHAN)").arg(4*nSPer)); // fensterfunktion über 4 signalperioden generieren
        DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL2)").arg(4*nSPer)); // fenster funktion anwenden
        DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL2,FRMSN)").arg(4*nSPer)); // die schnelle rms messung für kanal n
        DspIFace->addCycListItem( s = QString("MULNCC(%1,SCHAN,MESSSIGNAL3)").arg(4*nSPer)); // fenster funktion anwenden
        DspIFace->addCycListItem( s = QString("RMSN(%1,MESSSIGNAL3,FRMSX)").arg(4*nSPer)); // die schnelle rms messung für kanal x

        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (sinus)
        else
            DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (sinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL2,SCHAN)").arg(4*nSPer)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(4*nSPer)); // im = integral
        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (cosinus)
        else
            DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (cosinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL2,SCHAN)").arg(4*nSPer)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(4*nSPer)); // re = integral
        // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
        DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,FAMPL1N)"); // für schnelle lp anzeige

        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("SINUS(0,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (sinus)
        else
            DspIFace->addCycListItem( s = QString("SINUS(1,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (sinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL3,SCHAN)").arg(4*nSPer)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP1)").arg(4*nSPer)); // im = integral
        if (m_ConfData.m_bDCmeasurement)
            DspIFace->addCycListItem( s = QString("COSINUS(0,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (cosinus)
        else
            DspIFace->addCycListItem( s = QString("COSINUS(1,%1,SCHAN)").arg(4*nSPer)); // einheitswurzeln (cosinus)
        DspIFace->addCycListItem( s = QString("MULNCC(%1,MESSSIGNAL3,SCHAN)").arg(4*nSPer)); // mit signal multiplizieren
        DspIFace->addCycListItem( s = QString("INTEGRAL(%1,SCHAN,TEMP2)").arg(4*nSPer)); // re = integral
        // amplitude grundwelle = sqr(im^2 + re^2) bzw. geometrische summe und phasenlage
        DspIFace->addCycListItem( s = "ADDVVG(TEMP1,TEMP2,FAMPL1X)"); // für schnelle lp anzeige

        DspIFace->addCycListItem( s = "DEACTIVATECHAIN(1,0x0600)"); // deaktivieren der berechnung
    DspIFace->addCycListItem( s = "STOPCHAIN(1,0x0600)"); // ende prozessnr., hauptkette 4 subkette 0

    // ab hier interrupt befehlskette
    // ------------------------------

    // die ergebnisse werden aus der appl. heraus mittels kommando (bearbeiten int cmd list) generiert
    DspIFace->addIntListItem( s = "STARTCHAIN(1,1,0x0001)"); // aktiv, prozessnr. (dummy), hauptkette 0 subkette 1 start
    DspIFace->addIntListItem( s = "ACTIVATECHAIN(1,0x0501)"); // aktivieren der berechnung des filters
    DspIFace->addIntListItem( s = "STOPCHAIN(1,0x0001)"); // ende prozessnr., hauptkette 0 subkette 1

    // rücksetzen der maxima wie oben per int cmd list mit angabe der subketten nr.
    DspIFace->addIntListItem( s = "STARTCHAIN(1,1,0x0002)"); // aktiv, prozessnr.(dummy), hauptkette 0 subkette 2 start
    DspIFace->addIntListItem( s = "SETVAL(MAXX,0.0)");
    DspIFace->addIntListItem( s = "SETVAL(MAXN,0.0)");
    DspIFace->addIntListItem( s = "SETVAL(MAXRDY,0.0)");
    //    DspIFace->addIntListItem( s = "DSPINTTRIGGER(0x0,0x0002)"); // gibt interrupt an controler
    DspIFace->addIntListItem( s = "STOPCHAIN(1,0x0002)"); // ende prozessnr., hauptkette 0 subkette 2

    DspIFace->addIntListItem( s = "STARTCHAIN(1,1,0x0003)"); // aktiv, prozessnr. (dummy), hauptkette 0 subkette 3 start
    DspIFace->addIntListItem( s = "CLEARN(11,FILTER)");
    DspIFace->addIntListItem( s = "DSPINTTRIGGER(0x0,0x0003)"); // gibt interrupt an controler
    DspIFace->addIntListItem( s = "STOPCHAIN(1,0x0003)"); // ende prozessnr., hauptkette 0 subkette 3

    }
}


void cWM3000U::StopMeasurement()
{
    RangeTimer->stop();
    MeasureTimer->stop();
    MeasureLPTimer->stop();
    AHSFifo.remove(TriggerMeasureStart); // alle trigger measure event löschen
    AHSFifo.remove(MeasureStart); // alle measure event löschen
    AHSFifo.remove(MeasureLPStart);
    AHSFifo.remove(RangeObsermaticStart); // alle events zur bereichüberwachung löschen
    m_bStopped = true;
    m_bDspMeasureIgnore = true; // m_bDspMeasureTriggerActive; // wenn der trigger schon raus ist ignorieren wir die ergebnisse
    ActValues.bvalid = false; // wir schalten die fehleranzeige inaktiv
    emit SendActValuesSignal(&ActValues);
}

// simulationsdaten werden so generiert dass der benutzer und damit der programmierer feststellen kann
// ob die durchgeführten berechnungen richtig sind:
// die frequenz ist immer +5% neben der eingestellten signalfrequenz
// die amplitude ist immer zwischwn +/- 2% vom eingestellten Un
// der fehler des x wandlers ist immer 0
// durch verändern der eigenfehlertabelle und durch prüfling manipulation (abtastverzögerung und phasenfehler
// im konfigurationsmenu)  können die berechnungen kontrolliert werden.

void cWM3000U::SimulatedMeasurement()
{
    ActValues.dspActValues.kfkorrf = 1.005; // wenn 50 -> f =50.25
    ActValues.dspActValues.tdsync = 0.0;

    double rnd = 1.0+ (0.02 * random() / RAND_MAX);

    eParameter e = m_ConfData.m_NSecondary;

    ActValues.dspActValues.rmsnf = e.toDouble() * rnd;
    CWMRange* r = Range(m_ConfData.m_sRangeN,m_sNRangeList);
    ActValues.dspActValues.rmsnf *= r->Rejection()/r->Value();
    ActValues.RMSN1Sek = ActValues.dspActValues.rmsnf;
    ActValues.dspActValues.ampl1nf = ActValues.dspActValues.rmsnf * 1.414213562; // klirrfaktor = 0
    ActValues.dspActValues.phin = 0.0+PI/2; // vektor hat 0°

    float rej,val;
    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
     case Un_UxAbs:
        e =  m_ConfData.m_XSecondary;
        r = Range(m_ConfData.m_sRangeX,m_sNRangeList);
        val = r->Value();
        rej = r->Rejection();
    break;
              case Un_EVT:
        e =  m_ConfData.m_EVTSecondary;
        r = Range(m_ConfData.m_sRangeEVT,m_sEVTRangeList);
        val = r->Value();
        rej = r->Rejection();
    break;
              case Un_nConvent:
        e =  m_ConfData.m_XSecondary;
        val = 1.0;
        rej = 1e-3;
     }

    ActValues.dspActValues.rmsxf = e.toDouble() * rnd;
    ActValues.dspActValues.rmsxf *= rej/val;
    ActValues.RMSX1Sek = ActValues.dspActValues.rmsxf;
    ActValues.dspActValues.ampl1xf = ActValues.dspActValues.rmsxf * 1.414213562; // klirrfaktor = 0
    ActValues.dspActValues.phix = 0.0+PI/2; // vektor hat 0°

    ActValues.dspActValues.dphif = 0.0; // differenz vektor hat 0°
    MaxValues.maxn = ActValues.dspActValues.ampl1nf;
    MaxValues.maxx = ActValues.dspActValues.ampl1xf;
    MaxValues.maxRdy = 1.0; // maxima sind verfügbar

    CmpActValues();
    CmpRMSValues();
    emit SendActValuesSignal(&ActValues);
    emit SendLPSignal(&ActValues);
}


void cWM3000U::CmpActFrequency()
{

    double fsoll;

    if (m_ConfData.m_bDCmeasurement)
    {
        fsoll = 0.0;
    }
    else
    {
        switch (m_ConfData.m_nSFreq)
        {
            case F16 : fsoll = 50.0/3.0;break;
            case F50 : fsoll = 50.0;break;
            case F60 : fsoll = 60.0;
        }
    }

    ActValues.Frequenz = m_ConfData.m_fSFreq * ActValues.dspActValues.kfkorrf;
    if (ActValues.Frequenz < 6.0) //
        ActValues.Frequenz = 0.0;


    bool bFreqQuestionable = (fabs(ActValues.Frequenz-fsoll) > 1.0);
    emit FreqQuestionable(bFreqQuestionable);
    if (bFreqQuestionable)
        emit AffectStatus(SetQuestStat, QuestFrequency); // questionable status setzen
    else
        emit AffectStatus(ResetQuestStat, QuestFrequency);
}


void cWM3000U::CmpActValues() {  // here we will do all the necessary computations
    // korrektur der rohwinkel weil fft phi = 0 für cosinus

    ActValues.PHIN = normWinkelrad02PI(ActValues.dspActValues.phin - PI/2);  // winkel zwischen 0 und 2PI
    ActValues.PHIX = normWinkelrad02PI(ActValues.dspActValues.phix - PI/2);

    // frequenz und abtastverzögerung berechnen

    CmpActFrequency();
    ActValues.TDSync = ActValues.dspActValues.tdsync / TDBase;

    CWMRange* r = Range(m_ConfData.m_sRangeN,m_sNRangeList);

    double val,rej,im,re;
    val = r->Value();
    rej = r->Rejection();

    // setzen der übersetzungsverhältnisse

    eParameter PrimN, PrimX;
    eParameter SekN, SekX;

    PrimN = m_ConfData.m_NPrimary;
    SekN = m_ConfData.m_NSecondary;
    double kn = PrimN.toDouble() / SekN.toDouble();

    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case Un_UxAbs:
    case Un_nConvent: // wir brauchen jetzt das ü-verhältnis um die sekundär grösse zu ermitteln
        PrimX = m_ConfData.m_XPrimary;
        SekX = m_ConfData.m_XSecondary;
        break;
    case Un_EVT:
        PrimX = m_ConfData.m_EVTPrimary;
        SekX = m_ConfData.m_EVTSecondary;
        break;
    }

    double kx = PrimX.toDouble() / SekX.toDouble();

    // berechnen der sekundär grössen
    // amplitude n,x sind gefilterte messwerte, die winkel nicht da feste abtastfrequenz

    re = ( val * ActValues.dspActValues.ampl1nf ) / rej;
    ActValues.VekNSek = complex (re,0.0); // der winkel für vekn ist 0 .... definition !!!

    // wir haben u.u. eine offsetkorrektur für kanal n in "V" die wir nach der skalierung berücksichtigen müssen

    if (m_ConfData.m_bOffsetCorrectionN && m_ConfData.m_bDCmeasurement)
        ActValues.VekNSek -= m_JustValues.OffsetCorrDevN;


    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case Un_UxAbs:
        r = Range(m_ConfData.m_sRangeX,m_sNRangeList);
        val = r->Value();
        rej = r->Rejection();
        break;
    case Un_EVT:
        r = Range(m_ConfData.m_sRangeEVT,m_sEVTRangeList);
        val = r->Value();
        rej = r->Rejection();
        break;
    case Un_nConvent:
        val = 10.0e-3; // 1lsb -> 10mV
        //rej = 1.0;
        rej = kx; // wir wollen die sekundär grösse des nConvent haben ... neue forderung dies anzuzeigen
    }

    re = im = ( val * ActValues.dspActValues.ampl1xf ) / rej;
    im *= sin(ActValues.dspActValues.dphif);
    re *= cos(ActValues.dspActValues.dphif);
    ActValues.VekXSek = complex(re,im);

    if (m_ConfData.m_bOffsetCorrectionX && m_ConfData.m_bDCmeasurement)
        ActValues.VekXSek -= m_JustValues.OffsetCorrDevX;

    // alle winkel werden vom dsp ermittelt und  dphi ist schon mit tdsync frequenzabhängig korrigiert
    // die vektoren n und x würden normalerweise mit der differenzfrequenz aus abtast- und signalfrequenz
    // rotieren, wir legen vekn auf die ordinate und vekx hat den winkel dphif = konstant
    // winkel korrekturen aus abtastverzögerungen und phasenkorrekturwerten

    double phik;
    //  korrektur des kanal X vektors mit der abtastverzögerung und dem bekannten phasenfehler des prüflings
    phik = ( ( -360.0  * ActValues.Frequenz * m_ConfData.m_fxTimeShift * 1.0e-3 ) - m_ConfData.m_fxPhaseShift) * PI_180;
    ActValues.VekXSek *= complex( cos(phik),sin(phik) );

    // eigenfehler korrektur des normwandlers

    ActValues.UInCorr = m_pOwnError->GetOECorrVector(); // achtung complex !!!!!
    ActValues.VekNSek *= ActValues.UInCorr;

    // umrechnen auf primärgrößen

    ActValues.VekX = ActValues.VekXSek * kx;
    ActValues.VekN = ActValues.VekNSek * kn;

    //  fehler berechnung

    ActValues.AngleError = UserAtan(ActValues.VekX) - UserAtan(ActValues.VekN);
    ActValues.AngleError = normWinkelrad_PIPI(ActValues.AngleError);

    double err;

    if (m_ConfData.m_bDCmeasurement)
        err = (ActValues.VekX.re() - ActValues.VekN.re()) / ActValues.VekN.re();
    else
    {
        double absN;
        absN = fabs(ActValues.VekN);
        err = (fabs(ActValues.VekX) -absN) / absN;
    }

    ActValues.AmplErrorIEC = 100.0 * err;
    ActValues.RCF = 1.0 / (1.0 + err);
    ActValues.AmplErrorANSI = (ActValues.AmplErrorIEC/100.0 - ( (1.0+ActValues.AmplErrorIEC/100.0) * (4.0 / 3.0) * ActValues.AngleError ))*100.0;

    ActValues.bvalid = true; // aktivieren der fehleranzeige
}


void cWM3000U::CmpRMSValues()
{
    CWMRange* range;
    double val,rej;
    double kn,kx;

    // setzen der übersetzungsverhältnisse

    eParameter PrimN, PrimX;
    eParameter SekN, SekX;

    PrimN = m_ConfData.m_NPrimary;
    SekN = m_ConfData.m_NSecondary;
    kn = PrimN.toDouble() / SekN.toDouble();

    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case Un_UxAbs:
    case Un_nConvent: // wir brauchen jetzt das ü-verhältnis um die sekundär grösse zu ermitteln
        PrimX = m_ConfData.m_XPrimary;
        SekX = m_ConfData.m_XSecondary;
    break;
    case Un_EVT:
        PrimX = m_ConfData.m_EVTPrimary;
        SekX = m_ConfData.m_EVTSecondary;
    break;
    }

    kx = PrimX.toDouble() / SekX.toDouble();

    range = Range(m_ConfData.m_sRangeN,m_sNRangeList);
    val = range->Value();
    rej = range->Rejection();

    ActValues.RMSNSek = ( val * ActValues.dspRMSValues.fastRMSN ) / rej;
    ActValues.RMSN1Sek = ( val * ActValues.dspRMSValues.fastRMSN1 ) / rej;

    // wir haben u.u. eine offsetkorrektur für kanal n in "V" die wir nach der skalierung berücksichtigen müssen

    if (m_ConfData.m_bOffsetCorrectionN && m_ConfData.m_bDCmeasurement)
        ActValues.RMSNSek -= fabs(m_JustValues.OffsetCorrDevN);

    switch (m_ConfData.m_nMeasMode) // für dut messart abhängig
    {
    case Un_UxAbs:
        range = Range(m_ConfData.m_sRangeX,m_sNRangeList);
        val = range->Value();
        rej = range->Rejection();
    break;
    case Un_EVT:
        range = Range(m_ConfData.m_sRangeEVT,m_sEVTRangeList);
        val = range->Value();
        rej = range->Rejection();
    break;
    case Un_nConvent:
        val = 10e-3; // 1lsb -> 10mV
        rej = kx; // wir wollen die sekundär grösse des nConvent haben ... neue forderung dies anzuzeigen
    break;
    }

    ActValues.RMSXSek = ( val * ActValues.dspRMSValues.fastRMSX ) / rej;
    ActValues.RMSX1Sek = ( val * ActValues.dspRMSValues.fastRMSX1 ) / rej;

    // wir haben u.u. eine offsetkorrektur für kanal x in "V" die wir nach der skalierung berücksichtigen müssen
    if (m_ConfData.m_bOffsetCorrectionX && m_ConfData.m_bDCmeasurement)
        ActValues.RMSXSek -= fabs(m_JustValues.OffsetCorrDevX);

    // eigenfehler korrektur des normwandlers

    ActValues.UInCorr = m_pOwnError->GetOECorrVector(); // achtung complex !!!!!
    ActValues.RMSNSek *= fabs(ActValues.UInCorr);
    ActValues.RMSN1Sek *= fabs(ActValues.UInCorr);

    // umrechnen auf primärgrößen

    ActValues.RMSN = ActValues.RMSNSek * kn;
    ActValues.RMSN1 = ActValues.RMSN1Sek * kn;

    ActValues.RMSX = ActValues.RMSXSek * kx;
    ActValues.RMSX1 = ActValues.RMSX1Sek * kx;

    //  lastpunkt berechnung

    ActValues.LoadPoint = 100.0 * ActValues.RMSN/PrimN.toDouble();
    ActValues.LoadPoint1 = 100.0 * ActValues.RMSN1/PrimN.toDouble();
    ActValues.LoadPointX = 100.0 * ActValues.RMSN/PrimX.toDouble();
    ActValues.LoadPoint1X = 100.0 * ActValues.RMSN1/PrimX.toDouble();
}


void cWM3000U::CorrActValues()
{  // here we will do all the necessary corrections with use of adjustment values
    const float PI_180 = 0.017453293;
    int sign;
    double offsetCorr;
    QString key;

    ActValues.dspActValues.rmsnf *= m_JustValues.GainCorrCh0;
    ActValues.dspActValues.ampl1nf *= m_JustValues.GainCorrCh0;
    ActValues.dspActValues.phin += m_JustValues.PhaseCorrCh0 * PI_180;
    ActValues.dspActValues.dphif += m_JustValues.PhaseCorrCh0 * PI_180;

    if (m_ConfData.m_bDCmeasurement)
    {
        // wir korrigieren die offsetwerte aus der permanenten offset korrektur
        // im rms können wir aber das vorzeichen nicht mehr sehen .....
        sign = signum(ActValues.dspActValues.ampl1nf);
        ActValues.dspActValues.rmsnf = sign * ActValues.dspActValues.rmsnf + m_JustValues.OffsetCorrCh0;
        ActValues.dspActValues.ampl1nf /= 2.0;
        ActValues.dspActValues.ampl1nf += m_JustValues.OffsetCorrCh0;

        // wir korrigieren die offsetwerte aus der temp. offset korrektur
        CWMRange* r = Range(m_ConfData.m_sRangeN, m_sNRangeList);
        if (measOffsetCorrectionHash.contains(key = r->getOffsKorrKey()))
            offsetCorr = measOffsetCorrectionHash[key];
        else
            offsetCorr = 0.0;

        ActValues.dspActValues.rmsnf = fabs(ActValues.dspActValues.rmsnf + offsetCorr);
        ActValues.dspActValues.ampl1nf += offsetCorr;
    }

    if ( m_ConfData.m_nMeasMode != Un_nConvent)
    { // wenn nicht nconvent
        CWMRange* r;

        ActValues.dspActValues.rmsxf *= m_JustValues.GainCorrCh1; // x kanal auch korrigieren
        ActValues.dspActValues.ampl1xf *= m_JustValues.GainCorrCh1;
        ActValues.dspActValues.phix += m_JustValues.PhaseCorrCh1 * PI_180;
        ActValues.dspActValues.dphif -= m_JustValues.PhaseCorrCh1 * PI_180;

        if (m_ConfData.m_bDCmeasurement)
        {
            // wir korrigieren die offsetwerte aus der permanenten offset korrektur
            sign = signum(ActValues.dspActValues.ampl1xf);
            ActValues.dspActValues.rmsxf = sign * ActValues.dspActValues.rmsxf + m_JustValues.OffsetCorrCh1;
            ActValues.dspActValues.ampl1xf /= 2.0;
            ActValues.dspActValues.ampl1xf += m_JustValues.OffsetCorrCh1;

            // wir korrigieren die offsetwerte aus der temp. offset korrektur
            if (m_ConfData.m_nMeasMode == Un_UxAbs)
                r = Range(m_ConfData.m_sRangeX, m_sXRangeList);
            else
                r = Range(m_ConfData.m_sRangeEVT, m_sEVTRangeList);

            if (measOffsetCorrectionHash.contains(key = r->getOffsKorrKey()))
                offsetCorr = measOffsetCorrectionHash[key];
            else
                offsetCorr = 0.0;

            ActValues.dspActValues.rmsxf = fabs(ActValues.dspActValues.rmsxf + offsetCorr);
            ActValues.dspActValues.ampl1xf += offsetCorr;
        }
    }
}


void cWM3000U::CorrRMSValues()
{
    int sign;
    double offsetCorr;
    QString key;
    CWMRange* range;

    ActValues.dspRMSValues.fastRMSN *= m_JustValues.GainCorrCh0;
    ActValues.dspRMSValues.fastRMSN1 *= m_JustValues.GainCorrCh0;

    if (m_ConfData.m_bDCmeasurement)
    {
        // wir korrigieren die offsetwerte aus der permanenten offset korrektur
        sign = signum(ActValues.dspRMSValues.fastRMSN1);
        ActValues.dspRMSValues.fastRMSN = sign * ActValues.dspRMSValues.fastRMSN + m_JustValues.OffsetCorrCh0;
        ActValues.dspRMSValues.fastRMSN1 = sign * ActValues.dspRMSValues.fastRMSN1 + m_JustValues.OffsetCorrCh0;

        // wir korrigieren die offsetwerte aus der temp. offset korrektur
        range = Range(m_ConfData.m_sRangeN, m_sNRangeList);
        if (measOffsetCorrectionHash.contains(key = range->getOffsKorrKey()))
            offsetCorr = measOffsetCorrectionHash[key];
        else
            offsetCorr = 0.0;

        ActValues.dspRMSValues.fastRMSN = fabs(ActValues.dspRMSValues.fastRMSN + offsetCorr);
        ActValues.dspRMSValues.fastRMSN1 = fabs(ActValues.dspRMSValues.fastRMSN1 + offsetCorr);
    }

    if (m_ConfData.m_nMeasMode != Un_nConvent) // für nconvent die korrektur nicht berücksichtigen
    {
        ActValues.dspRMSValues.fastRMSX *= m_JustValues.GainCorrCh1;
        ActValues.dspRMSValues.fastRMSX1 *= m_JustValues.GainCorrCh1;

        if (m_ConfData.m_bDCmeasurement)
        {
            // wir korrigieren die offsetwerte aus der permanenten offset korrektur
            sign = signum(ActValues.dspRMSValues.fastRMSX1);
            ActValues.dspRMSValues.fastRMSX = sign * ActValues.dspRMSValues.fastRMSX + m_JustValues.OffsetCorrCh1;
            ActValues.dspRMSValues.fastRMSX1 = sign * ActValues.dspRMSValues.fastRMSX1 + m_JustValues.OffsetCorrCh1;

            // wir korrigieren die offsetwerte aus der temp. offset korrektur
            if (m_ConfData.m_nMeasMode == Un_UxAbs)
                range = Range(m_ConfData.m_sRangeX, m_sXRangeList);
            else
                range = Range(m_ConfData.m_sRangeEVT, m_sEVTRangeList);

            if (measOffsetCorrectionHash.contains(key = range->getOffsKorrKey()))
                offsetCorr = measOffsetCorrectionHash[key];
            else
                offsetCorr = 0.0;

            ActValues.dspRMSValues.fastRMSX = fabs(ActValues.dspRMSValues.fastRMSX + offsetCorr);
            ActValues.dspRMSValues.fastRMSX1 = fabs(ActValues.dspRMSValues.fastRMSX1 + offsetCorr);
        }
    }
}
