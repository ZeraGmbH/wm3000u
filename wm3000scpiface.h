// header datei für wm3000 scpi interface klasse
// start 08.10.09 Peter Lohmer

#ifndef WM3000SCPIFACE_H
#define WM3000SCPIFACE_H

#include <qstringlist.h>

#include "tools.h"
#include "wmglobal.h"
#include "confdata.h"
#include "scpiface.h"
#include "en61850.h"

enum wm3000SCPICmdType  { nixWCmd = LastCommonCommand, // 13
			
			  // alle scpi commands
			  // implementiertes system modell
			  
			  GetScpiErrorAll,
			  GetScpiErrorCount,
			  GetScpiError,
			  GetSerialNumber,
			  GetDSPVersion,
			  GetPCBVersion,
			  GetDeviceVersion,
  
			  // implementiertes status modell
			  
			  SetStatusEN61850Clear,
			  GetStatusEN61850SynclostCount,			  
			  GetStatusEN61850Error,
			  GetStatusEN61850DataCount,			  
			  
			  SetStatusQuestionablePTrans, // 23
			  SetStatusQuestionableNTrans,
			  SetStatusQuestionableEnable,
			  SetStatusQuestionableCondition,
			  GetStatusQuestionablePTrans,
			  GetStatusQuestionableNTrans,
			  GetStatusQuestionableEnable,
			  GetStatusQuestionableCondition,
			  GetStatusQuestionableEvent,
			  			  
			  SetStatusOperationPTrans,
			  SetStatusOperationNTrans, // 33
			  SetStatusOperationEnable,
			  SetStatusOperationCondition,
			  GetStatusOperationPTrans,
			  GetStatusOperationNTrans,
			  GetStatusOperationEnable,
			  GetStatusOperationCondition,
			  GetStatusOperationEvent,
              GetStatusStandard,

              // implementiertes store model

              GetChannelNOffsetCmd,
              GetChannelXOffsetCmd,
			  
			  // implementiertes sense model    
    
			  OutRangeCatalog,
			  SetRange, // 43
			  GetRange,
			  OutChannelCatalog,
    
			  // implementiertes measure model 
    
			  MeasurementRead,
			  MeasurementReadLoadpoint,
			  MeasurementConfigure,	         
			  MeasurementFetch,	         
			  MeasurementInitiate,	     
			  Measurement,
     
			  // implementiertes configuration model 
			      
//			  GetConfLogFileSize,
//			  SetConfLogFileSize, // 53
			  ConfApply,
			  ConfEnStatusClear,			    
			  GetConfEnStatus,			    
			  GetConfEnAppid,
			  SetConfEnAppid,
			  GetConfEnVid,
			  SetConfEnVid,
			  GetConfEnCfi,
			  SetConfEnCfi,
			  GetConfEnUPriority,
			  SetConfEnUPriority, // 63
              GetConfENFAsdu,
              SetConfENFAsdu,
              GetConfENLAsdu,
              SetConfENLAsdu,
			  GetConfEnDSet,
			  SetConfEnDSet,
			  GetConfENMAdrWM3000,
			  SetConfENMAdrWM3000,
			  GetConfENMAdrMU,
			  SetConfENMAdrMU,
			  GetConfRatioEvt, // 73
			  SetConfRatioEvt,
			  GetConfRatioChx,
			  SetConfRatioChx,
			  GetConfRatioChn,
			  SetConfRatioChn,
              GetConfSyncStrong,
              SetConfSyncStrong,
			  GetConfSyncPeriod,
			  SetConfSyncPeriod,
			  GetConfSyncSource,
			  SetConfSyncSource,
			  GetConfMeasTInt, // 83
			  SetConfMeasTInt,
			  GetConfMeasMPeriod,
			  SetConfMeasMPeriod,
			  GetConfMeasSRate,
			  SetConfMeasSRate,
			  GetConfMeasSFreq,
			  SetConfMeasSFreq,
			  GetConfCompPhcTime,
			  SetConfCompPhcTime,
			  GetConfCompPhcPhase, // 93
			  SetConfCompPhcPhase,
			  GetConfCompOecFile,
			  SetConfCompOecFile,
			  GetConfCompOecOn,
			  SetConfCompOecOn,
              GetConfCompOffskN,
              SetConfCompOffskN,
              GetConfCompOffskX,
              SetConfCompOffskX,
//			  GetConfCompModeCatalog,
//			  SetConfCompMode,
//			  GetConfCompMode,
			  GetConfOperModeCatalog,
			  SetConfOperMode, // 103
              GetConfOperMode,
              GetConfOperSignalCatalog,
              SetConfOperSignal,
              GetConfOperSignal
    		      };


