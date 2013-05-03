// headerdatei dspiface.h 
// deklaration einer dsp interface klasse

#ifndef DSPIFACE_H
#define DSPIFACE_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3ptrlist.h>
//Added by qt3to4:
#include <Q3MemArray>

#include "tools.h"
#include "ethadress.h"
#include "zhserveriface.h"
#include "zhclientsocket.h"


enum dType {dInt, dFloat};

enum AVType {vApplication = 1, vDspIntern = 2, vMemory = 4};

class cDspVar // dsp variable 
{
public:
    cDspVar(QString name, int size, int type)
	: m_sName(name), m_nsize(size), m_nType(type){};
    QString& Name() { return m_sName;};
    int size() { return m_nsize; };
    int type() { return m_nType; };
private:    
    QString m_sName; // hat einen namen
    int m_nsize; // besitzt eine anzahl v. elementen
    int m_nType; // ist vapplication und/oder vdspintern
};    


class cDspMeasData
{
public:
    cDspMeasData(QString); // name des messdaten satzes
    float* data();
    void addVarItem(cDspVar*);
    QString& VarList(); // liste mit allen variablen mit längenangaben
    QString& MeasVarList(); // liste aller namen vom typ vapplikation bzw. vmemory
    QString& name();
    
private:
    Q3PtrList<cDspVar> DspVarList;
    Q3MemArray<float> DspVarData;
    QString m_sname;
    QString m_slist;
};


enum dspIFaceActionHandlerState {
    dspIFaceIdle, // 0
    
    dspIFaceConnectYourselfStart,
    dspIFaceConnectionFinished,
    
    BootDspStart,
    BootDspTestRunning,
    BootDspReset,
    BootDspSetPath,
    BootDspExecute,
    BootDspFinished,
    
    SetSamplingSystemStart,
    SetSamplingSystemFinished, // 10
    
    VarLists2DspStart,
    VarLists2DspFinished,
    
    CmdLists2DspStart,
    IntCmdList2Dsp,
    CmdLists2DspFinished,
    
    SetSignalRoutingStart,
    SetSignalRoutingFinished,
    
    ResetMaximumStart,
    ResetMaximumExec,
    ResetMaximumFinished, // 20
    
    SetDsp61850SourceAdrStart,
    SetDsp61850SourceAdrExec,
    SetDsp61850SourceAdrWaitAutoNeg,
    SetDsp61850SourceAdrFinished,
    
    SetDsp61850DestAdrStart,
    SetDsp61850DestAdrExec,
    SetDsp61850DestAdrWaitAutoNeg,
    SetDsp61850DestAdrFinished,
    
    SetDsp61850PriorityTaggedStart,
    SetDsp61850PriorityTaggedExec, // 30
    SetDsp61850PriorityTaggedFinished,
    
    SetDsp61850EthTypeAppIdStart, 
    SetDsp61850EthTypeAppIdExec,
    SetDsp61850EthTypeAppIdFinished,
       
    SetDsp61850EthSyncStart,
    SetDsp61850EthSyncExec,
    SetDsp61850EthSyncFinished,
    
    TriggerIntHKSKStart,
    TriggerIntHKSKExec,
    TriggerIntHKSKFinished, // 40
    
    DataAcquisitionStart, 
    DataAcquisitionFinished,
    
    DspMemoryReadStart,
    DspMemoryReadFinished,
    
    DspMemoryWriteStart,
    DspMemoryWriteFinished,
    
    ActivateInterfaceStart,
    ActivateInterfaceFinished,
    
    DeactivateInterfaceStart,
    DeactivateInterfaceFinished,
    
    ReadDeviceVersionStart,
    ReadDeviceVersionFinished,
    
    ReadServerVersionStart,
    ReadServerVersionFinished,
    
    dspIFaceContinue
};


