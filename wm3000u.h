//  headerdatei wm3000U.h

#ifndef WM3000U_H
#define WM3000U_H

#include <qobject.h>
#include <q3ptrlist.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3mainwindow.h>
#include <q3textbrowser.h>
#include <qtimer.h>
#include <q3progressdialog.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <QHash>

#include "zhclientsocket.h"
#include "wmglobal.h"
#include "logfile.h"
#include "range.h"
#include "ownerror.h"
#include "dspiface.h"
#include "pcbiface.h"
#include "en61850.h"
#include "devserver.h"
#include "wm3000scpiface.h"
#include "movingwindowfilter.h"

class cRSelectString: public QString {
public:    
    cRSelectString() : QString(){}
    cRSelectString (const QString s) : QString(s){}
    cRSelectString (const char* s) : QString(s){}
};

enum wm3000ActionHandlerState {
    wm3000Idle,    			// 0
    
    EnterSimulationMode,
    
    InitializationStart,
    InitializationConnect2pcbHost,
    InitializationTestDC,
    InitializationReadJustdataVersion,
    InitializationOpenChannel0,
    InitializationOpenChannel1,
    
    InitializationSetRanges,
    InitializationSyncRange0,
    InitializationSyncRange01,
    InitializationSyncRange02, 
    InitializationSyncRange1,		// 10
    InitializationSyncRange11,
    InitializationSyncRange12,
      
    InitializationConnectDspIFace,
    InitializationBootDsp,
    InitializationBootDspPause,
    InitializationSetSamplingSystem,
    InitializationSetDspVarList,
    InitializationSetDspCmdLists,
    InitializationActivateCmdLists,
    InitializationSetSamplingFrequency,	//20
    InitializationSetSamplingPSamples,
    InitializationSetSenseMode,
    InitializationSetSyncSource,
    InitializationSetSyncTiming,
    InitializationSetDspSignalRouting,
    InitializationSetDsp61850SourceAdr,
    InitializationSetDsp61850DestAdr,
    InitializationSetDsp61850PriorityTagged,
    InitializationSetDsp61850EthTypeAppId,
    InitializationSetDsp61850EthSynchronisation,// 30
    InitializationResetMaximum,
    InitializationProtectionOff,
    InitializationReadPCBDeviceVersion,
    InitializationReadPCBServerVersion,
    InitializationReadPCBSerialNr,
    InitializationReadDSPDeviceVersion,
    InitializationReadDSPServerVersion,
    InitializationReadJustdataChksum,
    InitializationTestAdjustment,
    InitializationFinished,// 40
      
    ConfigurationStart,
    
    ConfigurationSetRanges, // block wie bei Initialization...also nicht neu realisiert
    ConfigurationSyncRange0,
    ConfigurationSyncRange01,
    ConfigurationSyncRange02,
    ConfigurationSyncRange1,
    ConfigurationSyncRange11, 
    ConfigurationSyncRange12,
    
    ConfigurationTestIntegrationTime,
    ConfigurationTestDspVarList, // 50
    ConfigurationSetDspVarList, // wie bei Initialization
    ConfigurationSetSamplingSystem, // wie bei Initialization
    ConfigurationTestCmdList,
    ConfigurationSetDspCmdLists, // wie bei Initialization
    ConfigurationActivateCmdLists, // wie bei Initialization
    ConfigurationTestSignalFrequency,
    ConfigurationSetSamplingFrequency, // wie bei Initialization
    ConfigurationTestSRate,
    ConfigurationSetSamplingPSamples, // wie bei Initialization 
    ConfigurationTestSenseMode, // 60
    ConfigurationSetSenseMode, // wie bei Initialization
    ConfigurationTestSyncMode,
    ConfigurationSetSyncSource, // wie bei Initialization
    ConfigurationTestSyncTiming,
    ConfigurationSetSyncTiming, // wie bei Initialization
    ConfigurationTestDspSignalRouting,
    ConfigurationSetDspSignalRouting, // wie bei Initialization
    ConfigurationTestDsp61850SourceAdr,
    ConfigurationSetDsp61850SourceAdr, // wie bei Initialization 
    ConfigurationTestDsp61850DestAdr, // 70
    ConfigurationSetDsp61850DestAdr, // wie bei Initialization
    ConfigurationTestDsp61850PriorityTagged,
    ConfigurationSetDsp61850PriorityTagged, // wie bei Initialization
    ConfigurationTestDsp61850EthTypeAppId,
    ConfigurationSetDsp61850EthTypeAppId, // wie bei Initialization
    ConfigurationTestDsp61850EthSynchronisation,
    ConfigurationSetDsp61850EthSynchronisation, // wie bei Initialization
    ConfigurationFinished,
    