enum wmExecuteCommandStates   { EN61850SynclostCountStart = ExecCmdLast,
			             EN61850SynclostCountFinished,
				     
			             EN61850ErrorStart,
			             EN61850ErrorFinished,
				     
			             EN61850DataCountStart,
			             EN61850DataCountFinished,
				     
			             MeasStart,
			             MeasConfiguration,  
 			             MeasInitiate,
			             MeasFetch,
			             MeasFinished,
				     
			             ConfigStart,
 			             ConfigConfiguration,
			             ConfigFinished,
				     
			             InitiateStart,
			             InitiateFinished,
				     
			             FetchStart, // finished ist von selbst
				     
			             ReadStart,
			             ReadRead,
				     
			             ReadLPStart,
			             ReadLPRead,
				     
			             ifSelftestStart,
                         ifSelftestFinished,

                         ifChannelNOffsetStart,
                         ifChannelNOffsetFinished,

                         ifChannelXOffsetStart,
                         ifChannelXOffsetFinished,
				     
				 };


enum wait4What { wait4Nothing, 
	             wait4ENStatus,
	             wait4RangeAutomatic,
	             wait4MeasurementData,
	             wait4LoadpointData,
                 wait4SelftestResult,
                 wait4Offsetresult,
		 };

class cWM3000SCPIFace: public cSCPIFace
{
    Q_OBJECT
    
public:
    cWM3000SCPIFace(cClientIODevice*, short); //  das zugehörige io device, länge für die eventqueue
    virtual void SCPICmd( int, char*);
    virtual char* SCPIQuery( int, char*);
    virtual char* GetDeviceIdentification();
    virtual char* DeviceSelfTest();
    virtual void ResetDevice();
    virtual char* GetChannelNOffset();
    virtual char* GetChannelXOffset();

    void setConfiguration(cConfData*); // beim einrichten des interface 1x aufrufen ...
    // im gegensatz zu receiveconfiguration (slot) werden hier actual und target conf. gesetzt
    
public slots:
    void ReceiveConfiguration(cConfData*);
    void ReceiveEN61850Status(cEN61850Info*);
    void SetRangeListSlot( cWMRangeList&,  cWMRangeList&);
    void ReceiveRangeAutomaticRdy();
    void ReceiveActValues(cwmActValues*);
    void ReceiveLPValue(cwmActValues*);
    void ReceiveVersionInfo(tVersSerial*);
    void ReceiveSelftestResult(int);
    void ReceiveNXOffset(double offs);
    
protected slots:    
    virtual void ExecuteCommand(int); // ausführen kommandos statemachine
    virtual void CmdExecution(QString&);    
        
signals:
    void SendConfiguration(cConfData*);
    void SendRange(cConfData*);
    void EN61850StatusRequest();
    void ResetETHStatus();
    void SetDefaultMeasConfig();
    void SelftestRequest();
    void ChannelNOffsetMeasureRequest();
    void ChannelXOffsetMeasureRequest();
        
private:
    int SelftestResult;
    double OffsetResult;
    tVersSerial* m_pVersion;
    cwmActValues mActValues;
    QStringList m_sNXItemList;
    QStringList m_sEVTItemList;
    QStringList mMeasChannelList; // liste aller messkanäle
    int m_nWait4What;
    bool m_bAddEventError;
    cEN61850Info m_EN61850Info;
    QStringList EXSFifo;
    cNode* InitScpiCmdTree(cNode*);
    cConfData m_ConfDataActual, m_ConfDataTarget;
    bool GetParameter(char**, ushort&, bool); // zeiger auf input ,  der gefundene wert, test auf blank?
    bool GetParameter(char**, ushort&, int, int, int, bool); // zeiger auf input ,  der gefundene wert, min, max, base, test auf blank?
    bool GetParameter(char**, double&, double, double, bool); // zeiger auf input ,  der gefundene wert, min, max ,test auf blank?
    bool GetParameter(char**, QString&,bool); // zeiger auf input, der gefundene string, test auf blank?
    bool GetTransformerRatio(char**, QString&, QString&, bool);
    void SetMacAdress(char**, cETHAdress&);
    bool SearchEntry(char**, char**, int, int&, bool); // suche, worin, wieviele einträge, eintrag, test auf    blank
    bool SearchEntry(char**, double*, int, int&, bool); // suche, worin, wieviele einträge, eintrag, test auf    blank
    bool SearchEntry(char**, int*, int, int&, bool); // suche, worin, wieviele einträge, eintrag, test auf    blank
     bool SearchFile(char*, bool); // suche, kompletter filename , test auf blank
     
