// header datei für scpi interface klasse
// start 22.09.09 Peter Lohmer

#ifndef SCPIFACE_H
#define SCPIFACE_H

#include <qobject.h>
#include <qtimer.h>
//Added by qt3to4:
#include <Q3ValueList>

#include "tools.h"
#include "cbiface.h"
#include "scpi.h"
#include "cmdinterpret.h"


struct scpiErrorType
{
    short ErrNum; // fehler nummer (negativ)
    char* ErrTxt; // fehler text
};


enum scpiErrorInd {  NoError,
                              CommandError,
		  InvalidSeparator,   
		  ParameterNotAllowed,
		  MissingParameter,
		  UndefinedHeader,	
		  NumericDataError,
		  
		  ExecutionError,
		  CommandProtected,
		  HardwareError,
		  HardwareMissing,
		  FileNameError,
		
		  DeviceSpecificError,
		  systemError,
		  QueueOverflow,
		  QueryError,
		  PowerOn,
		  scpiLastError};
		  

enum stbBits {   STBbit0 = 1, 
	          STBbit1 = 2, 
	          STBeeQueueNotEmpty = 4,
	          STBques = 8, 
	          STBmav = 16, 
	          STBesb = 32, 
	          STBrqs = 64, 
	          STBoper = 128};


enum sesrBits {   SESROperationComplete = 1, 
	            SESRRequestControl = 2, 
	            SESRQueryError = 4,
	            SESRDevDepError = 8, 
	            SESRExecError = 16, 
	            SESRCommandError = 32, 
	            SESRUserRequest = 64, 
	            SESRPowerOn = 128};


enum opcStates { OCIS, // operation idle state
	             OCAS, // operation active state
	             OQIS, // operation query idle state
	             OQAS }; // operation query active state

class cSCPIFace; // forward

class cSCPIStatSyst {
public:
    cSCPIStatSyst(cSCPIFace* , uchar); // callback , bit fürs statusbyte 
    void SetCondition(ushort); // überschreibt alle bits ...also ev. vorher lesen und 
    ushort GetCondition(); 
    void SetNTransition(ushort);
    ushort GetNTransition();
    void SetPTransition(ushort);
    ushort GetPTransition();
    void SetEnable(ushort);
    ushort GetEnable();
    ushort GetEvent();
    void SetConditionBit(ushort);
    void ResetConditionBit(ushort);

private:
    cSCPIFace* m_myIFace;
    uchar m_nStatBit; // bit position im status byte
    ushort m_nCondition; // wenn ein Conditionbit gesetzt bzw. rückgesetzt
    ushort m_nPTransition; // und pos. flanke enabled
    ushort m_nNTransition; // bzw. neg. flanke enabled
    ushort m_nEvent; // dann wird dies zu einem event 
    ushort m_nEnable; // wenn enabled führt event zum setzen von 
    
};


enum CommonCmdType  {	nixCmd, // 0 is nix
			
			// alle common commands
    
                                          IdentQuery, // *IDN?
			ResetCmd, // *RST
			SelfTestQuery, // *TST?
			OperationCompleteCmd, // *OPC
			OperationCompleteQuery, // *OPC?
			ClearStatusCmd, // *CLS
			StdEventStatEnableCmd, // *ESE
			StdEventStatEnableQuery, // *ESE?
			StdEventStatRegQuery, // *ESR?
			ServiceRequestEnableCmd, // *SRE
			ServiceRequestEnableQuery, // *SRE?
			StatusByteQuery, // *STB?
			LastCommonCommand // 13
		      };


enum ExecuteCommandStates   { ExecCmdIdle,
			      
			         ExecCmdStart,
			         ExecCmdPart,
			         ExecCmdPartFinished, // teikommando fehlerfrei bearbeitet
			         ExecCmdFinished, // komplettes kommando bearbeitet 	 
				 
			         ExecCmdContinue, // wir machen da weiter wo wir sind
				 
			         ExecCmdLast	 
			     };

enum AffectStatusCode { SetOperStat,
		          ResetOperStat,
		          SetQuestStat,
		          ResetQuestStat
		        };	  


enum OperationStates { OperRanging = 4,
		        OperMeasuring = 0x10,
                                    OperConfiguring = 0x100
		      };


enum QuestionStates { QuestServerProblems = 0x100,
		       QuestOverLoad = 0x200,
		       QuestOverLoadMax = 0x400,
		       QuestNotJustified = 0x800,
               QuestFrequency = 0x1000
		     };



