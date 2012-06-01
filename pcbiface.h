// headerdatei pcbiface.h 
// deklaration einer leiterkarten interface klasse

#ifndef PCBIFACE_H
#define PCBIFACE_H

#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <q3ptrlist.h>

#include "tools.h"
#include "zhserveriface.h"


enum pcbIFaceActionHandlerState {
    pcbIFaceIdle, // 0
    
    pcbIFaceConnectYourselfStart,
    pcbIFaceConnectionFinished,
    
    OpenChannelStart,
    OpenChannelFinished,
    
    ReadRangeStart,
    ReadRangeFinished, 
    
    SwitchRangeStart,
    SwitchRangeFinished,
    
    SetSamplingFrequencyStart,
    SetSamplingFrequencyFinished, // 10
    
    SetSamplingPSamplesStart,
    SetSamplingPSamplesFinished,
    
    SetTModeStart,
    SetTModeFinished,
    
    SetSyncSourceStart,
    SetSyncSourceFinished,
    
    SetSyncTimingStart,
    SetSyncTimingFinished,
    
    ReadGainCorrectionStart,
    ReadGainCorrectionFinished,
    
    ReadPhaseCorrectionStart,
    ReadPhaseCorrectionFinished,
        
    setPhaseNodeInfoStart,
    setPhaseNodeInfoFinished,
    
    cmpPhaseCoefficientStart,
    cmpPhaseCoefficientFinished,
        
    JustFlashProgStart,
    JustFlashProgFinished,
    
    JustFlashExportStart,
    JustFlashExportFinished,
    
    JustFlashImportStart,
    JustFlashImportFinished,
    
    JustFlashGetChksumStart,
    JustFlashGetChksumFinished,
    
    PCBReadDeviceVersionStart,
    PCBReadDeviceVersionFinished,
    
    PCBReadServerVersionStart,
    PCBReadServerVersionFinished,
    
    PCBReadSerialNrStart,
    PCBReadSerialNrFinished,
    
    GetAdjStatusStart,
    GetAdjStatusFinished,
    
    SetSenseProtectionStart,
    SetSenseProtectionFinished,
    
    pcbIFaceContinue
};


class cPCBIFace: public cZHServerIFace
{
    Q_OBJECT
    
public:
    cPCBIFace(QString, int);
    ~cPCBIFace();
    
    virtual void connectYourself(); // verbinde dich mit deinem server
    void openChannel(int); // messkanal öffnen
    void readRange(int); // den akt. bereich auslesen
    void switchRange(int, QString); // schaltet im kanal(int) den bereich (qstring)
    void setSamplingFrequency(float); 
    void setSamplingPSamples(int);
    void setTMode(int); 
    void setSyncSource(int);
    void setSyncTiming(int);
    void readGainCorrection(int, QString, float);
    void readPhaseCorrection(int, QString, float);
    void GetAdjustmentStatus();
    void setPhaseNodeInfo(QString, QString, int, float, float);
    void cmpPhaseCoefficient(QString);
    void SetSenseProtection(int);
       
    void JustFlashProgram();
    void JustFlashExport(QString);
    void JustFlashImport(QString);
    void JustFlashGetChksum();
    void ReadDeviceVersion();
    void ReadServerVersion();
    void ReadSerialNr();
    
protected slots:
    void ActionHandler(int);
    virtual void SocketDoneSlot();    
    virtual void SocketErrorSlot();    
    
private:
    QString m_sHost; // host name
    int m_nPort; // host port
    cSMTimer* m_ActTimer;
    
    void SendOpenChannelCommand();
    void SendReadRangeCommand();
    void SendSwitchRangeCommand();
    void SendSetSamplingFrequencyCommand();
    void SendSetSamplingPSamplesCommand();
    void SendSetTModeCommand();
    void SendSetSyncSourceCommand();
    void SendSetSyncTimingCommand();
    void SendReadGainCorrectionCommand();
    void SendReadPhaseCorrectionCommand();
    void SendsetPhaseNInfoCommand();
    void SendcmpPhaseCoefficientCommand();
    void SendSetSenseProtectionCommand();
    
    void SendJustFlashProgCommand();
    void SendJustFlashExportCommand();
    void SendJustFlashImportCommand();
    void SendReadDeviceVersionCommand();
    void SendReadServerVersionCommand();
    void SendReadSerialNrCommand();
    void SendReadFlashChksumCommand();
    void SendGetAdjStatusCommand();
    
    bool m_biFaceError;
    int m_nP1;
    float m_fP1, m_fP2;
    QString m_sP1, m_sP2; 
};
    
#endif