    SetRangeStart,		
    SetRangeSetRanges, // block wie bei Initialization...also nicht neu realisiert 80
    SetRangeSyncRange0,
    SetRangeSyncRange01,
    SetRangeSyncRange02,
    SetRangeSyncRange1,
    SetRangeSyncRange11, 
    SetRangeSyncRange12,
    
    SetRangeResetMaximum, // wie bei Initialization
    SetRangeFinished,
    
    TriggerMeasureStart,
    TriggerMeasureFinished, // 90
    
    TriggerMeasureDataAcquisition,
    TriggerMeasureComputation,
    TriggerMeasureGetGainCorrCh0,
    TriggerMeasureGetGainCorrCh1,
    TriggerMeasureGetPhaseCorrCh0,
    TriggerMeasureGetPhaseCorrCh1,
    TriggerMeasureGetOffsetCorrCh0,
    TriggerMeasureGetOffsetCorrCh1,
    TriggerMeasureCorrection,
    
    MeasureStart, // 100
    MeasureDataAcquisition, // wie bei Trigger.. 
    MeasureComputation, // dito
    MeasureGetGainCorrCh0,
    MeasureGetGainCorrCh1,
    MeasureGetPhaseCorrCh0,
    MeasureGetPhaseCorrCh1,
    MeasureGetOffsetCorrCh0,
    MeasureGetOffsetCorrCh1,
    MeasureCorrection, // hier ist messung zu ende
    
    MeasureLPStart, // 110
    MeasureLPComputation,
    
    RestartMeasurementStart,
    RestartMeasurementResetMaximum,
    RestartMeasurementRestart,
    RestartMeasurementFinished,
    
    RangeObsermaticStart,
    RangeObsermaticTest,
    RangeObsermaticSyncRange0, // block wie bei Initialization...also nicht neu realisiert
    RangeObsermaticSyncRange01,
    RangeObsermaticSyncRange02, // 120
    RangeObsermaticSyncRange1,
    RangeObsermaticSyncRange11, 
    RangeObsermaticSyncRange12,
    RangeObsermaticOverload,
    RangeObsermaticResetMaximum,	
    RangeObsermaticFinished,
  
    SenseProtectionOff,
    SenseProtectionOffRM,
    SenseProtectionOffResetMaximum, // 130
    SenseProtectionOffFinished,
    
    CmpPhaseCoeffStart,
    CmpPhaseCoeffCh0,
    CmpPhaseCoeffCh1,
    CmpPhaseCoeffCh2,
    CmpPhaseSetStatus,
    CmpPhaseSetStatus2,
    CmpPhaseCoeffFinished,

    CmpOffsetCoeffStart,
    CmpOffsetCoeffCh0,
    CmpOffsetCoeffCh1,
    CmpOffsetCoeffCh2, // 140
    CmpOffsetSetStatus,
    CmpOffsetSetStatus2,
    CmpOffsetCoeffFinished,
   
    PhaseNodeMeasStart,
    PhaseNodeMeasCoefficientClearN,
    PhaseNodeMeasCoefficientClearN2,
    PhaseNodeMeasCoefficientClearNFinished,
    PhaseNodeMeasBaseConfiguration,
    PhaseNodeMeasNodeConfig, // für jeden knoten muss etwas umkonfiguriert werden
    PhaseNodeMeasExec1,
    PhaseNodeMeasExec2,
    PhaseNodeMeasExec3, // 150
    PhaseNodeMeasExec4,            
    PhaseNodeMeasExec5,
    PhaseNodeMeasFinished,
    