class cCmdTimer: public QTimer
{
    Q_OBJECT
    
public:
     cCmdTimer(QObject * parent = 0, const char * name = 0 );
     int start(int, QString&);
     
signals:
     void Command2Execute(QString&);
     
private slots:
    void TimeExpired();
    
private:
    QString m_sCmd;
};

class cClientIODevice; // forward

class cSCPIFace : public QObject, public cbIFace  // virtuelle basisklasse eines scpi interfaces
{
    Q_OBJECT
    
public:
    cSCPIFace(cClientIODevice*, short); //  zeiger auf das zugehörige io device, länge für die eventqueue
    ~cSCPIFace();
    virtual void SCPICmd( int,  char*);
    virtual char* SCPIQuery( int, char*);
    cClientIODevice* GetClientIODevice();
    void AddEventError(int); // fehler bzw. event eintrag in queue
    void ClearEventError(); // alle einträge in queue löschen und mav bit = 0
    cSCPIStatSyst* m_pOperationStat; // public -> ein gerät das ein scpi interface hat kann die stati setzen
    cSCPIStatSyst* m_pQuestionableStat; // diese 2 stati sind immer vorhanden
    
    virtual void ResetDevice(); // wird von common cmd aufgerufen, virt. falls abgel. klasse auch was 
    virtual void DeviceClearStatus(); // rückzusetzen oder zu löschen hat
    virtual void OPCCommand(); // virt. falls ein device nicht nur sequentielle kommandos bearbeiten soll
    virtual char* OPCQuery(); // das gilt auch für die abfrage 
    void SetSTB(uchar); // status register setzen  ; hier alles einzeln weil setzen bzw. rücksetzen 
    void ResetSTB(uchar); // bzw. rücksetzen ;       kann weitere massnahmen zur folge haben
    void SetSRE(uchar); // service request register setzen
    void ResetSRE(uchar); // bzw. rücksetzen
    void SetESR(uchar); // standard event status register setzen
    void ResetESR(uchar); // bzw. rücksetzen
    void SetESE(uchar); // standard event enable register setzen
    void ResetESE(uchar); // bzw. rücksetzen
    
    // die routinen für die common commands
    void DeviceReset(char*);
    void SetDeviceOPC(char*);
    void DeviceClearStatus(char*);
    void SetDeviceESE(char*);
    void SetDeviceSRE(char*);
    virtual char* GetDeviceIdentification();
    virtual char* DeviceSelfTest(); 
    char* GetDeviceOPC();
    char* GetDeviceESE();
    char* GetDeviceESR();
    char* GetDeviceSRE();
    char* GetDeviceSTB();
    void setOPCQState(opcStates state);
    
    
public slots:
    void ReceiveCommand(QString&); // das kommando
    void ReceivePriority(int); // 0 ist die höchste priorität und darf alles
    void AffectSCPIStatus(uchar, ushort); 
    
protected slots:    
    virtual void ExecuteCommand(int)=0; // ausführen eines common kommandos statemachine
    virtual void CmdExecution(QString&) = 0;
    
protected:
    char* answ;
    cCmdTimer* m_pCmdTimer;
    cSMTimer* m_pSMachineTimer;
    bool isAuthorized();
    cNode* m_pCommands;
    cParse* m_pParser;
    cCmdInterpreter* m_pCmdInterpreter;
    QString m_sCommand;
    QString m_sAnswer; // wir speichern antworten zwischen
    void SetIEEE488Register(char*, uchar&);
    void SetnoOperFlag(bool);
    bool m_bCmdError; // kommando fehler speicher
    Q3ValueList<int> m_ErrEventQueue;
    
    
signals:
    void SendAnswer(QString&);     
     
private:
    int m_nPriority;
    cClientIODevice*  m_pCIOD; // pointer io device
    char* RegConversion(uchar);
    ushort m_nQueueLen; // max. anzahl einträge in obiger queue
    
    uchar m_nSTB; // status byte !!!!!! für diese 4 register immer (ausnahme konstruktor) funktion verwenden !!!!!!!
    uchar m_nSRE; // service reguest enable
    uchar m_nESR; // standard event status register (immer vorhanden)
    uchar m_nESE; // standard event status enable (immer vorhanden)
    opcStates m_nOPCState;
    opcStates m_nOPCQState;
    bool m_bnoOperationPendingFlag; // immer funktion zum setzen verwenden !!!!!!!
    
    cNode* InitCommonCmdTree(cNode*);
};

#endif