    // system modell routinen
    char* mGetScpiErrorAll();
    char* mGetScpiErrorCount();
    char* mGetScpiError();
    char* mGetSerialNumber();
    char* mGetDSPVersion();
    char* mGetPCBVersion();
    char* mGetDeviceVersion();
  
    // status modell routinen
    void mSetStatusEN61850Clear(char*);
    char*  mGetStatusEN61850SynclostCount();
    char* mGetStatusEN61850Error();
    char* mGetStatusEN61850DataCount(); 
    void mSetStatusQuestionablePTrans(char*);
    char* mGetStatusQuestionablePTrans();
    void mSetStatusQuestionableNTrans(char*);
    char* mGetStatusQuestionableNTrans();
    void mSetStatusQuestionableEnable(char*);
    char* mGetStatusQuestionableEnable();
    void mSetStatusQuestionableCondition(char*);
    char* mGetStatusQuestionableCondition();
    char* mGetStatusQuestionableEvent();
        
    void mSetStatusOperationPTrans(char*);
    char* mGetStatusOperationPTrans();
    void mSetStatusOperationNTrans(char*);
    char* mGetStatusOperationNTrans();
    void mSetStatusOperationEnable(char*);
    char* mGetStatusOperationEnable();
    void mSetStatusOperationCondition(char*);
    char* mGetStatusOperationCondition();
    char* mGetStatusOperationEvent();
    char* mGetStatusStandard();
			  
    // sense model routinen    
    char* mOutRangeCatalog();
    void mSetRange(char*);
    char* mGetRange();
    char* mOutChannelCatalog();
    
    // implementiertes measure model 
    char* mMeasurementRead();
    char* mMeasurementReadLoadpoint();
    void mMeasurementConfigure(char*);
    char* mMeasurementFetch();	         
    void mMeasurementInitiate(char*);
    char* mMeasurement();
     
    // implementiertes configuration model 
    void mConfigurationApply(char*);
//    char* mGetConfLogFileSize();
//    void mSetConfLogFileSize(char*);
    char* mGetConfEnAppid();
    void mSetConfEnAppid(char*);
    char* mGetConfEnVid();
    void mSetConfEnVid(char*);
    char* mGetConfEnCfi();
    void mSetConfEnCfi(char*);
    char* mGetConfEnUPriority();
    void mSetConfEnUPriority(char*);
    char* mGetConfENFAsdu();
    void mSetConfENFAsdu(char*);
    char* mGetConfENLAsdu();
    void mSetConfENLAsdu(char*);
    char* mGetConfEnDSet();
    void mSetConfEnDSet(char*);
    char* mGetConfENMAdrWM3000();
    void mSetConfENMAdrWM3000(char*);
    char* mGetConfENMAdrMU();
    void mSetConfENMAdrMU(char*);
    char* mGetConfRatioEvt();
    void mSetConfRatioEvt(char*);
    char* mGetConfRatioChx();
    void mSetConfRatioChx(char*);
    char* mGetConfRatioChn();
    void mSetConfRatioChn(char*);
    char* mGetConfSyncStrong();
    void mSetConfSyncStrong(char*);
    char* mGetConfSyncPeriod();
    void mSetConfSyncPeriod(char*);
    char* mGetConfSyncSource();
    void mSetConfSyncSource(char*);
    char* mGetConfMeasTInt();
    void mSetConfMeasTInt(char*);
    char* mGetConfMeasMPeriod();
    void mSetConfMeasMPeriod(char*);
    char* mGetConfMeasSRate();
    void mSetConfMeasSRate(char*);
    char* mGetConfMeasSFreq();
    void mSetConfMeasSFreq(char*);
    char* mGetConfCompPhcTime();
    void mSetConfCompPhcTime(char*);
    char* mGetConfCompPhcPhase();
    void mSetConfCompPhcPhase(char*);
    char* mGetConfCompOecFile();
    void mSetConfCompOecFile(char*);
    char* mGetConfCompOecOn();
    char* mGetConfCompOffskN();
    char* mGetConfCompOffskX();
    void mSetConfCompOecOn(char*);
    void mSetConfCompOffskN(char*);
    void mSetConfCompOffskX(char*);
//    char* mGetConfCompModeCatalog();
//    void mSetConfCompMode(char*);
//    char* mGetConfCompMode();
    char* mGetConfOperModeCatalog();
    void mSetConfOperMode(char*);
    char* mGetConfOperMode();
    char* mGetConfOperSignalCatalog();
    void mSetConfOperSignal(char*s);
    char* mGetConfOperSignal();
   
};

#endif