    OffsetMeasWM3000Start,
    OffsetMeasWM3000CoefficientClearN,
    OffsetMeasWM3000CoefficientClearN2,
    OffsetMeasWM3000CoefficientClearNFinished,
    OffsetMeasWM3000BaseConfiguration,
    OffsetMeasWM3000Exec1, // 160
    OffsetMeasWM3000Exec2,
    OffsetMeasWM3000Exec3,
    OffsetMeasWM3000Exec4,
    OffsetMeasWM3000Exec5,
    OffsetMeasWM3000Exec6,
    OffsetMeasWM3000Finished,

    OffsetMeasWM3000StartVar,
    OffsetMeasWM3000BaseConfigurationVar,
    OffsetMeasWM3000Exec1Var,
    OffsetMeasWM3000Exec2Var, // 170
    OffsetMeasWM3000Exec3Var,
    OffsetMeasWM3000Exec4Var,
    OffsetMeasWM3000Exec5Var,
    OffsetMeasWM3000FinishedVar,

    OffsetMeasChannelNStart,
    OffsetMeasChannelNSync,
    OffsetMeasChannelNFinished,

    OffsetMeasChannelXStart,
    OffsetMeasChannelXSync,
    OffsetMeasChannelXFinished,

    JustageFlashProgStart,
    JustageFlashProgFinished,
    
    JustageFlashExportStart,
    JustageFlashExportFinished,
    
    JustageFlashImportStart,
    JustageFlashImportFinished, // 180
    
    EN61850ReadStatusStart,
    EN61850ReadStatusFinished,
    
    EN61850WriteStatusStart,
    EN61850WriteStatusFinished,
    
    SelftestStart,
    SelftestBaseConfiguration,
    SelftestMeasConfiguration,
    SelftestSetRangeNX,
    SelftestMeasureNSync,
    SelftestMeasureN, // 190
    SelftestMeasureXSync,
    SelftestMeasureX,
    SelftestFinished,
        
    wm3000Continue,
    wm3000Repeat,
};


class tJustValues
{
public:
    tJustValues(){ GainCorrCh0 = 1; GainCorrCh1 = 1; PhaseCorrCh0 = 0; PhaseCorrCh1 = 0; OffsetCorrCh0 = 0; OffsetCorrCh1 = 0; OffsetCorrDevN = 0; OffsetCorrDevX = 0;}
    float GainCorrCh0;
    float GainCorrCh1;
    float PhaseCorrCh0;
    float PhaseCorrCh1;
    float OffsetCorrCh0;
    float OffsetCorrCh1;
    float OffsetCorrDevN;
    float OffsetCorrDevX;
};


class cWMessageBox: public QMessageBox
{
    Q_OBJECT
    
public:
    cWMessageBox();
    cWMessageBox ( const QString & caption, const QString & text, Icon icon, int button0, int button1, int button2, QWidget * parent = 0, const char * name = 0, bool modal = TRUE, Qt::WFlags f = Qt::WStyle_DialogBorder );
    virtual ~cWMessageBox();
    
protected slots:
    virtual void done(int r);
    
signals:
    void WMBoxClosed();
};


class cWM3000U: public QObject
{
    Q_OBJECT

public:
    
    cWM3000U(); 
    virtual ~cWM3000U();
       
    cConfData m_ConfData; // konfigurationsdaten 
    cOwnError *m_pOwnError; // eigenfehler 
    cDspIFace* DspIFace;
    cPCBIFace* PCBIFace;
    void InitWM3000(); // einmal komplett initialisieren
    void setConventional(bool b);
    bool isConventional();
    void setDC(bool b);
    bool isDC();

public slots:
     // slots, die vom hauptfenster aus aktiviert werden
    void GetOETAnalizeDone(void); // eigenfehler object hat seine arbeit erledigt (synchronisation)
    void JustageAmplitudeSlot(void);
    void JustagePhaseSlot(void);
    void JustagePhaseBerechnungSlot(void);
    void JustageFlashProgSlot(void);
    void JustageFlashExportSlot(QString);
    void JustageFlashImportSlot(QString);
    void JustageOffsetSlot(void);
    void JustageOffsetVarSlot();
    void JustageOffsetBerechnungSlot(void);
    void OffsetMessungChannelNSlot(void);
    void OffsetMessungChannelXSlot(void);
    void SelfTestManuell();
    void SelfTestRemote();
    void OffsetMessungChannelNRemote();
    void OffsetMessungChannelXRemote();
    