class cDspIFace: public cZHServerIFace
{
    Q_OBJECT
    
public:
    cDspIFace(QString, QString, int);
    ~cDspIFace();
    void connectYourself(); // verbinde dich mit deinem server
    bool connected();
    void BootDsp(); // aussführen des bootvorgangs  
    void SetSamplingSystem(int,int,int); // anzahl kanäle, samples/signalperiode, samples/messperiode
    void VarList2Dsp(); // variablenlisten zum server senden
    void CmdLists2Dsp(); // sendet cyc- und intliste ad den dsp server
    void ClearCmdList(); // löscht alle cmdlisten
    void ClearVarLists(); // löscht die varliste
    void ClearMemLists(); 
    void SetSignalRouting(ulong*); // zeiger auf ein 8*32bit routing feld 
    void SetDsp61850SourceAdr(cETHAdress&);
    void SetDsp61850DestAdr(cETHAdress&);
    void SetDsp61850PriorityTagged(ulong); // setzt die einträge für den eth frame decoder
    void SetDsp61850EthTypeAppId(ulong); // dito
    void SetDsp61850EthSynchronisation(ulong); // setzt ob harte sync. und auf welches dataset
    void ResetMaximum(); // löscht den maximum speicher
    void TriggerIntHKSK(int); // triggert die abarbeitung hksk in intliste
    void addCycListItem(QString&); // ein neues Kommando ans ende der cyc. liste
    void addIntListItem(QString&); // dito für intliste
    cDspMeasData* GetMVHandle(QString); // legt eine neue messwerte gruppe an 
    cDspMeasData* GetMemHandle(QString); // legt eine neue memory gruppe an 
    void addVarItem(cDspMeasData*,cDspVar*); // eine neue dsp variable
    void ActivateInterface(); // aktiviert die var- und cmd-listen im dsp
    void DeactivateInterface(); // nur anders rum
    void DataAcquisition(cDspMeasData*); // liest alle daten dieser messwertegruppe vom type vapplication
    void DspMemoryRead(cDspMeasData*); // liest alle daten dieser memorygruppe
    void DspMemoryWrite(cDspMeasData*,dType); // schreibt alle daten dieser memorygruppe
    float* data(cDspMeasData*); // gibt einen zeiger zurück auf die var daten vom typ vapplication
    void SetGainCorrection(int,float); // setzt für kanal (int 0..) die verstärkungskorrektur
    void SetPhaseCorrection(int,float); // setzt für kanal (int 0..) die phasenkorrektur    
    void SetOffsetCorrection(int,float); // setzt für kanal (int 0..) die offsetkorrektur
    bool IFaceError(); // abfrage ob bei der letzten aktion fehler aufgetreten ist
    void ReadDeviceVersion();
    void ReadServerVersion();
    
    
protected slots:
    void ActionHandler(int);
    virtual void SocketConnectedSlot();
    virtual void SocketDoneSlot();    
    virtual void SocketErrorSlot();
    
private:
    QString dspBootPath; // vollständiger programmname
    QString m_sHost; // host name
    int m_nPort; // host port
    QStringList CycCmdList, IntCmdList;
    Q3PtrList<cDspMeasData> DspMeasDataList; // eine liste mit zeigern auf "programmdaten"
    Q3PtrList<cDspMeasData> DspMemoryDataList; // eine liste mit zeigern auf  dsp speicher allgemein
    cSMTimer* m_ActTimer;
    void GetInterfaceData();
    void TestDspRunning(); // schreibt test kommando an dsp
    void SetDspBootPath(); // schreibt kommando für bootpath zu setzen
    void DspReset(); 
    void SendBootCommand(); // sendet das bootkommando an den server
    void SendSamplingSystemCommand(); // na was wohl ?
    void SendVarListCommand(); // und hier ????
    void SendCmdListCommand();
    void SendCmdIntListCommand();
    void SendSignalRoutingCommand();
    void SendResetMaximumCommand();
    void SendSetDsp61850SourceAdrCommand();
    void SendSetDsp61850DestAdrCommand();
    void SendSetDsp61850PriorityTaggedCommand();
    void SendSetDsp61850EthTypeAppIdCommand();
    void SendSetDsp61850EthSyncCommand();
    void SendTriggerIntHKSKCommand();
    void SendDataAcquisitionCommand();
    void SendDspMemoryReadCommand();
    void SendDspMemoryWriteCommand();
    void SendActivateInterfaceCommand();
    void SendDeactivateInterfaceCommand();
    void SendReadDeviceVersionCommand();
    void SendReadServerVersionCommand();
    
    bool m_biFaceError;
    int m_nP1, m_nP2, m_nP3; // parameter
    ulong m_nuP1;
    ulong m_lP1[16]; // parameter
    cETHAdress m_ethadr;
    cDspMeasData *m_pMeasData; 
    int m_nBusyCount;
    bool m_bConnected;
};
    
#endif
    