    // slots, die vom bereichauswahl menu aktiviert werden
    void SetRangeSlot(cConfData*); 
    // slots, die vom konfigurationsmenu aktiviert werden
    void SetConfDataSlot(cConfData*); // konfiguration aus confdialog übernehmen
    bool LoadSettings(QString); // zum lesen der einstellungen ....
    void WriteSettings(QString); // dito zum schreiben
    void StoreResultsSlot(); // abspeichern von messergebnissen
    void EN61850ResetStatusSlot();
    void EN61850InfoRequestSlot(); 
    void DefaultSettingsSlot();
    void DefaultSettingsMeasurementSlot();
    
private slots:
    void ActionHandler(int); // zur serialisierung von actions , arbeitet die actions hintereinander ab
    // slot für messung und bereich obsermatic werden vom Timer aktiviert
    void MeasureSlot(); // messung 
    void MeasureLPSlot(); // messung rms n für lastpunkt
    void RangeObsermaticSlot(); // bereich überwachung und automatik
    void DspIFaceAsyncDataSlot(const QString&); // für asynchrone meldungen vom dsp server
    void XIFaceDoneSlot(); // wenn gültige daten da sind vom einen oder anderen server
    void DspIFaceErrorSlot(); // wenn fehler aufgetreten sind am dsp server
    void pcbIFaceErrorSlot(); // dito für leitenkarten server
    void PhaseJustSyncSlot(); // zur synchronisation des justageablaufes mit configuration und messung
    void OffsetJustSyncSlot();
    void JustAbortSlot();
    void SelftestSyncSlot(); // zur synchronisation des selbsttest ablaufes mit configuration und messung
    void SelftestAbortSlot();
    void OverLoadMaxQuitSlot();
    
    
signals:    
    void JustifiedSignal(bool); // signal an hauptview ob justiert oder nicht
    void SendConfDataSignal(cConfData*); // konfigurationsdaten senden
    void SendOEDataSignal(cOwnError*); // eigenfehlerdaten senden
    void SendActValuesSignal(cwmActValues*); // istwerte senden
    void SendJustValuesSignal(tJustValues*);
    void SendLPSignal(cwmActValues*); // es werden alle istwerte versendet, es sollaber nur der lp aktualisiert werden
    void SendIOStringSignal(QString); //  scpi input/output senden
    void SendLogFileSizeSignal(const long); // logfile grösse senden
    void SendConfDialogInfoSignal(QStringList&,QStringList&); // wandler primär/sekundär listen senden
    void SendRangeListSignal(cWMRangeList&,cWMRangeList&); // bereichauswahllisten  senden 
    void AbortProgramSignal(); // für einen programmabbruch
    void StartStateMachine(int);
    void EN61850StatusSignal(cEN61850Info*);
    void ConfigReady(); // zur sync. des phasejustage ablaufes
    void MeasureReady(); // dito
    void SetRangeReady(); // dito
    void RangeAutomaticDone(); // info dass bereichautomatik 1x aktiv war
    void SendVersionInfo(tVersSerial*); // wir versenden die versions , serial , chksum info 
    void AffectStatus(uchar, ushort); // zum setzen, rücksetzen der scpi status systeme
    void SelftestDone(int); // 0 ok  -1 fehler
    void OffsetValue(double);
    void FreqQuestionable(bool);

protected:
    tVersSerial SerialVersions; // serien nummer und version
    
    // listen für bereiche der messeinrichtung und auswahlwerte hierzu 
    cWMRangeList m_sNRangeList; // liste von absolut bereichen kanal n
    cWMRangeList m_sXRangeList; // liste von absolut bereichen kanal x
    cWMRangeList m_sEVTRangeList; // liste für die bereiche der elektronischen wandler
    cCalcInfoList m_CalcInfoList; // liste mit bereichen, deren phasenkorrektur koeffizienten zu berechnen sind (kanal, bereich)

    cPhaseNodeMeasInfoList m_PhaseNodeMeasInfoList; // liste mit infos für den justageablauf siehe cJustNodeMeasInfo (wmglobal)
    cOffsetMeasInfoList m_OffsetMeasInfoList; // dito
    QStringList m_SelftestInfoList;
    cTCPConfig TCPConfig; // information zur verbindung zu den servern
    
private:
    bool m_binitDone;
    bool m_bIFError; // zum feststellen ob bei der letzten aktion ein fehler war
    // cWM3000SCPIFace* m_pWM3000SCPIFace;
    // cZeraDeviceServer* m_pZeraDeviceServer;
    
    void DefaultSettings(cConfData&);
    void DefaultSettingsMeasurement(cConfData&);
    void ServerIFaceErrorHandling(int, QString, int);
    cSMTimer* m_ActTimer; // für actionhandler ablaufsteuerung
    cSMTimer* m_AsyncTimer; // für actionhandler ablaufsteuerung
    Q3ValueList<int> AHSFifo;
     
    cConfData m_ConfDataCopy; // kopie der konfigurationsdaten 
    cConfData SaveConfData; // retten der konfiguration während des justageablaufes
    cConfData NewConfData; // zum setzen der konfiguration aus justageablauf  
    bool SelectRange(cWMRangeList&,QString&,QString&,QString&,float,bool&); // sucht aus der liste der bereiche und soll,ist,vorgabe werte und dem max. einen bereich, rückgabe -> zwingende änderung
    CWMRange* Range(cRSelectString,cWMRangeList&); // sucht den bereich über den selector
    CWMRange* Range(QString,cWMRangeList&); // sucht den bereich über den namen
    CWMRange* Range(float,cWMRangeList&); // sucht einen beeich dessen bereichendwert >= float par ist
    CWMRange* DummyRange;
 
    void SetDspWMVarList();
    void SetDspWMCmdList();

    void SetPhaseCalcInfo();
    void SetPhaseNodeMeasInfo();
    void SetOffsetCalcInfo();
    void SetOffsetMeasInfo(int te, int tm);
    void offsetCorrectionHash2File();
    void readOffsetCorrectionFile();
    void SetSelfTestInfo(bool); // manuell oder remote
    int signum(double value);
    
    cMovingWindowFilter m_MovingWindowFilter;
    QTimer *MeasureTimer;
    QTimer *MeasureLPTimer; 
    QTimer *RangeTimer;
    
    void StopMeasurement();

    void SimulatedMeasurement();
    void CmpActFrequency();
    void CmpActValues();
    void CmpRMSValues();
    void CorrActValues();
    void CorrRMSValues();

    bool m_bJust;
    bool m_bStopped;
    bool m_bDspMeasureTriggerActive;
    bool m_bDspMeasureIgnore;
    tJustValues m_JustValues;
    cDspMeasData* ActValData;
    cDspMeasData* MaxValData;
    cDspMeasData* RMSValData; // schnelle rms messung (4..26 * 20ms @ 50hz)
    cDspMeasData* ETHStatusHandle;
    cDspMeasData* ETHStatusResetHandle;
    cwmActValues ActValues;
    cDspMaxValues MaxValues;
    Q3ProgressDialog *m_pProgressDialog;
    /*wm3000ActionHandlerState*/ int m_PhaseNodeMeasState; // hier merken wir uns wo´s weiter geht
    /*wm3000ActionHandlerState*/ int m_OffsetMeasState; // hier merken wir uns wo´s weiter geht
    /*wm3000ActionHandlerState*/ int m_SelftestState; // hier merken wir uns wo´s weiter geht
    Q3ValueList<float> JustValueList;
    QHash<QString, double> adjOffsetCorrectionHash; // offset correction is in lsb !!!!!
    QHash<QString, double> measOffsetCorrectionHash; // this hash is filled from file ... i think
    QString m_sJustText;
    QPushButton* m_pAbortButton;
    QString JDataFile;
    QFile m_SelftestLogfile;
    QFile m_PhaseJustLogfile;
    QFile m_OffsetJustLogfile;
    QFile m_OffsetDatafile;
    QFile m_NSAOffsetDatafile;
    cWMessageBox *m_OVLMsgBox;
    cWMessageBox *m_SelftestMsgBox;
    bool m_bConventional;
    bool m_bDC;
    bool m_bNoDCAdjust;
};

#endif    

